/// \file VK2DTypes.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>

#include "src/VK2DTypes.h"
#include "src/IntermediateTypes.h"

/*************** Texture ***************/
void vksk_RuntimeVK2DTextureAllocate(WrenVM *vm) {
	_vksk_RuntimeTexture* tex = (_vksk_RuntimeTexture*)wrenSetSlotNewForeign(vm,0, 0, sizeof(_vksk_RuntimeTexture));
	tex->tex = vk2dTextureLoad(wrenGetSlotString(vm, 1));
	tex->freed = false;
}

void vksk_RuntimeVK2DTextureFinalize(void *data) {
	if (((_vksk_RuntimeTexture*)data)->freed == false) {
		vk2dRendererWait();
		vk2dTextureFree(((_vksk_RuntimeTexture *) data)->tex);
	}
}

void vksk_RuntimeVK2DTextureFree(WrenVM *vm) {
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 0);
	if (tex->freed == false) {
		vk2dRendererWait();
		vk2dTextureFree(tex->tex);
		tex->freed = true;
	}
}

void vksk_RuntimeVK2DTextureWidth(WrenVM *vm) {
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->tex->img->width);
}

void vksk_RuntimeVK2DTextureHeight(WrenVM *vm) {
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->tex->img->height);
}

/*************** Surface ***************/
void vksk_RuntimeVK2DSurfaceAllocate(WrenVM *vm) {
	_vksk_RuntimeTexture* tex = (_vksk_RuntimeTexture*)wrenSetSlotNewForeign(vm,0, 0, sizeof(_vksk_RuntimeTexture));
	tex->tex = vk2dTextureCreate(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
	tex->freed = false;
}

void vksk_RuntimeVK2DSurfaceFinalize(void *data) {
	if (((_vksk_RuntimeTexture*)data)->freed == false) {
		vk2dRendererWait();
		vk2dTextureFree(((_vksk_RuntimeTexture *) data)->tex);
	}
}

void vksk_RuntimeVK2DSurfaceFree(WrenVM *vm) {
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 0);
	if (tex->freed == false) {
		vk2dRendererWait();
		vk2dTextureFree(tex->tex);
		tex->freed = true;
	}
}

void vksk_RuntimeVK2DSurfaceWidth(WrenVM *vm) {
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->tex->img->width);
}

void vksk_RuntimeVK2DSurfaceHeight(WrenVM *vm) {
	_vksk_RuntimeTexture *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->tex->img->height);
}

/*************** Camera ***************/
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
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < 3; i++) {
		wrenSetSlotDouble(vm, 1, cam->spec.Perspective.eyes[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void vksk_RuntimeVK2DCameraSetEyes(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	vec3 vec;
	wrenEnsureSlots(vm, 3);
	for (int i = 0; i < 3; i++) {
		wrenGetListElement(vm, 1, i, 2);
		vec[i] = wrenGetSlotDouble(vm, 2);
	}
	memcpy(cam->spec.Perspective.eyes, vec, sizeof(vec3));
}

void vksk_RuntimeVK2DCameraGetCentre(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < 3; i++) {
		wrenSetSlotDouble(vm, 1, cam->spec.Perspective.centre[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void vksk_RuntimeVK2DCameraSetCentre(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	vec3 vec;
	wrenEnsureSlots(vm, 3);
	for (int i = 0; i < 3; i++) {
		wrenGetListElement(vm, 1, i, 2);
		vec[i] = wrenGetSlotDouble(vm, 2);
	}
	memcpy(cam->spec.Perspective.centre, vec, sizeof(vec3));
}

void vksk_RuntimeVK2DCameraGetUp(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < 3; i++) {
		wrenSetSlotDouble(vm, 1, cam->spec.Perspective.up[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void vksk_RuntimeVK2DCameraSetUp(WrenVM *vm) {
	_vksk_RuntimeCamera *cam = wrenGetSlotForeign(vm, 0);
	vec3 vec;
	wrenEnsureSlots(vm, 3);
	for (int i = 0; i < 3; i++) {
		wrenGetListElement(vm, 1, i, 2);
		vec[i] = wrenGetSlotDouble(vm, 2);
	}
	memcpy(cam->spec.Perspective.up, vec, sizeof(vec3));
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