/// \file RendererBindings.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>
#include <wren.h>

#include "src/JamUtil.h"
#include "src/RendererBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"
#include "src/Util.h"

// Globals
extern SDL_Window *gWindow; // -- from src/Runtime.c
extern bool gOutsideFrame;  // -- from src/Runtime.c
static VK2DShader gShader = NULL;
static int gShaderSize = 0;
static WrenHandle *gShaderBuffer;
static JUFont gDefaultFont;
static VK2DTexture gDefaultFontTexture;

// For shadows
VK2DShadowEnvironment gShadowEnvironment;    // For lighting
static _vksk_LightSource *gLightSources;
static int gLightSourcesCount = 0;
static VK2DTexture gShadowMapTexture;
static float gDrawnWidth;
static float gDrawnHeight;

// For adding lights to the lighting setup
int _vksk_RendererAddLightSource(float x, float y, float rotation, float originX, float originY, VK2DTexture tex) {
    int index = -1;
    const int extension = 10;

    // Search for available index
    for (int i = 0; i < gLightSourcesCount && index == -1; i++) {
        if (gLightSources[i].enabled == false)
            index = i;
    }

    // Extend list if no available slot
    if (index == -1) {
        void *newMem = realloc(gLightSources, sizeof(_vksk_LightSource) * (gLightSourcesCount + extension));

        if (newMem != NULL) {
            gLightSources = newMem;
            for (int i = gLightSourcesCount; i < gLightSourcesCount + extension; i++)
                gLightSources[i].enabled = false;

            index = gLightSourcesCount;
            gLightSourcesCount += 10;
        } else {
            vksk_Error(true, "Out of memory");
        }
    }

    gLightSources[index].enabled = true;
    gLightSources[index].x = x;
    gLightSources[index].y = y;
    gLightSources[index].rotation = rotation;
    gLightSources[index].originX = originX;
    gLightSources[index].originY = originY;
    gLightSources[index].scaleX = 1;
    gLightSources[index].scaleY = 1;
    gLightSources[index].colour[0] = 1;
    gLightSources[index].colour[1] = 1;
    gLightSources[index].colour[2] = 1;
    gLightSources[index].colour[3] = 1;
    gLightSources[index].tex = tex;

    return index;
}

_vksk_LightSource *_vkskRendererGetLightSource(int index) {
    if (index < gLightSourcesCount) {
        return &gLightSources[index];
    } else {
        vksk_Error(true, "Invalid light source");
        return NULL;
    }
}

void _vksk_RendererRemoveLightSource(int index) {
    if (index < gLightSourcesCount) {
        gLightSources[index].enabled = false;
    } else {
        vksk_Error(true, "Invalid light source");
    }
}

void _vksk_RendererResetLights() {
    gLightSourcesCount = 0;
    free(gLightSources);
    gLightSources = NULL;
}

// Updates shader data
void _vksk_RendererUpdateShaderBinding(VK2DShader shader, WrenHandle *buffer) {
	if (gShader == shader)
		gShaderBuffer = buffer;
}

// Macro for not letting user draw outside of frame
#define CHECK_VALID_DRAW() if (gOutsideFrame) {vksk_Error(true, "Drawing may not be performed in Level.pre_frame() or Level.destroy()");return;}

VK2DMSAA convertToMSAAEnum(double wrenVal) {
	if (wrenVal == 2)
		return VK2D_MSAA_2X;
	if (wrenVal == 4)
		return VK2D_MSAA_4X;
	if (wrenVal == 8)
		return VK2D_MSAA_8X;
	if (wrenVal == 16)
		return VK2D_MSAA_16X;
	if (wrenVal == 32)
		return VK2D_MSAA_32X;
	return VK2D_MSAA_1X;
}

VK2DFilterType convertToFilterTypeEnum(double wrenVal) {
	if (wrenVal == 1)
		return VK2D_FILTER_TYPE_LINEAR;
	return VK2D_FILTER_TYPE_NEAREST;
}

VK2DScreenMode convertToScreenModeEnum(double wrenVal) {
	if (wrenVal == 1)
		return VK2D_SCREEN_MODE_VSYNC;
	if (wrenVal == 2)
		return VK2D_SCREEN_MODE_TRIPLE_BUFFER;
	return VK2D_SCREEN_MODE_IMMEDIATE;
}

double convertFromMSAAEnum(VK2DMSAA val) {
	if (val == VK2D_MSAA_2X)
		return 2;
	if (val == VK2D_MSAA_4X)
		return 4;
	if (val == VK2D_MSAA_8X)
		return 8;
	if (val == VK2D_MSAA_16X)
		return 16;
	if (val == VK2D_MSAA_32X)
		return 32;
	return 1;
}

double convertFromFilterTypeEnum(VK2DFilterType val) {
	if (val == VK2D_FILTER_TYPE_LINEAR)
		return 1;
	return 0;
}

double convertFromScreenModeEnum(VK2DScreenMode val) {
	if (val == VK2D_SCREEN_MODE_VSYNC)
		return 1;
	if (val == VK2D_SCREEN_MODE_TRIPLE_BUFFER)
		return 2;
	return 0;
}

// Uses 3 slots in front of mapSlot
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, bool *fullscreen, VK2DRendererConfig *config) {
	// TODO: Check that the key exists first

	wrenEnsureSlots(vm, 3 + mapSlot);
	int keySlot = mapSlot + 1;
	int valueSlot = mapSlot + 2;

	// Window name
	wrenSetSlotString(vm, mapSlot + 1, "window_title");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*windowTitle = wrenGetSlotString(vm, valueSlot);

	// Window width
	wrenSetSlotString(vm, mapSlot + 1, "window_width");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*windowWidth = (int)wrenGetSlotDouble(vm, valueSlot);

	// Window height
	wrenSetSlotString(vm, mapSlot + 1, "window_height");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*windowHeight = (int)wrenGetSlotDouble(vm, valueSlot);

	// Fullscreen
	wrenSetSlotString(vm, mapSlot + 1, "fullscreen");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*fullscreen = wrenGetSlotBool(vm, valueSlot);

	// Config MSAA
	wrenSetSlotString(vm, mapSlot + 1, "msaa");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	config->msaa = convertToMSAAEnum(wrenGetSlotDouble(vm, valueSlot));

	// Config screen mode
	wrenSetSlotString(vm, mapSlot + 1, "screen_mode");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	config->screenMode = convertToScreenModeEnum(wrenGetSlotDouble(vm, valueSlot));

	// Config filter type
	wrenSetSlotString(vm, mapSlot + 1, "filter_type");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	config->filterMode = convertToFilterTypeEnum(wrenGetSlotDouble(vm, valueSlot));
}

static void *vksk_GetShaderData(WrenVM *vm) {
	if (gShaderBuffer != NULL) {
		wrenSetSlotHandle(vm, 0, gShaderBuffer);
		VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
		return buffer->buffer.data;
	} else if (gShaderBuffer == NULL && gShaderSize > 0) {
		vksk_Error(true, "Shader expected buffer data but none was provided.");
	}
	return NULL;
}

// Draws using the current shader
static void _vksk_DrawTexture(WrenVM *vm, VK2DTexture tex, float x, float y, float xScale, float yScale, float rot, float xOrigin, float yOrigin, float xInTex, float yInTex, float wInTex, float hInTex) {
	if (gShader == NULL)
		vk2dRendererDrawTexture(tex, x, y, xScale, yScale, rot, xOrigin, yOrigin, xInTex, yInTex, wInTex, hInTex);
	else
		vk2dRendererDrawShader(gShader, vksk_GetShaderData(vm), tex, x, y, xScale, yScale, rot, xOrigin, yOrigin, xInTex, yInTex, wInTex, hInTex);
}

void _vksk_RendererBindingsInit(void *textureData, int size) {
    gDefaultFontTexture = vk2dTextureFrom(textureData, size);
    gDefaultFont = juFontLoadFromTexture(gDefaultFontTexture, 32, 128, 7 * 3, 8 * 3);
    gShadowEnvironment = vk2DShadowEnvironmentCreate();
}

void _vksk_RendererBindingsQuit(WrenVM *vm) {
    juFontFree(gDefaultFont);
    vk2dTextureFree(gShadowMapTexture);
    vk2DShadowEnvironmentFree(gShadowEnvironment);
}

void vksk_RuntimeRendererGetWindowWidth(WrenVM *vm) {
	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);
	wrenSetSlotDouble(vm, 0, (double)w);
}

void vksk_RuntimeRendererGetWindowHeight(WrenVM *vm) {
	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);
	wrenSetSlotDouble(vm, 0, (double)h);
}

void vksk_RuntimeRendererSetWindowSize(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	SDL_SetWindowSize(gWindow, (int)wrenGetSlotDouble(vm, 1), (int)wrenGetSlotDouble(vm, 2));
}

void vksk_RuntimeRendererGetWindowFullscreen(WrenVM *vm) {
	uint32_t flags = SDL_GetWindowFlags(gWindow);
	wrenSetSlotBool(vm, 0, flags & SDL_WINDOW_FULLSCREEN || flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void vksk_RuntimeRendererSetWindowFullscreen(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL, FOREIGN_END)
	SDL_SetWindowFullscreen(gWindow, wrenGetSlotBool(vm, 1) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void vksk_RuntimeRendererDrawCircle(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	vk2dRendererDrawCircle(
			wrenGetSlotDouble(vm, 1),
			wrenGetSlotDouble(vm, 2),
			wrenGetSlotDouble(vm, 3)
			);
}

void vksk_RuntimeRendererDrawTextureExt(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float xscale = wrenGetSlotDouble(vm, 4);
	float yscale = wrenGetSlotDouble(vm, 5);
	float rot = wrenGetSlotDouble(vm, 6);
	float ox = wrenGetSlotDouble(vm, 7);
	float oy = wrenGetSlotDouble(vm, 8);
	_vksk_DrawTexture(vm, tex->texture.tex, x, y, xscale, yscale, rot, ox, oy, 0, 0, vk2dTextureWidth(tex->texture.tex), vk2dTextureHeight(tex->texture.tex));
}

void vksk_RuntimeRendererDrawTexture(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	_vksk_DrawTexture(vm, tex->texture.tex, x, y, 1, 1, 0, 0, 0, 0, 0, vk2dTextureWidth(tex->texture.tex), vk2dTextureHeight(tex->texture.tex));
}

void vksk_RuntimeRendererDrawTexturePartExt(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float xscale = wrenGetSlotDouble(vm, 4);
	float yscale = wrenGetSlotDouble(vm, 5);
	float rot = wrenGetSlotDouble(vm, 6);
	float ox = wrenGetSlotDouble(vm, 7);
	float oy = wrenGetSlotDouble(vm, 8);
	float xt = wrenGetSlotDouble(vm, 9);
	float yt = wrenGetSlotDouble(vm, 10);
	float tw = wrenGetSlotDouble(vm, 11);
	float th = wrenGetSlotDouble(vm, 12);
	_vksk_DrawTexture(vm, tex->texture.tex, x, y, xscale, yscale, rot, ox, oy, xt, yt, tw, th);
}

void vksk_RuntimeRendererDrawTexturePart(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float xt = wrenGetSlotDouble(vm, 4);
	float yt = wrenGetSlotDouble(vm, 5);
	float tw = wrenGetSlotDouble(vm, 6);
	float th = wrenGetSlotDouble(vm, 7);
	_vksk_DrawTexture(vm, tex->texture.tex, x, y, 1, 1, 0, 0, 0, xt, yt, tw, th);
}

// vksk_RuntimeRendererGetConfig() - get_config()
void vksk_RuntimeRendererGetConfig(WrenVM *vm) {
	wrenEnsureSlots(vm, 3);
	wrenSetSlotNewMap(vm, 0);
	int keySlot = 1;
	int valSlot = 2;
	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);
	VK2DRendererConfig conf = vk2dRendererGetConfig();

	// Title
	wrenSetSlotString(vm, keySlot, "window_title");
	wrenSetSlotString(vm, valSlot, SDL_GetWindowTitle(gWindow));
	wrenSetMapValue(vm, 0, keySlot, valSlot);

	// Width
	wrenSetSlotString(vm, keySlot, "window_width");
	wrenSetSlotDouble(vm, valSlot, (double)w);
	wrenSetMapValue(vm, 0, keySlot, valSlot);

	// Height
	wrenSetSlotString(vm, keySlot, "window_height");
	wrenSetSlotDouble(vm, valSlot, (double)h);
	wrenSetMapValue(vm, 0, keySlot, valSlot);

	// Fullscreen
	wrenSetSlotString(vm, keySlot, "fullscreen");
	wrenSetSlotBool(vm, valSlot, SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP);
	wrenSetMapValue(vm, 0, keySlot, valSlot);

	// MSAA
	wrenSetSlotString(vm, keySlot, "msaa");
	wrenSetSlotDouble(vm, valSlot, convertFromMSAAEnum(conf.msaa));
	wrenSetMapValue(vm, 0, keySlot, valSlot);

	// Screen mode
	wrenSetSlotString(vm, keySlot, "screen_mode");
	wrenSetSlotDouble(vm, valSlot, convertFromScreenModeEnum(conf.screenMode));
	wrenSetMapValue(vm, 0, keySlot, valSlot);

	// Filter type
	wrenSetSlotString(vm, keySlot, "filter_type");
	wrenSetSlotDouble(vm, valSlot, convertFromFilterTypeEnum(conf.filterMode));
	wrenSetMapValue(vm, 0, keySlot, valSlot);
}

// vksk_RuntimeRendererSetConfig(VK2DRendererConfig config) - set_config(_)
void vksk_RuntimeRendererSetConfig(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_MAP, FOREIGN_END)
	const char *windowTitle;
	int windowWidth, windowHeight;
	bool fullscreen;
	VK2DRendererConfig rendererConfig;
	vksk_LoadVK2DConfigFromMap(vm, 1, &windowTitle, &windowWidth, &windowHeight, &fullscreen, &rendererConfig);
	vk2dRendererSetConfig(rendererConfig);
	SDL_SetWindowSize(gWindow, windowWidth, windowHeight);
	SDL_SetWindowFullscreen(gWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

// vksk_RuntimeRendererSetTarget(VK2DTexture target) - set_target(_)
void vksk_RuntimeRendererSetTarget(WrenVM *vm) {
	CHECK_VALID_DRAW()
	if (wrenGetSlotType(vm, 1) != WREN_TYPE_NULL) {
		VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SURFACE, FOREIGN_END)
		VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
		vk2dRendererSetTarget(tex->surface);
	} else {
		vk2dRendererSetTarget(VK2D_TARGET_SCREEN);
	}
}

// vksk_RuntimeRendererSetBlendMode(VK2DBlendMode blendMode) - set_blend_mode(_)
void vksk_RuntimeRendererSetBlendMode(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	vk2dRendererSetBlendMode(wrenGetSlotDouble(vm, 1));
}

// vksk_RuntimeRendererGetBlendMode() - get_blend_mode()
void vksk_RuntimeRendererGetBlendMode(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, vk2dRendererGetBlendMode());
}

// vksk_RuntimeRendererSetColourMod(const vec4 mod) - set_colour_mod(_)
void vksk_RuntimeRendererSetColourMod(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST | FOREIGN_STRING, FOREIGN_END)
	vec4 vec;
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_LIST) {
		wrenEnsureSlots(vm, 3);
		for (int i = 0; i < 4; i++) {
			wrenGetListElement(vm, 1, i, 2);
			vec[i] = wrenGetSlotDouble(vm, 2);
		}
	} else {
		vk2dColourHex(vec, wrenGetSlotString(vm, 1));
	}
	vk2dRendererSetColourMod(vec);
}

// vksk_RuntimeRendererGetColourMod(vec4 dst) - get_colour_mod()
void vksk_RuntimeRendererGetColourMod(WrenVM *vm) {
	vec4 vec;
	vk2dRendererGetColourMod(vec);
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < 4; i++) {
		wrenSetSlotDouble(vm, 1, vec[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void vksk_RuntimeRendererSetShader(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SHADER | FOREIGN_NULL, FOREIGN_END)
	CHECK_VALID_DRAW()
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_NULL) {
		gShader = NULL;
		gShaderSize = 0;
		gShaderBuffer = NULL;
	} else {
		VKSK_RuntimeForeign *shader = wrenGetSlotForeign(vm, 1);
		gShader = shader->shader.shader;
		gShaderSize = shader->shader.size;
		gShaderBuffer = shader->shader.data;
	}
}

// vksk_RuntimeRendererSetTextureCamera(bool useCameraOnTextures) - set_texture_camera(_)
void vksk_RuntimeRendererSetTextureCamera(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL, FOREIGN_END)
	vk2dRendererSetTextureCamera(wrenGetSlotBool(vm, 1));
}

// vksk_RuntimeRendererGetAverageFrameTime() - average_frame_time()
void vksk_RuntimeRendererGetAverageFrameTime(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, vk2dRendererGetAverageFrameTime());
}

// vksk_RuntimeRendererLockCameras(VK2DCameraIndex cam) - lock_cameras(_)
void vksk_RuntimeRendererLockCameras(WrenVM *vm) {
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_NULL) {
		vk2dRendererLockCameras(VK2D_DEFAULT_CAMERA);
	} else {
		VALIDATE_FOREIGN_ARGS(vm, FOREIGN_CAMERA, FOREIGN_END)
		VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 1);
		vk2dRendererLockCameras(cam->camera.index);
	}
}

// vksk_RuntimeRendererUnlockCameras() - unlock_cameras()
void vksk_RuntimeRendererUnlockCameras(WrenVM *vm) {
	vk2dRendererUnlockCameras();
}

// vksk_RuntimeRendererClear() - clear()
void vksk_RuntimeRendererClear(WrenVM *vm) {
	vk2dRendererClear();
}

void vksk_RuntimeRendererClearBlank(WrenVM *vm) {
	vk2dRendererEmpty();
}

// vksk_RuntimeRendererDrawRectangle(float x, float y, float w, float h, float r, float ox, float oy) - draw_rectangle(_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawRectangle(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	float x = wrenGetSlotDouble(vm, 1);
	float y = wrenGetSlotDouble(vm, 2);
	float w = wrenGetSlotDouble(vm, 3);
	float h = wrenGetSlotDouble(vm, 4);
	float r = wrenGetSlotDouble(vm, 5);
	float ox = wrenGetSlotDouble(vm, 6);
	float oy = wrenGetSlotDouble(vm, 7);
	vk2dRendererDrawRectangle(x, y, w, h, r, ox, oy);
}

// vksk_RuntimeRendererDrawRectangleOutline(float x, float y, float w, float h, float r, float ox, float oy, float lineWidth) - draw_rectangle_outline(_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawRectangleOutline(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	float x = wrenGetSlotDouble(vm, 1);
	float y = wrenGetSlotDouble(vm, 2);
	float w = wrenGetSlotDouble(vm, 3);
	float h = wrenGetSlotDouble(vm, 4);
	float r = wrenGetSlotDouble(vm, 5);
	float ox = wrenGetSlotDouble(vm, 6);
	float oy = wrenGetSlotDouble(vm, 7);
	float lw = wrenGetSlotDouble(vm, 8);
	vk2dRendererDrawRectangleOutline(x, y, w, h, r, ox, oy, lw);
}

// vksk_RuntimeRendererDrawCircleOutline(float x, float y, float r, float lineWidth) - draw_circle_outline(_,_,_,_)
void vksk_RuntimeRendererDrawCircleOutline(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	float x = wrenGetSlotDouble(vm, 1);
	float y = wrenGetSlotDouble(vm, 2);
	float w = wrenGetSlotDouble(vm, 3);
	float h = wrenGetSlotDouble(vm, 4);
	vk2dRendererDrawCircleOutline(x, y, w, h);
}

// vksk_RuntimeRendererDrawLine(float x1, float y1, float x2, float y2) - draw_line(_,_,_,_)
void vksk_RuntimeRendererDrawLine(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	float x = wrenGetSlotDouble(vm, 1);
	float y = wrenGetSlotDouble(vm, 2);
	float w = wrenGetSlotDouble(vm, 3);
	float h = wrenGetSlotDouble(vm, 4);
	vk2dRendererDrawLine(x, y, w, h);
}

void vksk_RuntimeRendererDrawFont(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BITMAP_FONT | FOREIGN_NULL, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	JUFont font;
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_FOREIGN)
	    font = ((VKSK_RuntimeForeign *)wrenGetSlotForeign(vm, 1))->bitmapFont;
	else
	    font = gDefaultFont;
	const char *str = wrenGetSlotString(vm, 2);
	float x = wrenGetSlotDouble(vm, 3);
	float y = wrenGetSlotDouble(vm, 4);
	juFontDraw(font, x, y, "%s", str);
}

void vksk_RuntimeRendererDrawFontWrapped(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BITMAP_FONT | FOREIGN_NULL, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	JUFont font;
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_FOREIGN)
	    font = ((VKSK_RuntimeForeign *)wrenGetSlotForeign(vm, 1))->bitmapFont;
	else
	    font = gDefaultFont;
	const char *str = wrenGetSlotString(vm, 2);
	float x = wrenGetSlotDouble(vm, 3);
	float y = wrenGetSlotDouble(vm, 4);
	float w = wrenGetSlotDouble(vm, 5);
	juFontDrawWrapped(font, x, y, w, "%s", str);
}

void vksk_RuntimeRendererDrawFontExt(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BITMAP_FONT | FOREIGN_NULL, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	JUFont font;
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_FOREIGN)
	    font = ((VKSK_RuntimeForeign *)wrenGetSlotForeign(vm, 1))->bitmapFont;
	else
	    font = gDefaultFont;
	const char *str = wrenGetSlotString(vm, 2);
	float x = wrenGetSlotDouble(vm, 3);
	float y = wrenGetSlotDouble(vm, 4);
	juFontDrawExt(font, x, y, str);
}

void vksk_RuntimeRendererDrawFontExtWrapped(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BITMAP_FONT | FOREIGN_NULL, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	JUFont font;
	if (wrenGetSlotType(vm, 1) == WREN_TYPE_FOREIGN)
	    font = ((VKSK_RuntimeForeign *)wrenGetSlotForeign(vm, 1))->bitmapFont;
	else
	    font = gDefaultFont;
	const char *str = wrenGetSlotString(vm, 2);
	float x = wrenGetSlotDouble(vm, 3);
	float y = wrenGetSlotDouble(vm, 4);
	float w = wrenGetSlotDouble(vm, 5);
	juFontDrawWrappedExt(font, x, y, w, str);
}


void vksk_RuntimeRendererDrawSpritePos(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SPRITE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 1);
	// First we check if we must advance a frame
	if ((double)(SDL_GetPerformanceCounter() - spr->sprite.spr->Internal.lastTime) / (double)SDL_GetPerformanceFrequency() >= spr->sprite.spr->delay) {
		spr->sprite.spr->Internal.frame = spr->sprite.spr->Internal.frame == spr->sprite.spr->Internal.frames - 1 ? 0 : spr->sprite.spr->Internal.frame + 1;
		spr->sprite.spr->Internal.lastTime = SDL_GetPerformanceCounter();
	}

	// Calculate where in the texture to draw
	float drawX = roundf(spr->sprite.spr->x + ((int)(spr->sprite.spr->Internal.frame * spr->sprite.spr->Internal.w) % (int)(vk2dTextureWidth(spr->sprite.spr->Internal.tex) - spr->sprite.spr->x)));
	float drawY = roundf(spr->sprite.spr->y + (spr->sprite.spr->Internal.h * floorf((spr->sprite.spr->Internal.frame * spr->sprite.spr->Internal.w) / (vk2dTextureWidth(spr->sprite.spr->Internal.tex) - spr->sprite.spr->x))));

	_vksk_DrawTexture(
			vm,
			spr->sprite.spr->Internal.tex,
			wrenGetSlotDouble(vm, 2) - (spr->sprite.spr->originX * spr->sprite.spr->scaleX),
			wrenGetSlotDouble(vm, 3) - (spr->sprite.spr->originY * spr->sprite.spr->scaleY),
			spr->sprite.spr->scaleX,
			spr->sprite.spr->scaleY,
			spr->sprite.spr->rotation,
			spr->sprite.spr->originX,
			spr->sprite.spr->originY,
			drawX,
			drawY,
			spr->sprite.spr->Internal.w,
			spr->sprite.spr->Internal.h);
}

void vksk_RuntimeRendererDrawSpriteFrame(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SPRITE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 1);
	int index = (int)wrenGetSlotDouble(vm, 2);
	if (index >= 0 && index < spr->sprite.spr->Internal.frames) {
		// Calculate where in the texture to draw
		float drawX = roundf(spr->sprite.spr->x + ((int)(index * spr->sprite.spr->Internal.w) % (int)(vk2dTextureWidth(spr->sprite.spr->Internal.tex) - spr->sprite.spr->x)));
		float drawY = roundf(spr->sprite.spr->y + (spr->sprite.spr->Internal.h * floorf((index * spr->sprite.spr->Internal.w) / (vk2dTextureWidth(spr->sprite.spr->Internal.tex) - spr->sprite.spr->x))));

		_vksk_DrawTexture(
				vm,
				spr->sprite.spr->Internal.tex,
				wrenGetSlotDouble(vm, 3) - (spr->sprite.spr->originX * spr->sprite.spr->scaleX),
				wrenGetSlotDouble(vm, 4) - (spr->sprite.spr->originY * spr->sprite.spr->scaleY),
				spr->sprite.spr->scaleX,
				spr->sprite.spr->scaleY,
				spr->sprite.spr->rotation,
				spr->sprite.spr->originX,
				spr->sprite.spr->originY,
				drawX,
				drawY,
				spr->sprite.spr->Internal.w,
				spr->sprite.spr->Internal.h);
	}
}

void vksk_RuntimeRendererDrawModel(WrenVM *vm) {
	// model, x, y, z
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_MODEL, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *model = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float z = wrenGetSlotDouble(vm, 4);
	vec3 axis = {0};
	vk2dRendererDrawModel(model->model, x, y, z, 1, 1, 1, 0, axis, 0, 0, 0);
}

void vksk_RuntimeRendererDrawModelExt(WrenVM *vm) {
	// model, x, y, z, x_scale, y_scale, z_scale, rotation, axis, origin_x, origin_y, origin_z
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_MODEL, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_LIST, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *model = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float z = wrenGetSlotDouble(vm, 4);
	float xScale = wrenGetSlotDouble(vm, 5);
	float yScale = wrenGetSlotDouble(vm, 6);
	float zScale = wrenGetSlotDouble(vm, 7);
	float rot = wrenGetSlotDouble(vm, 8);
	float xOrigin = wrenGetSlotDouble(vm, 10);
	float yOrigin = wrenGetSlotDouble(vm, 11);
	float zOrigin = wrenGetSlotDouble(vm, 12);
	vec3 axis = {0};
	if (wrenGetListCount(vm, 9) >= 3) {
		wrenGetListElement(vm, 9, 0, 1);
		axis[0] = wrenGetSlotDouble(vm, 1);
		wrenGetListElement(vm, 9, 1, 1);
		axis[1] = wrenGetSlotDouble(vm, 1);
		wrenGetListElement(vm, 9, 2, 1);
		axis[2] = wrenGetSlotDouble(vm, 1);
	}

	vk2dRendererDrawModel(model->model, x, y, z, xScale, yScale, zScale, rot, axis, xOrigin, yOrigin, zOrigin);
}

void vksk_RuntimeRendererDrawPolygon(WrenVM *vm) {
	// polygon, x, y
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_POLYGON, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *polygon = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	vk2dDrawPolygon(polygon->polygon, x, y);
}

void vksk_RuntimeRendererDrawPolygonExt(WrenVM *vm) {
	// polygon, x, y, x_scale, y_scale, rotation, origin_x, origin_y, outline, lineWidth
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_POLYGON, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_BOOL, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *polygon = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float xScale = wrenGetSlotDouble(vm, 4);
	float yScale = wrenGetSlotDouble(vm, 5);
	float rot = wrenGetSlotDouble(vm, 6);
	float xOrigin = wrenGetSlotDouble(vm, 7);
	float yOrigin = wrenGetSlotDouble(vm, 8);
	bool outline = wrenGetSlotBool(vm, 9);
	float lineWidth = wrenGetSlotDouble(vm, 10);
	vk2dRendererDrawPolygon(polygon->polygon, x, y, !outline, lineWidth, xScale, yScale, rot, xOrigin, yOrigin);
}

void vksk_RuntimeRendererSetupLighting(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    float internalWidth = wrenGetSlotDouble(vm, 1);
    float internalHeight = wrenGetSlotDouble(vm, 2);
    gDrawnWidth = wrenGetSlotDouble(vm, 3);
    gDrawnHeight = wrenGetSlotDouble(vm, 4);

    vk2dRendererWait();
    vk2dTextureFree(gShadowMapTexture);
    gShadowMapTexture = vk2dTextureCreate(internalWidth, internalHeight);
    if (gShadowMapTexture == NULL) {
        vksk_Error(true, "Failed to create shadow map internal texture of size (%f/%f)", internalWidth, internalHeight);
    }
}

void vksk_RuntimeRendererDrawLighting(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SURFACE | FOREIGN_NULL, FOREIGN_CAMERA, FOREIGN_CAMERA, FOREIGN_END)
    const VKSK_RuntimeForeign *surfaceForeign = wrenGetSlotType(vm, 1) == WREN_TYPE_NULL ? NULL : wrenGetSlotForeign(vm, 1);
    const VKSK_RuntimeForeign *gameCamera = wrenGetSlotForeign(vm, 2);
    const VKSK_RuntimeForeign *uiCamera = wrenGetSlotForeign(vm, 3);
    const VK2DTexture surface = surfaceForeign == NULL ? VK2D_TARGET_SCREEN : surfaceForeign->surface;
    for (int i = 0; i < gLightSourcesCount; i++) {
        if (!gLightSources[i].enabled)
            continue;

        // Prepare light texture
        vk2dRendererLockCameras(gameCamera->camera.index);
        vk2dRendererSetTarget(gShadowMapTexture);
        vk2dRendererEmpty();

        // Draw light orb
        vk2dRendererSetColourMod(gLightSources[i].colour);
        vk2dRendererDrawTexture(
            gLightSources[i].tex,
            gLightSources[i].x - gLightSources[i].originX,
            gLightSources[i].y - gLightSources[i].originY,
            gLightSources[i].scaleX,
            gLightSources[i].scaleY,
            gLightSources[i].rotation,
            gLightSources[i].originX,
            gLightSources[i].originY,
            0,
            0,
            vk2dTextureWidth(gLightSources[i].tex),
            vk2dTextureHeight(gLightSources[i].tex)
        );
        vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);

        // Subtract shadows from it
        vk2dRendererSetBlendMode(VK2D_BLEND_MODE_SUBTRACT);
        vec2 pos = {gLightSources[i].x, gLightSources[i].y};
        vk2dRendererDrawShadows(gShadowEnvironment, VK2D_BLACK, pos);
        vk2dRendererSetBlendMode(VK2D_BLEND_MODE_BLEND);

        // Draw it to the screen
        vk2dRendererLockCameras(uiCamera->camera.index);
        vk2dRendererSetTarget(surface);
        const float scaleX = gDrawnWidth / vk2dTextureWidth(gShadowMapTexture);
        const float scaleY = gDrawnHeight / vk2dTextureHeight(gShadowMapTexture);
        vk2dDrawTextureExt(
            gShadowMapTexture,
            0,
            0,
            scaleX,
            scaleY,
            0,
            0,
            0)
        ;
    }
}

void vksk_RuntimeRendererDrawFOV(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    const float x = wrenGetSlotDouble(vm, 1);
    const float y = wrenGetSlotDouble(vm, 2);
    vec4 c;
    vec2 l = {x, y};
    vk2dRendererGetColourMod(c);
    vk2dRendererDrawShadows(gShadowEnvironment, c, l);
}

void vksk_RuntimeRendererHideCursor(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL, FOREIGN_END)
    SDL_ShowCursor(wrenGetSlotBool(vm, 1) ? SDL_ENABLE : SDL_DISABLE);
}