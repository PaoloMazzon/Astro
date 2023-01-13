/// \file VK2DTypes.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>

#include "src/VK2DTypes.h"

/*************** Texture ***************/
void vksk_RuntimeVK2DTextureAllocate(WrenVM *vm) {
	VK2DTexture* tex = (VK2DTexture*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VK2DTexture));
	*tex = vk2dTextureLoad(wrenGetSlotString(vm, 1));
}

void vksk_RuntimeVK2DTextureFinalize(void *data) {
	vk2dRendererWait();
	vk2dTextureFree(*(VK2DTexture*)data);
}

void vksk_RuntimeVK2DTextureWidth(WrenVM *vm) {
	VK2DTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*tex)->img->width);
}

void vksk_RuntimeVK2DTextureHeight(WrenVM *vm) {
	VK2DTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*tex)->img->height);
}

/*************** Surface ***************/
void vksk_RuntimeVK2DSurfaceAllocate(WrenVM *vm) {
	VK2DTexture* tex = (VK2DTexture*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VK2DTexture));
	*tex = vk2dTextureCreate(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void vksk_RuntimeVK2DSurfaceFinalize(void *data) {
	vk2dRendererWait();
	vk2dTextureFree(*(VK2DTexture*)data);
}

void vksk_RuntimeVK2DSurfaceWidth(WrenVM *vm) {
	VK2DTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*tex)->img->width);
}

void vksk_RuntimeVK2DSurfaceHeight(WrenVM *vm) {
	VK2DTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*tex)->img->height);
}

/*************** Camera ***************/
// Just for the internal wren camera
typedef struct _vksk_RuntimeCamera {
	VK2DCameraIndex index;
	VK2DCameraSpec spec;
} _vksk_RuntimeCamera;


void vksk_RuntimeVK2DCameraAllocate(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenSetSlotNewForeign(vm, 0, 0, sizeof(_vksk_RuntimeCamera));
	memset(&cam->spec, 0, sizeof(VK2DCameraSpec));
	cam->spec = vk2dRendererGetCamera();
	cam->index = vk2dCameraCreate(cam->spec);
}

void vksk_RuntimeVK2DCameraFinalize(void *data) {

}

void vksk_RuntimeVK2DCameraGetType(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.type);
}

void vksk_RuntimeVK2DCameraSetType(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.type = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetX(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.x);
}

void vksk_RuntimeVK2DCameraSetX(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.x = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetY(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.y);
}

void vksk_RuntimeVK2DCameraSetY(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.y = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetW(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.w);
}

void vksk_RuntimeVK2DCameraSetW(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.w = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetH(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.h);
}

void vksk_RuntimeVK2DCameraSetH(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.h = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetZoom(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.zoom);
}

void vksk_RuntimeVK2DCameraSetZoom(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.zoom = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetRotation(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.rot);
}

void vksk_RuntimeVK2DCameraSetRotation(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.rot = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetXOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.xOnScreen);
}

void vksk_RuntimeVK2DCameraSetXOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.xOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetYOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.yOnScreen);
}

void vksk_RuntimeVK2DCameraSetYOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.yOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetWOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.wOnScreen);
}

void vksk_RuntimeVK2DCameraSetWOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.wOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetHOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->spec.hOnScreen);
}

void vksk_RuntimeVK2DCameraSetHOnScreen(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.hOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetEyes(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	// TODO: This
}

void vksk_RuntimeVK2DCameraSetEyes(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	// TODO: This
}

void vksk_RuntimeVK2DCameraGetCentre(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	// TODO: This
}

void vksk_RuntimeVK2DCameraSetCentre(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	// TODO: This
}

void vksk_RuntimeVK2DCameraGetUp(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	// TODO: This
}

void vksk_RuntimeVK2DCameraSetUp(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	// TODO: This
}

void vksk_RuntimeVK2DCameraGetFov(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.Perspective.fov = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraSetFov(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	cam->spec.Perspective.fov = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraUpdate(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	vk2dCameraUpdate(cam->index, cam->spec);
}