/// \file VK2DTypes.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>
#include <VK2D/stb_image.h>

#include "src/VK2DTypes.h"
#include "src/IntermediateTypes.h"
#include "src/Validation.h"
#include "src/Util.h"
#include "src/Runtime.h"

/*************** Texture ***************/
void vksk_RuntimeVK2DTextureAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign* tex = (VKSK_RuntimeForeign*)wrenSetSlotNewForeign(vm,0, 0, sizeof(VKSK_RuntimeForeign));

	int size;
	void *buffer = vksk_GetFileBuffer(wrenGetSlotString(vm, 1), &size);
	if (buffer != NULL) {
		tex->texture.tex = vk2dTextureFrom(buffer, size);
		tex->type = FOREIGN_TEXTURE;
		if (tex->texture.tex == NULL) {
			vksk_Error(false, "Failed to load texture '%s'", wrenGetSlotString(vm, 1));
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
}

void vksk_RuntimeVK2DTextureFree(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	vk2dTextureFree(tex->texture.tex);
	tex->texture.tex = NULL;
}

void vksk_RuntimeVK2DTextureWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, vk2dTextureWidth(tex->texture.tex));
}

void vksk_RuntimeVK2DTextureHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, vk2dTextureHeight(tex->texture.tex));
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
	wrenSetSlotDouble(vm, 0, vk2dTextureWidth(tex->texture.tex));
}

void vksk_RuntimeVK2DSurfaceHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, vk2dTextureHeight(tex->texture.tex));
}

/*************** Camera ***************/
void vksk_RuntimeVK2DCameraAllocate(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	memset(&cam->camera.spec, 0, sizeof(VK2DCameraSpec));
	cam->camera.spec = vk2dRendererGetCamera();
	cam->camera.index = vk2dCameraCreate(cam->camera.spec);
	cam->camera.spec.zoom = 1;
	cam->type = FOREIGN_CAMERA;
	if (cam->camera.index == VK2D_INVALID_CAMERA) {
		wrenSetSlotNull(vm, 0);
		vksk_Error(true, "Failed to create additional cameras.");
	}
}

void vksk_RuntimeVK2DCameraFinalize(void *data) {
	VKSK_RuntimeForeign *cam = data;
	vk2dCameraSetState(cam->camera.index, VK2D_CAMERA_STATE_DELETED);
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

void vksk_RuntimeVK2DCameraGetEyes(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.eyes[0]);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.eyes[1]);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.eyes[2]);
	wrenInsertInList(vm, 0, -1, 1);
}

void vksk_RuntimeVK2DCameraSetEyes(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_END)
	wrenEnsureSlots(vm, 3);
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	if (wrenGetListCount(vm, 1) >= 3) {
		wrenGetListElement(vm, 1, 0, 2);
		cam->camera.spec.Perspective.eyes[0] = wrenGetSlotDouble(vm, 2);
		wrenGetListElement(vm, 1, 1, 2);
		cam->camera.spec.Perspective.eyes[1] = wrenGetSlotDouble(vm, 2);
		wrenGetListElement(vm, 1, 2, 2);
		cam->camera.spec.Perspective.eyes[2] = wrenGetSlotDouble(vm, 2);
	}
}

void vksk_RuntimeVK2DCameraGetCentre(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.centre[0]);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.centre[1]);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.centre[2]);
	wrenInsertInList(vm, 0, -1, 1);
}

void vksk_RuntimeVK2DCameraSetCentre(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_END)
	wrenEnsureSlots(vm, 3);
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	if (wrenGetListCount(vm, 1) >= 3) {
		wrenGetListElement(vm, 1, 0, 2);
		cam->camera.spec.Perspective.centre[0] = wrenGetSlotDouble(vm, 2);
		wrenGetListElement(vm, 1, 1, 2);
		cam->camera.spec.Perspective.centre[1] = wrenGetSlotDouble(vm, 2);
		wrenGetListElement(vm, 1, 2, 2);
		cam->camera.spec.Perspective.centre[2] = wrenGetSlotDouble(vm, 2);
	}
}

void vksk_RuntimeVK2DCameraGetUp(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.up[0]);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.up[1]);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, cam->camera.spec.Perspective.up[2]);
	wrenInsertInList(vm, 0, -1, 1);
}

void vksk_RuntimeVK2DCameraSetUp(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_END)
	wrenEnsureSlots(vm, 3);
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	if (wrenGetListCount(vm, 1) >= 3) {
		wrenGetListElement(vm, 1, 0, 2);
		cam->camera.spec.Perspective.up[0] = wrenGetSlotDouble(vm, 2);
		wrenGetListElement(vm, 1, 1, 2);
		cam->camera.spec.Perspective.up[1] = wrenGetSlotDouble(vm, 2);
		wrenGetListElement(vm, 1, 2, 2);
		cam->camera.spec.Perspective.up[2] = wrenGetSlotDouble(vm, 2);
	}
}

void vksk_RuntimeVK2DCameraGetFov(WrenVM *vm) {
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, cam->camera.spec.Perspective.fov);
}

void vksk_RuntimeVK2DCameraSetFov(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 0);
	cam->camera.spec.Perspective.fov = wrenGetSlotDouble(vm, 1);
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

	// Load shaders from pak or otherwise
	void *vert, *frag;
	int vertSize, fragSize;
	vert = vksk_GetFileBuffer(wrenGetSlotString(vm, 1), &vertSize);
	frag = vksk_GetFileBuffer(wrenGetSlotString(vm, 2), &fragSize);

	shader->shader.shader = vk2dShaderFrom(
			vert,
			vertSize,
			frag,
			fragSize,
			(int)wrenGetSlotDouble(vm, 3)
	);
	shader->shader.size = (int)wrenGetSlotDouble(vm, 3);
	shader->shader.data = NULL;
	if (shader->shader.shader == NULL) {
		vksk_Error(false, "Failed to load shader %s/%s with uniform buffer size of %i.", wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), (int)wrenGetSlotDouble(vm, 3));
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeVK2DShaderFinalize(void *data) {
	VKSK_RuntimeForeign *shader = data;
	vk2dRendererWait();
	wrenReleaseHandle(vksk_GetVM(), shader->shader.data);
	vk2dShaderFree(shader->shader.shader);
}

void _vksk_RendererUpdateShaderBinding(VK2DShader shader, WrenHandle *buffer);
void vksk_RuntimeVK2DShaderSetData(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BUFFER, FOREIGN_END)
	VKSK_RuntimeForeign *shader = wrenGetSlotForeign(vm, 0);
	VKSK_RuntimeForeign *data = wrenGetSlotForeign(vm, 1);
	if (shader->shader.data != NULL)
		wrenReleaseHandle(vm, shader->shader.data);
	shader->shader.data = wrenGetSlotHandle(vm, 1);

	// In case this shader is currently bound we need to update the renderer's expected data
	_vksk_RendererUpdateShaderBinding(shader->shader.shader, shader->shader.data);

	if (data->buffer.size < shader->shader.size) {
		vksk_Error(true, "Buffer provided to shader is too small; shader expects buffer of size at least %i but was instead given a buffer of size %i.", shader->shader.size, data->buffer.size);
	}
}

void vksk_RuntimeVK2DPolygonAllocate(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *polygon = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
    polygon->type = FOREIGN_POLYGON;
}

void vksk_RuntimeVK2DPolygonCreate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_END)
    wrenEnsureSlots(vm, 4);
    wrenGetVariable(vm, "lib/Drawing", "Polygon", 0);
	VKSK_RuntimeForeign *polygon = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	polygon->type = FOREIGN_POLYGON;
	const int vertexListSize = wrenGetListCount(vm, 1);
	vec2 *vertices = malloc(sizeof(vec2) * vertexListSize);
	const int vertexSlot = 2;
	const int valueSlot = 3;

	for (int i = 0; i < vertexListSize; i++) {
		wrenGetListElement(vm, 1, i, vertexSlot);
		wrenGetListElement(vm, vertexSlot, 0, valueSlot);
		vertices[i][0] = wrenGetSlotDouble(vm, valueSlot);
		wrenGetListElement(vm, vertexSlot, 1, valueSlot);
		vertices[i][1] = wrenGetSlotDouble(vm, valueSlot);
	}

	polygon->polygon = vk2dPolygonCreate(vertices, vertexListSize);
	if (polygon->polygon == NULL) {
		wrenSetSlotNull(vm, 0);
		vksk_Error(false, "Failed to create polygon");
	}
	free(vertices);
}

void vksk_RuntimeVK2DPolygonFinalize(void *data) {
	VKSK_RuntimeForeign *f = data;
	vk2dPolygonFree(f->polygon);
}

void vksk_RuntimeVK2DPolygonFree(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	vk2dPolygonFree(f->polygon);
	f->model = NULL;
}


void vksk_RuntimeVK2DModelAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_LIST, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_END)
	VKSK_RuntimeForeign *model = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	model->type = FOREIGN_MODEL;
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 3);
	VK2DTexture target;
	if (tex->type == FOREIGN_SURFACE)
		target = tex->surface;
	else
		target = tex->texture.tex;
	// Slot 0 - Output model
	// Slot 1 - Vertex list
	// Slot 2 - Index list
	// Slot 3 - Output list from vertices
	// Slot 4 - Output value from either slot 3 list or slot 2 list
	const int vertexListSlot = 1;
	const int indexListSlot = 2;
	const int vertexSlot = 3;
	const int valueSlot = 4;
	const int vertexListSize = wrenGetListCount(vm, vertexListSlot);
	const int indexListSize = wrenGetListCount(vm, indexListSlot);
	wrenEnsureSlots(vm, 5);

	VK2DVertex3D *vertices = malloc(sizeof(VK2DVertex3D) * vertexListSize);
	uint16_t *indices = malloc(sizeof(uint16_t) * indexListSize);

	// Copy all the vertices from wren
	for (int i = 0; i < vertexListSize; i++) {
		VK2DVertex3D vertex;
		wrenGetListElement(vm, vertexListSlot, i, vertexSlot);
		wrenGetListElement(vm, vertexSlot, 0, valueSlot);
		vertex.pos[0] = wrenGetSlotDouble(vm, valueSlot);
		wrenGetListElement(vm, vertexSlot, 1, valueSlot);
		vertex.pos[1] = wrenGetSlotDouble(vm, valueSlot);
		wrenGetListElement(vm, vertexSlot, 2, valueSlot);
		vertex.pos[2] = wrenGetSlotDouble(vm, valueSlot);
		wrenGetListElement(vm, vertexSlot, 0, valueSlot);
		vertex.uv[0] = wrenGetSlotDouble(vm, valueSlot);
		wrenGetListElement(vm, vertexSlot, 1, valueSlot);
		vertex.uv[1] = wrenGetSlotDouble(vm, valueSlot);
		vertices[i] = vertex;
	}

	// Copy the indices
	for (int i = 0; i < indexListSize; i++) {
		wrenGetListElement(vm, indexListSlot, i, valueSlot);
		indices[i] = (uint16_t)wrenGetSlotDouble(vm, valueSlot);
	}

	model->model = vk2dModelCreate(vertices, vertexListSize, indices, indexListSize, target);
	if (model->model == NULL) {
		wrenSetSlotNull(vm, 0);
		vksk_Error(false, "Failed to load model");
	}

	free(vertices);
	free(indices);
}

void vksk_RuntimeVK2DModelLoad(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_END)
	wrenGetVariable(vm, "lib/Drawing", "Model", 0);
	VKSK_RuntimeForeign *model = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	const char *fname = wrenGetSlotString(vm, 1);
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 2);
	VK2DTexture target;
	if (tex->type == FOREIGN_SURFACE)
		target = tex->surface;
	else
		target = tex->texture.tex;

	int size;
	void *buffer = vksk_GetFileBuffer(fname, &size);
	model->type = FOREIGN_MODEL;
	model->model = vk2dModelFrom(buffer, size, target);

	if (model->model == NULL) {
		wrenSetSlotNull(vm, 0);
		vksk_Error(false, "Failed to load model \"%s\"", fname);
	}
}

void vksk_RuntimeVK2DModelFinalize(void *data) {
	VKSK_RuntimeForeign *f = data;
	vk2dModelFree(f->model);
}

void vksk_RuntimeVK2DModelFree(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	vk2dModelFree(f->model);
	f->model = NULL;
}
