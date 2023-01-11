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

