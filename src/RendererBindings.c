/// \file RendererBindings.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>
#include <wren.h>

#include "src/JamUtil.h"
#include "src/RendererBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"

// Globals
extern SDL_Window *gWindow; // -- from src/Runtime.c
extern bool gOutsideFrame; // -- from src/Runtime.c
static VK2DShader gShader = NULL;

// Macro for not letting user draw outside of frame
#define CHECK_VALID_DRAW() if (gOutsideFrame) {vksk_Error(true, "Drawing may not be performed in Level.pre_frame()");return;}

VK2DMSAA convertToMSAAEnum(double wrenVal) {
	if (wrenVal == 2)
		return msaa_2x;
	if (wrenVal == 4)
		return msaa_4x;
	if (wrenVal == 8)
		return msaa_8x;
	if (wrenVal == 16)
		return msaa_16x;
	if (wrenVal == 32)
		return msaa_32x;
	return msaa_1x;
}

VK2DFilterType convertToFilterTypeEnum(double wrenVal) {
	if (wrenVal == 1)
		return ft_Linear;
	return ft_Nearest;
}

VK2DScreenMode convertToScreenModeEnum(double wrenVal) {
	if (wrenVal == 1)
		return sm_VSync;
	if (wrenVal == 2)
		return sm_TripleBuffer;
	return sm_Immediate;
}

double convertFromMSAAEnum(VK2DMSAA val) {
	if (val == msaa_2x)
		return 2;
	if (val == msaa_4x)
		return 4;
	if (val == msaa_8x)
		return 8;
	if (val == msaa_16x)
		return 16;
	if (val == msaa_32x)
		return 32;
	return 1;
}

double convertFromFilterTypeEnum(VK2DFilterType val) {
	if (val == ft_Linear)
		return 1;
	return 0;
}

double convertFromScreenModeEnum(VK2DScreenMode val) {
	if (val == sm_VSync)
		return 1;
	if (val == sm_TripleBuffer)
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
	if (gShader == NULL)
		vk2dRendererDrawTexture(tex->texture.tex, x, y, xscale, yscale, rot, ox, oy, 0, 0, tex->texture.tex->img->width, tex->texture.tex->img->height);
	else
		vk2dRendererDrawShader(gShader, tex->texture.tex, x, y, xscale, yscale, rot, ox, oy, 0, 0, tex->texture.tex->img->width, tex->texture.tex->img->height);
}

void vksk_RuntimeRendererDrawTexture(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	if (gShader == NULL)
		vk2dRendererDrawTexture(tex->texture.tex, x, y, 1, 1, 0, 0, 0, 0, 0, tex->texture.tex->img->width, tex->texture.tex->img->height);
	else
		vk2dRendererDrawShader(gShader, tex->texture.tex, x, y, 1, 1, 0, 0, 0, 0, 0, tex->texture.tex->img->width, tex->texture.tex->img->height);
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
	if (gShader == NULL)
		vk2dRendererDrawTexture(tex->texture.tex, x, y, xscale, yscale, rot, ox, oy, xt, yt, tw, th);
	else
		vk2dRendererDrawShader(gShader, tex->texture.tex, x, y, xscale, yscale, rot, ox, oy, xt, yt, tw, th);
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
	if (gShader == NULL)
		vk2dRendererDrawTexture(tex->texture.tex, x, y, 1, 1, 0, 0, 0, xt, yt, tw, th);
	else
		vk2dRendererDrawShader(gShader, tex->texture.tex, x, y, 1, 1, 0, 0, 0, xt, yt, tw, th);
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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_END)
	vec4 vec;
	wrenEnsureSlots(vm, 3);
	for (int i = 0; i < 4; i++) {
		wrenGetListElement(vm, 1, i, 2);
		vec[i] = wrenGetSlotDouble(vm, 2);
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
	} else {
		VKSK_RuntimeForeign *shader = wrenGetSlotForeign(vm, 1);
		gShader = shader->shader;
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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BITMAP_FONT, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *font = wrenGetSlotForeign(vm, 1);
	const char *str = wrenGetSlotString(vm, 2);
	float x = wrenGetSlotDouble(vm, 3);
	float y = wrenGetSlotDouble(vm, 4);
	juFontDraw(font->bitmapFont, x, y, "%s", str);
}

void vksk_RuntimeRendererDrawSpritePos(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SPRITE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 1);
	if (gShader == NULL) {
		juSpriteDraw(spr->sprite.spr, wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3));
	} else {
		// First we check if we must advance a frame
		if ((double)(SDL_GetPerformanceCounter() - spr->sprite.spr->Internal.lastTime) / (double)SDL_GetPerformanceFrequency() >= spr->sprite.spr->delay) {
			spr->sprite.spr->Internal.frame = spr->sprite.spr->Internal.frame == spr->sprite.spr->Internal.frames - 1 ? 0 : spr->sprite.spr->Internal.frame + 1;
			spr->sprite.spr->Internal.lastTime = SDL_GetPerformanceCounter();
		}

		// Calculate where in the texture to draw
		float drawX = roundf(spr->sprite.spr->x + ((int)(spr->sprite.spr->Internal.frame * spr->sprite.spr->Internal.w) % (int)(spr->sprite.spr->Internal.tex->img->width - spr->sprite.spr->x)));
		float drawY = roundf(spr->sprite.spr->y + (spr->sprite.spr->Internal.h * floorf((spr->sprite.spr->Internal.frame * spr->sprite.spr->Internal.w) / (spr->sprite.spr->Internal.tex->img->width - spr->sprite.spr->x))));

		vk2dRendererDrawShader(
				gShader,
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
}

void vksk_RuntimeRendererDrawSpriteFrame(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_SPRITE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 1);
	if (gShader == NULL) {
		juSpriteDrawFrame(spr->sprite.spr, wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3), wrenGetSlotDouble(vm, 4));
	} else {
		int index = (int)wrenGetSlotDouble(vm, 2);
		if (index >= 0 && index < spr->sprite.spr->Internal.frames) {
			// Calculate where in the texture to draw
			float drawX = roundf(spr->sprite.spr->x + ((int)(index * spr->sprite.spr->Internal.w) % (int)(spr->sprite.spr->Internal.tex->img->width - spr->sprite.spr->x)));
			float drawY = roundf(spr->sprite.spr->y + (spr->sprite.spr->Internal.h * floorf((index * spr->sprite.spr->Internal.w) / (spr->sprite.spr->Internal.tex->img->width - spr->sprite.spr->x))));

			vk2dRendererDrawTexture(
					spr->sprite.spr->Internal.tex,
					wrenGetSlotDouble(vm, 3) - spr->sprite.spr->originX,
					wrenGetSlotDouble(vm, 4) - spr->sprite.spr->originY,
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
}

void vksk_RuntimeRendererDrawModel(WrenVM *vm) {
	// model, x, y, z
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_MODEL, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	CHECK_VALID_DRAW()
	VKSK_RuntimeForeign *model = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float z = wrenGetSlotDouble(vm, 4);
	vec3 axis = {};
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
	vec3 axis = {};
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
