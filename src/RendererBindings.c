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

// Uses 3 slots in front of mapSlot
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, VK2DRendererConfig *config) {
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
}

void vksk_RuntimeRendererDrawTexturePart(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "Texture", FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM)
}

// vksk_RuntimeRendererGetConfig() - get_config()
void vksk_RuntimeRendererGetConfig(WrenVM *vm) {

}

// vksk_RuntimeRendererSetConfig(VK2DRendererConfig config) - set_config(_)
void vksk_RuntimeRendererSetConfig(WrenVM *vm) {

}

// vksk_RuntimeRendererSetTarget(VK2DTexture target) - set_target(_)
void vksk_RuntimeRendererSetTarget(WrenVM *vm) {

}

// vksk_RuntimeRendererSetBlendMode(VK2DBlendMode blendMode) - set_blend_mode(_)
void vksk_RuntimeRendererSetBlendMode(WrenVM *vm) {

}

// vksk_RuntimeRendererGetBlendMode() - get_blend_mode()
void vksk_RuntimeRendererGetBlendMode(WrenVM *vm) {

}

// vksk_RuntimeRendererSetColourMod(const vec4 mod) - set_colour_mod(_)
void vksk_RuntimeRendererSetColourMod(WrenVM *vm) {

}

// vksk_RuntimeRendererGetColourMod(vec4 dst) - get_colour_mod()
void vksk_RuntimeRendererGetColourMod(WrenVM *vm) {

}

// vksk_RuntimeRendererSetTextureCamera(bool useCameraOnTextures) - set_texture_camera(_)
void vksk_RuntimeRendererSetTextureCamera(WrenVM *vm) {

}

// vksk_RuntimeRendererGetAverageFrameTime() - average_frame_time()
void vksk_RuntimeRendererGetAverageFrameTime(WrenVM *vm) {

}

// vksk_RuntimeRendererLockCameras(VK2DCameraIndex cam) - lock_cameras(_)
void vksk_RuntimeRendererLockCameras(WrenVM *vm) {

}

// vksk_RuntimeRendererUnlockCameras() - unlock_cameras()
void vksk_RuntimeRendererUnlockCameras(WrenVM *vm) {

}

// vksk_RuntimeRendererSetViewport(float x, float y, float w, float h) - set_viewport(_,_,_,_)
void vksk_RuntimeRendererSetViewport(WrenVM *vm) {

}

// vksk_RuntimeRendererGetViewport(float *x, float *y, float *w, float *h) - get_viewport()
void vksk_RuntimeRendererGetViewport(WrenVM *vm) {

}

// vksk_RuntimeRendererClear() - clear()
void vksk_RuntimeRendererClear(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawRectangle(float x, float y, float w, float h, float r, float ox, float oy) - draw_rectangle(_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawRectangle(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawRectangleOutline(float x, float y, float w, float h, float r, float ox, float oy, float lineWidth) - draw_rectangle_outline(_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawRectangleOutline(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawCircleOutline(float x, float y, float r, float lineWidth) - draw_circle_outline(_,_,_,_)
void vksk_RuntimeRendererDrawCircleOutline(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawLine(float x1, float y1, float x2, float y2) - draw_line(_,_,_,_)
void vksk_RuntimeRendererDrawLine(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawShader(VK2DShader shader, VK2DTexture tex, float x, float y, float xscale, float yscale, float rot, float originX, float originY, float xInTex, float yInTex, float texWidth, float texHeight) - draw_shader(_,_,_,_,_,_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawShader(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawPolygon(VK2DPolygon polygon, float x, float y, bool filled, float lineWidth, float xscale, float yscale, float rot, float originX, float originY) - draw_polygon(_,_,_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawPolygon(WrenVM *vm) {

}

// vksk_RuntimeRendererDrawModel(VK2DModel model, float x, float y, float z, float xscale, float yscale, float zscale, float rot, vec3 axis, float originX, float originY, float originZ) - draw_model(_,_,_,_,_,_,_,_,_,_,_,_)
void vksk_RuntimeRendererDrawModel(WrenVM *vm) {

}
