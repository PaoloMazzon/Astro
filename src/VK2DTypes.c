/// \file VK2DTypes.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>
#include <VK2D/stb_image.h>

#include "src/VK2DTypes.h"
#include "src/IntermediateTypes.h"
#include "src/Validation.h"
#include "src/Util.h"

/*************** Texture ***************/
void vksk_RuntimeVK2DTextureAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign* tex = (VKSK_RuntimeForeign*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VKSK_RuntimeForeign));

	int x, y, channels, size;
	void *pixels;
	void *buffer = vksk_GetFileBuffer(wrenGetSlotString(vm, 1), &size);
	if (buffer != NULL) {
		pixels = stbi_load_from_memory(buffer, size, &x, &y, &channels, 4);
		if (pixels != NULL) {
			tex->texture.img = vk2dImageFromPixels(vk2dRendererGetDevice(), pixels, x, y);
			stbi_image_free(pixels);

			if (tex->texture.img == NULL) {
				vksk_Error(false, "Failed to load texture image '%s'", wrenGetSlotString(vm, 1));
				wrenSetSlotNull(vm, 0);
			} else {
				tex->texture.tex = vk2dTextureLoadFromImage(tex->texture.img);
				tex->type = FOREIGN_TEXTURE;
				if (tex->texture.tex == NULL) {
					vk2dImageFree(tex->texture.img);
					vksk_Error(false, "Failed to load texture '%s'", wrenGetSlotString(vm, 1));
					wrenSetSlotNull(vm, 0);
				}
			}
		} else {
			vksk_Error(false, "Failed to load texture pixels '%s'", wrenGetSlotString(vm, 1));
			wrenSetSlotNull(vm, 0);
		}
		free(buffer);
	} else {
		vksk_Error(false, "Failed to load texture buffer '%s'", wrenGetSlotString(vm, 1));
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeVK2DTextureFinalize(void *data) {
	vk2dRendererWait();
	vk2dTextureFree(((VKSK_RuntimeForeign*)data)->texture.tex);
	vk2dImageFree(((VKSK_RuntimeForeign*)data)->texture.img);
}

void vksk_RuntimeVK2DTextureFree(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	vk2dTextureFree(tex->texture.tex);
	vk2dImageFree(tex->texture.img);
	tex->texture.tex = NULL;
	tex->texture.img = NULL;
}

void vksk_RuntimeVK2DTextureWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->texture.img->width);
}

void vksk_RuntimeVK2DTextureHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, tex->texture.img->height);
}

/*************** Surface ***************/
void vksk_RuntimeVK2DSurfaceAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign* tex = (VKSK_RuntimeForeign*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VKSK_RuntimeForeign));
	tex->surface = vk2dTextureCreate(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
	tex->type = FOREIGN_SURFACE;
	if (tex->surface == NULL) {
		vksk_Error(false, "Failed to create surface of size %fx%f", wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
		wrenSetSlotNull(vm, 0);
	}
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
	if (cam->camera.index == VK2D_INVALID_CAMERA) {
		wrenSetSlotNull(vm, 0);
		vksk_Error(false, "Failed to create additional cameras.");
	}
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
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.type = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetX(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.x);
}

void vksk_RuntimeVK2DCameraSetX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.x = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetY(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.y);
}

void vksk_RuntimeVK2DCameraSetY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.y = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetW(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.w);
}

void vksk_RuntimeVK2DCameraSetW(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.w = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetH(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.h);
}

void vksk_RuntimeVK2DCameraSetH(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.h = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetZoom(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.zoom);
}

void vksk_RuntimeVK2DCameraSetZoom(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.zoom = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetRotation(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.rot);
}

void vksk_RuntimeVK2DCameraSetRotation(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.rot = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetXOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.xOnScreen);
}

void vksk_RuntimeVK2DCameraSetXOnScreen(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.xOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetYOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.yOnScreen);
}

void vksk_RuntimeVK2DCameraSetYOnScreen(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.yOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetWOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.wOnScreen);
}

void vksk_RuntimeVK2DCameraSetWOnScreen(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.wOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraGetHOnScreen(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.hOnScreen);
}

void vksk_RuntimeVK2DCameraSetHOnScreen(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.hOnScreen = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeVK2DCameraUpdate(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	vk2dCameraUpdate(cam->camera.index, cam->camera.spec);
}

/*************** Shader ***************/

void vksk_RuntimeVK2DShaderAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *shader = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	shader->type = FOREIGN_SHADER;
	shader->shader = vk2dShaderLoad(
			wrenGetSlotString(vm, 1),
			wrenGetSlotString(vm, 2),
			(int)wrenGetSlotDouble(vm, 3)
	);
	if (shader->shader == NULL) {
		vksk_Error(false, "Failed to load shader %s/%s with uniform buffer size of %i.", wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), (int)wrenGetSlotDouble(vm, 3));
	}
}

void vksk_RuntimeVK2DShaderFinalize(void *data) {
	VKSK_RuntimeForeign *shader = data;
	vk2dRendererWait();
	vk2dShaderFree(shader->shader);
}

void vksk_RuntimeVK2DShaderSetData(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BUFFER, FOREIGN_END)
	VKSK_RuntimeForeign *shader = wrenGetSlotForeign(vm, 0);
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 1);
	vk2dShaderUpdate(shader->shader, buffer->buffer.data);
}
