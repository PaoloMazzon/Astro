/// \file InternalBindings.c
/// \author Paolo Mazzon
#include <stdio.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define CUTE_TILED_IMPLEMENTATION
#include "src/cute_tiled.h"
#include "src/InternalBindings.h"
#include "src/ConfigFile.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"
#include "src/Runtime.h"
#include "src/Util.h"

static const double NO_MORE_LAYERS = 0;
static const double TILE_LAYER = 1;
static const double OBJECT_LAYER = 2;

extern uint32_t rmask, gmask, bmask, amask;

typedef struct _VKSK_GamepadInputs {
	bool inputs[15];
} _VKSK_GamepadInputs;

// Globals
static SDL_GameController *gControllers[4] = {0};
static _VKSK_GamepadInputs gInputPrevious[4] = {0};
static _VKSK_GamepadInputs gInput[4] = {0};
static double gAxisDeadzone = 0.1;
static double gTriggerDeadzone = 0.1;

void vksk_RuntimeINIAllocate(WrenVM *vm) {
	VKSK_RuntimeForeign *conf = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	conf->ini = vksk_ConfigLoad(wrenGetSlotString(vm, 1));
	conf->type = FOREIGN_INI;
}

void vksk_RuntimeINIFinalize(void *data) {
	VKSK_RuntimeForeign *f = data;
	vksk_ConfigFree(f->ini);
}

void vksk_RuntimeINIFlush(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigFlush(conf->ini, wrenGetSlotString(vm, 1));
}

void vksk_RuntimeINIKeyExists(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotBool(vm, 0, vksk_ConfigKeyExists(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2)));
}

void vksk_RuntimeINIGetString(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotString(vm, 0, vksk_ConfigGetString(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotString(vm, 3)));
}

void vksk_RuntimeINISetString(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigSetString(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotString(vm, 3));
}

void vksk_RuntimeINIGetNum(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, vksk_ConfigGetDouble(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotDouble(vm, 3)));
}

void vksk_RuntimeINISetNum(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigSetDouble(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vksk_RuntimeINIGetBool(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_BOOL, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotBool(vm, 0, vksk_ConfigGetBool(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotBool(vm, 3)));
}

void vksk_RuntimeINISetBool(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_BOOL, FOREIGN_END)
	VKSK_RuntimeForeign *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigSetBool(conf->ini, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotBool(vm, 3));
}

const char* loadFile(const char *filename); // from VMConfig.c
bool _vk2dFileExists(const char *filename);

void vksk_RuntimeFileRead(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *fname = wrenGetSlotString(vm, 1);
	if (_vk2dFileExists(fname)) {
		const char *str = loadFile(fname);
		wrenSetSlotString(vm, 0, str);
		free((void*)str);
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeFileReadFromPak(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *fname = wrenGetSlotString(vm, 1);
	if (!vksk_PakFileExists(gGamePak, fname)) {
		wrenSetSlotNull(vm, 0);
	} else {
		int size;
		char *out = (void*)vksk_PakGetFile(gGamePak, fname, &size);
		out = realloc(out, size + 1);
		out[size] = 0;
		wrenSetSlotString(vm, 0, out);
		free(out);
	}
}

void vksk_RuntimeFileWrite(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING | FOREIGN_BUFFER, FOREIGN_END)
	if (wrenGetSlotType(vm, 2) == WREN_TYPE_STRING) {
		const char *fname = wrenGetSlotString(vm, 1);
		const char *val = wrenGetSlotString(vm, 2);
		FILE *f = fopen(fname, "w");
		fprintf(f, "%s", val);
		fclose(f);
	} else {
		const char *fname = wrenGetSlotString(vm, 1);
		VKSK_RuntimeForeign *val = wrenGetSlotForeign(vm, 2);
		FILE *f = fopen(fname, "w");
		fwrite(val->buffer.data, 1, val->buffer.size, f);
		fclose(f);
	}
}

void vksk_RuntimeFileExists(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *fname = wrenGetSlotString(vm, 1);
	wrenSetSlotBool(vm, 0, _vk2dFileExists(fname));
}

void vksk_RuntimeFileExistsInPak(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *fname = wrenGetSlotString(vm, 1);
	wrenSetSlotBool(vm, 0, vksk_PakFileExists(gGamePak, fname));
}

void vksk_RuntimeFileRemove(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *fname = wrenGetSlotString(vm, 1);
	remove(fname);
}

void vksk_RuntimeFileRename(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_END)
	const char *fname = wrenGetSlotString(vm, 1);
	const char *newfname = wrenGetSlotString(vm, 2);
	rename(fname, newfname);
}

void vksk_RuntimeTiledAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *tiled = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	int size;
	void *buffer = vksk_GetFileBuffer(wrenGetSlotString(vm, 1), &size);
	if (buffer != NULL) {
		tiled->tiled.map = cute_tiled_load_map_from_memory(buffer, size, NULL);
		tiled->tiled.layer = NULL;
		tiled->type = FOREIGN_TILED_MAP;
		fflush(stdout);
		fflush(stderr);
		if (tiled->tiled.map == NULL) {
			vksk_Error(false, "Failed to load Tiled map '%s'", wrenGetSlotString(vm, 1));
			wrenSetSlotNull(vm, 0);
		}
		free(buffer);
	} else {
		vksk_Error(false, "Failed to load Tiled map '%s'", wrenGetSlotString(vm, 1));
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeTiledFinalize(void *data) {
	VKSK_RuntimeForeign *f = data;
	cute_tiled_free_map(f->tiled.map);
}

void vksk_RuntimeTiledWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (double)f->tiled.map->width);
}

void vksk_RuntimeTiledHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (double)f->tiled.map->height);
}

void vksk_RuntimeTiledCellWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (double)f->tiled.map->tilewidth);
}

void vksk_RuntimeTiledCellHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (double)f->tiled.map->tileheight);
}

void vksk_RuntimeTiledLayerName(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	if (f->tiled.layer != NULL)
		wrenSetSlotString(vm, 0, f->tiled.layer->name.ptr);
	else
		wrenSetSlotString(vm, 0, "");
}

void vksk_RuntimeTiledNextLayer(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	if (f->tiled.layer == NULL) {
		f->tiled.layer = f->tiled.map->layers;
	} else {
		f->tiled.layer = f->tiled.layer->next;
	}

	// Iterate through the layers until we find a tile layer, object layer, or run out
	// (basically just skip unsupported layers)
	bool foundLayer = false;
	while (!foundLayer) {
		if (f->tiled.layer == NULL) {
			foundLayer = true;
			wrenSetSlotDouble(vm, 0, NO_MORE_LAYERS);
		} else {
			if (strcmp(f->tiled.layer->type.ptr, "tilelayer") == 0) {
				wrenSetSlotDouble(vm, 0, TILE_LAYER);
				foundLayer = true;
			} else if (strcmp(f->tiled.layer->type.ptr, "objectgroup") == 0) {
				wrenSetSlotDouble(vm, 0, OBJECT_LAYER);
				foundLayer = true;
			} else {
				f->tiled.layer = f->tiled.layer->next;
			}
		}
	}
}

void vksk_RuntimeTiledGetObjects(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	struct cute_tiled_object_t *current = f->tiled.layer->objects;
	const int listSlot = 0;
	const int mapSlot = 1;
	const int keySlot = 2;
	const int valSlot = 3;
	const int val2Slot = 4;
	wrenEnsureSlots(vm, 5);
	wrenSetSlotNewList(vm, listSlot);

	while (current != NULL) {
		wrenSetSlotNewMap(vm, mapSlot);

		// Load x/y
		wrenSetSlotString(vm, keySlot, "x");
		wrenSetSlotDouble(vm, valSlot, current->x);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "y");
		wrenSetSlotDouble(vm, valSlot, current->y);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);

		// Type
		wrenSetSlotString(vm, keySlot, "class");
		wrenSetSlotString(vm, valSlot, current->type.ptr);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);

		// Various niche stuff
		wrenSetSlotString(vm, keySlot, "width");
		wrenSetSlotDouble(vm, valSlot, current->width);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "height");
		wrenSetSlotDouble(vm, valSlot, current->height);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "gid");
		wrenSetSlotDouble(vm, valSlot, current->gid);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "point");
		wrenSetSlotBool(vm, valSlot, current->point);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "rotation");
		wrenSetSlotDouble(vm, valSlot, current->rotation);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "id");
		wrenSetSlotDouble(vm, valSlot, current->id);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "visible");
		wrenSetSlotBool(vm, valSlot, current->visible);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);


		// Load properties
		wrenSetSlotNewMap(vm, valSlot);
		for (int i = 0; i < current->property_count; i++) {
			wrenSetSlotString(vm, keySlot, current->properties[i].name.ptr);

			if (current->properties[i].type == CUTE_TILED_PROPERTY_STRING)
				wrenSetSlotString(vm, val2Slot, current->properties[i].data.string.ptr);
			else if (current->properties[i].type == CUTE_TILED_PROPERTY_BOOL)
				wrenSetSlotBool(vm, val2Slot, current->properties[i].data.boolean);
			else if (current->properties[i].type == CUTE_TILED_PROPERTY_COLOR)
				wrenSetSlotDouble(vm, val2Slot, (double)current->properties[i].data.color);
			else if (current->properties[i].type == CUTE_TILED_PROPERTY_FILE)
				wrenSetSlotString(vm, val2Slot, current->properties[i].data.file.ptr);
			else if (current->properties[i].type == CUTE_TILED_PROPERTY_FLOAT)
				wrenSetSlotDouble(vm, val2Slot, current->properties[i].data.floating);
			else if (current->properties[i].type == CUTE_TILED_PROPERTY_INT)
				wrenSetSlotDouble(vm, val2Slot, (double)current->properties[i].data.integer);

			wrenSetMapValue(vm, valSlot, keySlot, val2Slot);
		}
		wrenSetSlotString(vm, keySlot, "properties");
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);

		wrenInsertInList(vm, listSlot, -1, mapSlot);
		current = current->next;
	}
}

void vksk_RuntimeTiledGetTiles(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);

	wrenEnsureSlots(vm, 3);
	const int yList = 0;
	const int xList = 1;
	const int valSlot = 2;
	wrenSetSlotNewList(vm, yList);

	for (int y = 0; y < f->tiled.layer->height; y++) {
		wrenSetSlotNewList(vm, xList);

		for (int x = 0; x < f->tiled.layer->width; x++) {
			wrenSetSlotDouble(vm, valSlot, f->tiled.layer->data[(y * f->tiled.layer->width) + x]);
			wrenInsertInList(vm, xList, -1, valSlot);
		}

		wrenInsertInList(vm, yList, -1, xList);
	}
}

void vksk_RuntimeTiledGetTilesets(WrenVM *vm) {
	VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, 0);
	cute_tiled_tileset_t *current = f->tiled.map->tilesets;
	const int listSlot = 0;
	const int mapSlot = 1;
	const int keySlot = 2;
	const int valSlot = 3;
	wrenEnsureSlots(vm, 4);
	wrenSetSlotNewList(vm, listSlot);

	while (current != NULL) {
		wrenSetSlotNewMap(vm, mapSlot);

		wrenSetSlotString(vm, keySlot, "gid");
		wrenSetSlotDouble(vm, valSlot, (double)current->firstgid);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);
		wrenSetSlotString(vm, keySlot, "filename");
		wrenSetSlotString(vm, valSlot, current->image.ptr);
		wrenSetMapValue(vm, mapSlot, keySlot, valSlot);

		wrenInsertInList(vm, listSlot, -1, mapSlot);
		current = current->next;
	}
}

void vksk_RuntimeBufferAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	buffer->buffer.pointer = 0;
	buffer->buffer.size = wrenGetSlotDouble(vm, 1);
	buffer->buffer.data = malloc(buffer->buffer.size);
	if (buffer->buffer.data == NULL) {
		vksk_Log("Could not allocate buffer of size %f\n", buffer->buffer.size);
	}
	buffer->type = FOREIGN_BUFFER;
}

void vksk_RuntimeBufferFinalize(void *data) {
	VKSK_RuntimeForeign *buffer = data;
	free(buffer->buffer.data);
}

unsigned char* loadFileRaw(const char *filename, int *size) {
	FILE* file = fopen(filename, "rb");
	unsigned char *buffer = NULL;
	*size = 0;

	if (file != NULL) {
		// Find file size
		fseek(file, 0, SEEK_END);
		*size = ftell(file);
		rewind(file);

		buffer = malloc(*size);

		if (buffer != NULL) {
			// Fill the buffer
			fread(buffer, 1, *size, file);
		}
		fclose(file);
	}

	return (unsigned char*)buffer;
}

void vksk_RuntimeBufferOpen(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	wrenGetVariable(vm, "lib/Util", "Buffer", 0);
	VKSK_RuntimeForeign *buffer = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	buffer->buffer.pointer = 0;
	buffer->buffer.data = loadFileRaw(wrenGetSlotString(vm, 1), &buffer->buffer.size);
	if (buffer->buffer.data == NULL) {
		vksk_Log("Could not open buffer from \"%s\"\n", wrenGetSlotString(vm, 1));
	}
	buffer->type = FOREIGN_BUFFER;
}

void vksk_RuntimeBufferOpenFromPak(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	wrenGetVariable(vm, "lib/Util", "Buffer", 0);
	VKSK_RuntimeForeign *buffer = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	buffer->type = FOREIGN_BUFFER;
	buffer->buffer.pointer = 0;
	buffer->buffer.data = 0;
	buffer->buffer.size = 0;
	const char *fname = wrenGetSlotString(vm, 1);

	if (vksk_PakFileExists(gGamePak, fname)) {
		buffer->buffer.data = (void*)vksk_PakGetFile(gGamePak, fname, &buffer->buffer.size);
	}
}

void vksk_RuntimeBufferResize(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	buffer->buffer.data = realloc(buffer->buffer.data, (size_t)wrenGetSlotDouble(vm, 1));
	if (buffer->buffer.data == NULL)
		vksk_Log("Could not resize buffer\n");
	else
		buffer->buffer.size = (int)wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeBufferSize(WrenVM *vm) {
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, buffer->buffer.size);
}

void vksk_RuntimeBufferSetPointer(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	int pointer = (int)wrenGetSlotDouble(vm, 1);
	if (pointer > 0 && pointer <= buffer->buffer.size)
		buffer->buffer.pointer = pointer;
}

void vksk_RuntimeBufferGetPointer(WrenVM *vm) {
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, buffer->buffer.pointer);
}

void vksk_RuntimeBufferReadDouble(WrenVM *vm) { // 8 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 8) {
		double *v = (double*)(&buffer->buffer.data[buffer->buffer.pointer]);
		if (isnan(*v))
			wrenSetSlotDouble(vm, 0, 0);
		else
			wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 8;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteDouble(WrenVM *vm) { // 8 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 8) {
		double *v = (double*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (double)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 8;
	}
}

void vksk_RuntimeBufferReadFloat(WrenVM *vm) { // 4 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 4) {
		float *v = (float*)(&buffer->buffer.data[buffer->buffer.pointer]);
		if (isnan(*v))
			wrenSetSlotDouble(vm, 0, 0);
		else
			wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 4;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteFloat(WrenVM *vm) { // 4 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 4) {
		float *v = (float*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (float)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 4;
	}
}

void vksk_RuntimeBufferReadUint64(WrenVM *vm) { // 8 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 8) {
		uint64_t *v = (uint64_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 8;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteUint64(WrenVM *vm) { // 8 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 8) {
		uint64_t *v = (uint64_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (uint64_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 8;
	}
}

void vksk_RuntimeBufferReadUint32(WrenVM *vm) { // 4 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 4) {
		uint32_t *v = (uint32_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 4;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteUint32(WrenVM *vm) { // 4 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 4) {
		uint32_t *v = (uint32_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (uint32_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 4;
	}
}

void vksk_RuntimeBufferReadUint16(WrenVM *vm) { // 2 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 2) {
		uint16_t *v = (uint16_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 2;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteUint16(WrenVM *vm) { // 2 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 2) {
		uint16_t *v = (uint16_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (uint16_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 2;
	}
}

void vksk_RuntimeBufferReadUint8(WrenVM *vm) { // 1 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 1) {
		uint8_t *v = (uint8_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 1;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteUint8(WrenVM *vm) { // 1 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 1) {
		uint8_t *v = (uint8_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (uint8_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 1;
	}
}

void vksk_RuntimeBufferReadInt64(WrenVM *vm) { // 8 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 8) {
		int64_t *v = (int64_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 8;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteInt64(WrenVM *vm) { // 8 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 8) {
		int64_t *v = (int64_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (int64_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 8;
	}
}

void vksk_RuntimeBufferReadInt32(WrenVM *vm) { // 4 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 4) {
		int32_t *v = (int32_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 4;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteInt32(WrenVM *vm) { // 4 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 4) {
		int32_t *v = (int32_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (int32_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 4;
	}
}

void vksk_RuntimeBufferReadInt16(WrenVM *vm) { // 2 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 2) {
		int16_t *v = (int16_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 2;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteInt16(WrenVM *vm) { // 2 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 2) {
		int16_t *v = (int16_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (int16_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 2;
	}
}

void vksk_RuntimeBufferReadInt8(WrenVM *vm) { // 1 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 1) {
		int8_t *v = (int8_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotDouble(vm, 0, *v);
		buffer->buffer.pointer += 1;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteInt8(WrenVM *vm) { // 1 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 1) {
		int8_t *v = (int8_t*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (int8_t)wrenGetSlotDouble(vm, 1);
		buffer->buffer.pointer += 1;
	}
}

void vksk_RuntimeBufferReadString(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	int size = wrenGetSlotDouble(vm, 1);
	if (buffer->buffer.pointer <= buffer->buffer.size - size) {
		char *str = malloc(size + 1);
		memcpy(str, &buffer->buffer.data[buffer->buffer.pointer], size);
		str[size] = 0;
		wrenSetSlotString(vm, 0, str);
		free(str);
		buffer->buffer.pointer += size;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteString(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	int size = strlen(wrenGetSlotString(vm, 1));
	if (buffer->buffer.pointer <= buffer->buffer.size - size) {
		memcpy(&buffer->buffer.data[buffer->buffer.pointer], wrenGetSlotString(vm, 1), size);
		buffer->buffer.pointer += size;
	}
}

void vksk_RuntimeBufferReadBool(WrenVM *vm) { // 1 bytes
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 1) {
		bool *v = (bool*)(&buffer->buffer.data[buffer->buffer.pointer]);
		wrenSetSlotBool(vm, 0, *v);
		buffer->buffer.pointer += 1;
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeBufferWriteBool(WrenVM *vm) { // 1 bytes
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL, FOREIGN_END)
	VKSK_RuntimeForeign *buffer = wrenGetSlotForeign(vm, 0);
	if (buffer->buffer.pointer <= buffer->buffer.size - 1) {
		bool *v = (bool*)(&buffer->buffer.data[buffer->buffer.pointer]);
		*v = (bool)wrenGetSlotBool(vm, 1);
		buffer->buffer.pointer += 1;
	}
}

void _vksk_RuntimeControllerRefresh() {
	for (int i = 0; i < 4; i++) {
		if (gControllers[i] != NULL) {
			SDL_GameControllerClose(gControllers[i]);
			gControllers[i] = NULL;
		}
	}

	int gamepad = 0;
	for (int i = 0; i < SDL_NumJoysticks() && gamepad < 4; i++) {
		if (SDL_IsGameController(i)) {
			SDL_GameController *controller = SDL_GameControllerOpen(i);
			if (controller == NULL) {
				vksk_Log("Failed to open controller with joy index %i, SDL error: %s", i, SDL_GetError());
			} else {
				gControllers[gamepad] = controller;
				gamepad++;
			}
		}
	}
}

void _vksk_RuntimeControllersUpdate() {
	for (int i = 0; i < 4; i++) {
		if (gControllers[i] != NULL) {
			_VKSK_GamepadInputs input = {0};
			input.inputs[0] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_A);
			input.inputs[1] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_B);
			input.inputs[2] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_X);
			input.inputs[3] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_Y);
			input.inputs[4] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_BACK);
			input.inputs[5] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_GUIDE);
			input.inputs[6] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_START);
			input.inputs[7] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_LEFTSTICK);
			input.inputs[8] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_RIGHTSTICK);
			input.inputs[9] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			input.inputs[10] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			input.inputs[11] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_DPAD_UP);
			input.inputs[12] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			input.inputs[13] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			input.inputs[14] = SDL_GameControllerGetButton(gControllers[i], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

			gInputPrevious[i] = gInput[i];
			gInput[i] = input;
		} else {
			memset(&gInput[i], 0, sizeof(struct _VKSK_GamepadInputs));
			memset(&gInputPrevious[i], 0, sizeof(struct _VKSK_GamepadInputs));
		}
	}
}

void vksk_RuntimeControllerConnectedCount(WrenVM *vm) {
	double count = 0;
	for (int i = 0; i < 4; i++) {
		if (gControllers[i] != NULL && SDL_GameControllerGetAttached(gControllers[i]))
			count += 1;
	}
	wrenSetSlotDouble(vm, 0, count);
}

void vksk_RuntimeControllerStickDeadzone(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, gAxisDeadzone);
}

void vksk_RuntimeControllerStickDeadzoneSet(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	gAxisDeadzone = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeControllerTriggerDeadzone(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, gTriggerDeadzone);
}

void vksk_RuntimeControllerTriggerDeadzoneSet(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	gTriggerDeadzone = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeControllerButtonPressed(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	int button = (int)wrenGetSlotDouble(vm, 2);
	if (index >= 0 && index < 4 && button >= 0 && button < 15) {
		wrenSetSlotBool(vm, 0, gInput[index].inputs[button] && !gInputPrevious[index].inputs[button]);
	} else {
		wrenSetSlotBool(vm, 0, false);
	}
}

void vksk_RuntimeControllerButtonReleased(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	int button = (int)wrenGetSlotDouble(vm, 2);
	if (index >= 0 && index < 4 && button >= 0 && button < 15) {
		wrenSetSlotBool(vm, 0, !gInput[index].inputs[button] && gInputPrevious[index].inputs[button]);
	} else {
		wrenSetSlotBool(vm, 0, false);
	}
}

void vksk_RuntimeControllerButton(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	int button = (int)wrenGetSlotDouble(vm, 2);
	if (index >= 0 && index < 4 && button >= 0 && button < 15) {
		wrenSetSlotBool(vm, 0, gInput[index].inputs[button]);
	} else {
		wrenSetSlotBool(vm, 0, false);
	}
}

void vksk_RuntimeControllerLeftStickX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		double val = (double)SDL_GameControllerGetAxis(gControllers[index], SDL_CONTROLLER_AXIS_LEFTX) / (double)INT16_MAX;
		if (!(val > -gAxisDeadzone && val < gAxisDeadzone))
			wrenSetSlotDouble(vm, 0, val);
		else
			wrenSetSlotDouble(vm, 0, 0);
	} else {
		wrenSetSlotDouble(vm, 0, 0);
	}
}

void vksk_RuntimeControllerLeftStickY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		double val = (double)SDL_GameControllerGetAxis(gControllers[index], SDL_CONTROLLER_AXIS_LEFTY) / (double)INT16_MAX;
		if (!(val > -gAxisDeadzone && val < gAxisDeadzone))
			wrenSetSlotDouble(vm, 0, val);
		else
			wrenSetSlotDouble(vm, 0, 0);
	} else {
		wrenSetSlotDouble(vm, 0, 0);
	}
}

void vksk_RuntimeControllerRightStickX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		double val = (double)SDL_GameControllerGetAxis(gControllers[index], SDL_CONTROLLER_AXIS_RIGHTX) / (double)INT16_MAX;
		if (!(val > -gAxisDeadzone && val < gAxisDeadzone))
			wrenSetSlotDouble(vm, 0, val);
		else
			wrenSetSlotDouble(vm, 0, 0);
	} else {
		wrenSetSlotDouble(vm, 0, 0);
	}
}

void vksk_RuntimeControllerRightStickY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		double val = (double)SDL_GameControllerGetAxis(gControllers[index], SDL_CONTROLLER_AXIS_RIGHTY) / (double)INT16_MAX;
		if (!(val > -gAxisDeadzone && val < gAxisDeadzone))
			wrenSetSlotDouble(vm, 0, val);
		else
			wrenSetSlotDouble(vm, 0, 0);
	} else {
		wrenSetSlotDouble(vm, 0, 0);
	}
}

void vksk_RuntimeControllerLeftTrigger(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		double val = (double)SDL_GameControllerGetAxis(gControllers[index], SDL_CONTROLLER_AXIS_TRIGGERLEFT) / (double)INT16_MAX;
		if (!(val > -gTriggerDeadzone && val < gTriggerDeadzone))
			wrenSetSlotDouble(vm, 0, val);
		else
			wrenSetSlotDouble(vm, 0, 0);
	} else {
		wrenSetSlotDouble(vm, 0, 0);
	}
}

void vksk_RuntimeControllerRightTrigger(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		double val = (double)SDL_GameControllerGetAxis(gControllers[index], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / (double)INT16_MAX;
		if (!(val > -gTriggerDeadzone && val < gTriggerDeadzone))
			wrenSetSlotDouble(vm, 0, val);
		else
			wrenSetSlotDouble(vm, 0, 0);
	} else {
		wrenSetSlotDouble(vm, 0, 0);
	}
}

void vksk_RuntimeControllerRumble(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		uint16_t rumble = (uint16_t)(wrenGetSlotDouble(vm, 2) * UINT16_MAX);
		uint32_t duration = (uint32_t)wrenGetSlotDouble(vm, 3);
		SDL_GameControllerRumble(gControllers[index], rumble, rumble, duration);
	}
}

void vksk_RuntimeControllerIsConnected(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		wrenSetSlotBool(vm, 0, gControllers[index] != NULL);
	} else {
		wrenSetSlotBool(vm, 0, false);
	}
}

void vksk_RuntimeControllerName(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	int index = (int)wrenGetSlotDouble(vm, 1);
	if (index >= 0 && index < 4) {
		wrenSetSlotString(vm, 0, SDL_GameControllerName(gControllers[index]));
	} else {
		wrenSetSlotString(vm, 0, "N/A");
	}
}

static inline uint32_t *alphaToRGBA(uint8_t *pixels, int w, int h, bool aa) {
	uint32_t *out = calloc(1, w * h * sizeof(uint32_t));
	for (int i = 0; i < w * h; i++) {
		if (pixels[i] > 0) {
			uint32_t t = pixels[i];
			uint32_t alpha = amask == 0xff000000 ? t << 24 : pixels[i];
			if (aa || (!aa && pixels[i] > 50))
				out[i] = rmask + gmask + bmask + (amask & alpha);
		}
	}

	free(pixels);
	return (void*)out;
}

void vksk_RuntimeFontAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_BOOL, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *font = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	font->bitmapFont = malloc(sizeof(struct JUFont));
	font->type = FOREIGN_BITMAP_FONT;
	const char *filename = wrenGetSlotString(vm, 1);
	float size = wrenGetSlotDouble(vm, 2);
	bool aa = wrenGetSlotBool(vm, 3);
	int uniStart = wrenGetSlotDouble(vm, 4);
	int uniEnd = wrenGetSlotDouble(vm, 5);
	bool error = false;

	// Load initial font data
	int bufferSize, lineGap, ascent, descent;
	void *fntData = vksk_GetFileBuffer(filename, &bufferSize);
	if (fntData != NULL) {
		stbtt_fontinfo info = {0};
		if (stbtt_InitFont(&info, fntData, 0)) {
			float scale = stbtt_ScaleForPixelHeight(&info, size);
			stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
			font->bitmapFont->newLineHeight = (ascent * scale) - (descent * scale) + (lineGap * scale);
			font->bitmapFont->unicodeStart = uniStart;
			font->bitmapFont->unicodeEnd = uniEnd;
			float spaceSize = font->bitmapFont->newLineHeight / 2;

			// Calculate the width and height of the image we'll need
			font->bitmapFont->characters = calloc(uniEnd - uniStart + 1, sizeof(struct JUCharacter));
			float w = 0;
			float h = 0;
			for (int i = 0; i <= (uniEnd - uniStart); i++) {
				int codePoint = i + uniStart;
				int x0, y0, x1, y1;
				JUCharacter *c = &font->bitmapFont->characters[i];
				stbtt_GetCodepointBox(&info, codePoint, &x0, &y0, &x1, &y1);
				if (x1 != 0) {
					c->w = (x1 * scale) - (x0 * scale);
					c->h = (y1 * scale) - (y0 * scale);
					c->x = w;
					c->drawn = true;
				} else {
					c->w = spaceSize;
					c->h = 0;
					c->x = w;
					c->drawn = false;
				}
				w += c->w + 4;
				if (h < c->h) h = c->h;
			}
			SDL_Surface *bitmap = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
			VALIDATE_SDL(bitmap)

			// Draw each glyph to the surface
			for (int i = 0; i <= (uniEnd - uniStart); i++) {
				int codePoint = i + uniStart;
				JUCharacter *c = &font->bitmapFont->characters[i];
				if (c->drawn) {
					c->ykern = c->h;
					int xoff, yoff, width, height;
					uint8_t *alpha = stbtt_GetCodepointBitmap(&info, scale, scale, codePoint, &width, &height, &xoff,
															  &yoff);
					uint32_t *pixels = alphaToRGBA(alpha, width, height, aa);
					SDL_Surface *temp = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, 4 * width, rmask, gmask,
																 bmask,
																 amask);
					VALIDATE_SDL(temp)
					c->ykern = ((-(float) ascent * scale) + yoff) + ((ascent * scale) * 2);
					SDL_Rect dst = {c->x, c->y, width, height};
					SDL_BlitSurface(temp, NULL, bitmap, &dst);
					SDL_FreeSurface(temp);
					free(pixels);
				}
			}

			// Create image and texture from surface
			SDL_LockSurface(bitmap);
			font->bitmapFont->image = vk2dImageFromPixels(vk2dRendererGetDevice(), bitmap->pixels, bitmap->w,
														  bitmap->h);
			SDL_UnlockSurface(bitmap);
			SDL_FreeSurface(bitmap);
			font->bitmapFont->bitmap = vk2dTextureLoadFromImage(font->bitmapFont->image);
		} else {
			error = true;
		}
		free(fntData);
	} else {
		error = true;
	}
	if (error) {
		vksk_Error(false, "Failed to load font \"%s\"", wrenGetSlotString(vm, 1));
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeFontFinalize(void *data) {
	VKSK_RuntimeForeign *font = data;
	vk2dRendererWait();
	vk2dImageFree(vk2dTextureGetImage(font->bitmapFont->bitmap));
	juFontFree(font->bitmapFont);
}

void vksk_RuntimeFontSize(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	float w, h;
	const char *str = wrenGetSlotString(vm, 1);
	VKSK_RuntimeForeign *font = wrenGetSlotForeign(vm, 0);
	juFontUTF8Size(font->bitmapFont, &w, &h, "%s", str);
	wrenSetSlotNewList(vm, 0);
	wrenSetSlotDouble(vm, 1, w);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotDouble(vm, 1, h);
	wrenInsertInList(vm, 0, -1, 1);
}

void vksk_RuntimeFontFree(WrenVM *vm) {
	VKSK_RuntimeForeign *font = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	vk2dImageFree(vk2dTextureGetImage(font->bitmapFont->bitmap));
	juFontFree(font->bitmapFont);
	font->bitmapFont = NULL;
}
