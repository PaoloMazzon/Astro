/// \file RendererBindings.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>
#include <wren.h>

#include "src/RendererBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"

// Globals
extern SDL_Window *gWindow; // -- from src/Runtime.c

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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	vk2dRendererDrawCircle(
			wrenGetSlotDouble(vm, 1),
			wrenGetSlotDouble(vm, 2),
			wrenGetSlotDouble(vm, 3)
			);
}

void vksk_RuntimeRendererDrawTextureExt(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Texture", FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float xscale = wrenGetSlotDouble(vm, 4);
	float yscale = wrenGetSlotDouble(vm, 5);
	float rot = wrenGetSlotDouble(vm, 6);
	float ox = wrenGetSlotDouble(vm, 7);
	float oy = wrenGetSlotDouble(vm, 8);
	vk2dRendererDrawTexture(tex->tex, x, y, xscale, yscale, rot, ox, oy, 0, 0, tex->tex->img->width, tex->tex->img->height);
}

void vksk_RuntimeRendererDrawTexture(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Texture", FOREIGN_NUM, FOREIGN_NUM)
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	vk2dRendererDrawTexture(tex->tex, x, y, 1, 1, 0, 0, 0, 0, 0, tex->tex->img->width, tex->tex->img->height);
}

void vksk_RuntimeRendererDrawTexturePartExt(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Texture", FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 1);
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
	vk2dRendererDrawTexture(tex->tex, x, y, xscale, yscale, rot, ox, oy, xt, yt, tw, th);
}

void vksk_RuntimeRendererDrawTexturePart(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Texture", FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 1);
	float x = wrenGetSlotDouble(vm, 2);
	float y = wrenGetSlotDouble(vm, 3);
	float xt = wrenGetSlotDouble(vm, 4);
	float yt = wrenGetSlotDouble(vm, 5);
	float tw = wrenGetSlotDouble(vm, 6);
	float th = wrenGetSlotDouble(vm, 7);
	vk2dRendererDrawTexture(tex->tex, x, y, 1, 1, 0, 0, 0, xt, yt, tw, th);
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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_MAP)
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
	if (wrenGetSlotType(vm, 1) != WREN_TYPE_NULL) {
		VALIDATE_FOREIGN_ARGS(vm, "Texture")
		_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 1);
		vk2dRendererSetTarget(tex->tex);
	} else {
		vk2dRendererSetTarget(VK2D_TARGET_SCREEN);
	}
}

// vksk_RuntimeRendererSetBlendMode(VK2DBlendMode blendMode) - set_blend_mode(_)
void vksk_RuntimeRendererSetBlendMode(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	vk2dRendererSetBlendMode(wrenGetSlotDouble(vm, 1));
}

// vksk_RuntimeRendererGetBlendMode() - get_blend_mode()
void vksk_RuntimeRendererGetBlendMode(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, vk2dRendererGetBlendMode());
}

// vksk_RuntimeRendererSetColourMod(const vec4 mod) - set_colour_mod(_)
void vksk_RuntimeRendererSetColourMod(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST)
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

// vksk_RuntimeRendererSetTextureCamera(bool useCameraOnTextures) - set_texture_camera(_)
void vksk_RuntimeRendererSetTextureCamera(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL)
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
		VALIDATE_FOREIGN_ARGS(vm, "Camera")
		_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 1);
		vk2dRendererLockCameras(cam->index);
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

// vksk_RuntimeRendererDrawRectangle(float x, float y, float w, float h, float r, float ox, float oy) - draw_rectangle(_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawRectangle(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	float x = wrenGetSlotDouble(vm, 1);
	float y = wrenGetSlotDouble(vm, 2);
	float w = wrenGetSlotDouble(vm, 3);
	float h = wrenGetSlotDouble(vm, 4);
	vk2dRendererDrawCircleOutline(x, y, w, h);
}

// vksk_RuntimeRendererDrawLine(float x1, float y1, float x2, float y2) - draw_line(_,_,_,_)
void vksk_RuntimeRendererDrawLine(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	float x = wrenGetSlotDouble(vm, 1);
	float y = wrenGetSlotDouble(vm, 2);
	float w = wrenGetSlotDouble(vm, 3);
	float h = wrenGetSlotDouble(vm, 4);
	vk2dRendererDrawLine(x, y, w, h);
}

// vksk_RuntimeRendererDrawShader(VK2DShader shader, VK2DTexture tex, float x, float y, float xscale, float yscale, float rot, float originX, float originY, float xInTex, float yInTex, float texWidth, float texHeight) - draw_shader(_,_,_,_,_,_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawShader(WrenVM *vm) {
	// TODO: This
}

// vksk_RuntimeRendererDrawPolygon(VK2DPolygon polygon, float x, float y, bool filled, float lineWidth, float xscale, float yscale, float rot, float originX, float originY) - draw_polygon(_,_,_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawPolygon(WrenVM *vm) {
	// TODO: This
}

// vksk_RuntimeRendererDrawModel(VK2DModel model, float x, float y, float z, float xscale, float yscale, float zscale, float rot, vec3 axis, float originX, float originY, float originZ) - draw_model(_,_,_,_,_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawModel(WrenVM *vm) {
	// TODO: This
}

void vksk_RuntimeRendererDrawFont(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "BitmapFont", FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM)
	JUFont *font = wrenGetSlotForeign(vm, 1);
	const char *str = wrenGetSlotString(vm, 2);
	float x = wrenGetSlotDouble(vm, 3);
	float y = wrenGetSlotDouble(vm, 4);
	juFontDraw(*font, x, y, "%s", str);
}

void vksk_RuntimeRendererDrawSpritePos(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Sprite", FOREIGN_NUM, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 1);
	juSpriteDraw(*spr, wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vksk_RuntimeRendererDrawSpriteFrame(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Sprite", FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 1);
	juSpriteDrawFrame(*spr, wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3), wrenGetSlotDouble(vm, 4));
}

