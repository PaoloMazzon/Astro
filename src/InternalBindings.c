/// \file InternalBindings.c
/// \author Paolo Mazzon
#include <stdio.h>

#define CUTE_TILED_IMPLEMENTATION
#include "src/cute_tiled.h"
#include "src/InternalBindings.h"
#include "src/ConfigFile.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"

static const double NO_MORE_LAYERS = 0;
static const double TILE_LAYER = 1;
static const double OBJECT_LAYER = 2;

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

void vksk_RuntimeTiledAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *tiled = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
	tiled->tiled.map = cute_tiled_load_map_from_file(wrenGetSlotString(vm, 1), NULL);
	tiled->tiled.layer = NULL;
	tiled->type = FOREIGN_TILED_MAP;
	fflush(stdout);
	fflush(stderr);
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



void vksk_RuntimeControllerStickDeadzone(WrenVM *vm) {

}

void vksk_RuntimeControllerStickDeadzoneSet(WrenVM *vm) {

}

void vksk_RuntimeControllerTriggerDeadzone(WrenVM *vm) {

}

void vksk_RuntimeControllerTriggerDeadzoneSet(WrenVM *vm) {

}

void vksk_RuntimeControllerButtonPressed(WrenVM *vm) {

}

void vksk_RuntimeControllerButtonReleased(WrenVM *vm) {

}

void vksk_RuntimeControllerButton(WrenVM *vm) {

}

void vksk_RuntimeControllerLeftStickX(WrenVM *vm) {

}

void vksk_RuntimeControllerLeftStickY(WrenVM *vm) {

}

void vksk_RuntimeControllerRightStickX(WrenVM *vm) {

}

void vksk_RuntimeControllerRightStickY(WrenVM *vm) {

}

void vksk_RuntimeControllerLeftTrigger(WrenVM *vm) {

}

void vksk_RuntimeControllerRightTrigger(WrenVM *vm) {

}

void vksk_RuntimeControllerRumble(WrenVM *vm) {

}

void vksk_RuntimeControllerIsConnected(WrenVM *vm) {

}

void vksk_RuntimeControllerName(WrenVM *vm) {

}

