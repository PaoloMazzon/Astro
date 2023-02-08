/// \file VK2DTypes.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>

#include "src/VK2DTypes.h"
#include "src/IntermediateTypes.h"

/*************** Texture ***************/
void vksk_RuntimeVK2DTextureAllocate(WrenVM *vm) {
	VKSK_RuntimeForeign* tex = (VKSK_RuntimeForeign*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VKSK_RuntimeForeign));
	tex->texture = vk2dTextureLoad(wrenGetSlotString(vm, 1));
	tex->type = FOREIGN_TEXTURE;
}

void vksk_RuntimeVK2DTextureFinalize(void *data) {
	vk2dRendererWait();
	vk2dTextureFree(((VKSK_RuntimeForeign*)data)->texture);
}

void vksk_RuntimeVK2DTextureFree(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	vk2dTextureFree(tex->texture);
	tex->texture = NULL;
}

void vksk_RuntimeVK2DTextureWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->texture->img->width);
}

void vksk_RuntimeVK2DTextureHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->texture->img->height);
}

/*************** Surface ***************/
void vksk_RuntimeVK2DSurfaceAllocate(WrenVM *vm) {
	VKSK_RuntimeForeign* tex = (VKSK_RuntimeForeign*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VKSK_RuntimeForeign));
	tex->surface = vk2dTextureCreate(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
	tex->type = FOREIGN_SURFACE;
}

void vksk_RuntimeVK2DSurfaceFinalize(void *data) {
	vk2dRendererWait();
	vk2dTextureFree(((VKSK_RuntimeForeign *) data)->surface);
}

void vksk_RuntimeVK2DSurfaceFree(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	vk2dTextureFree(tex->surface);
	tex->surface = NULL;
}

void vksk_RuntimeVK2DSurfaceWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->surface->img->width);
}

void vksk_RuntimeVK2DSurfaceHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->surface->img->height);
}

/*************** Camera ***************/
void vksk_RuntimeVK2DCameraAllocate(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	memset(&cam->camera.spec, 0, sizeof(VK2DCameraSpec));
	cam->camera.spec = vk2dRendererGetCamera();
	cam->camera.index = vk2dCameraCreate(cam->camera.spec);
	cam->type = FOREIGN_CAMERA;
}

void vksk_RuntimeVK2DCameraFinalize(void *data) {
	VKSK_RuntimeForeign *cam = data;
	vk2dCameraSetState(cam->camera.index, cs_Deleted);
}

void vksk_RuntimeVK2DCameraGetType(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.type);
}

void vksk_RuntimeVK2DCameraSetType(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.type = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetX(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.x);
}

void vksk_RuntimeVK2DCameraSetX(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.x = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetY(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.y);
}

void vksk_RuntimeVK2DCameraSetY(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.y = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetW(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.w);
}

void vksk_RuntimeVK2DCameraSetW(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.w = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetH(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.h);
}

void vksk_RuntimeVK2DCameraSetH(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.h = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetZoom(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.zoom);
}

void vksk_RuntimeVK2DCameraSetZoom(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.zoom = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetRotation(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.rot);
}

void vksk_RuntimeVK2DCameraSetRotation(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.rot = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetXOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.xOnScreen);
}

void vksk_RuntimeVK2DCameraSetXOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.xOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetYOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.yOnScreen);
}

void vksk_RuntimeVK2DCameraSetYOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.yOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetWOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.wOnScreen);
}

void vksk_RuntimeVK2DCameraSetWOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.wOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetHOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.hOnScreen);
}

void vksk_RuntimeVK2DCameraSetHOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.hOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraUpdate(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	vk2dCameraUpdate(cam->camera.index, cam->camera.spec);
}