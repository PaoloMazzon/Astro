/// \file IntermediateTypes.h
/// \author Paolo Mazzon
/// \brief Types that the VM needs but don't correspond 1:1 to what they come from
#pragma once
#include <stdint.h>
#include <VK2D/VK2D.h>

#include "src/JamUtil.h"
#include "src/ConfigFile.h"
#include "src/cute_tiled.h"

// Various Wren-types Astro cares about, can be bit-wised together - defined in Validation.c
extern const uint64_t FOREIGN_END;
extern const uint64_t FOREIGN_STRING;
extern const uint64_t FOREIGN_BOOL;
extern const uint64_t FOREIGN_NUM;
extern const uint64_t FOREIGN_LIST;
extern const uint64_t FOREIGN_MAP;
extern const uint64_t FOREIGN_NULL;
extern const uint64_t FOREIGN_FOREIGN;
extern const uint64_t FOREIGN_INI;
extern const uint64_t FOREIGN_BITMAP_FONT;
extern const uint64_t FOREIGN_TEXTURE;
extern const uint64_t FOREIGN_SPRITE;
extern const uint64_t FOREIGN_SURFACE;
extern const uint64_t FOREIGN_CAMERA;
extern const uint64_t FOREIGN_AUDIO_DATA;
extern const uint64_t FOREIGN_AUDIO;
extern const uint64_t FOREIGN_TILED_MAP;
extern const uint64_t FOREIGN_BUFFER;
extern const uint64_t FOREIGN_SHADER;
extern const uint64_t FOREIGN_POLYGON;
extern const uint64_t FOREIGN_MODEL;
extern const uint64_t FOREIGN_POLY_HITBOX;

// To protect Textures from garbage collection kinda
typedef struct _vksk_RuntimeTexture {
	VK2DTexture tex;
}_vksk_RuntimeTexture;

// VK2D cameras in Wren are both index and spec because there is no access to the default camera
typedef struct _vksk_RuntimeCamera {
	VK2DCameraIndex index;
	VK2DCameraSpec spec;
} _vksk_RuntimeCamera;

// For navigating tiled maps
typedef struct _vksk_RuntimeTiledMap {
	cute_tiled_map_t *map;
	cute_tiled_layer_t *layer;
} _vksk_RuntimeTiledMap;

// Binary buffers
typedef struct _vksk_RuntimeBuffer {
	int size;
	int pointer;
	uint8_t *data;
} _vksk_RuntimeBuffer;

typedef struct _vksk_RuntimeSprite {
	JUSprite spr;
	VK2DTexture tex;
} _vksk_RuntimeSprite;

typedef struct _vksk_RuntimeShader {
	VK2DShader shader;
	WrenHandle *data;
	int size;
} _vksk_RuntimeShader;

typedef struct _vksk_RuntimePolygonHitbox {
    vec2 *vertices;
    int count;
    double bb_left, bb_right, bb_top, bb_bottom;
} _vksk_RuntimePolygonHitbox;

// All foreign types come from this for type checking purposes
typedef struct VKSK_RuntimeForeign {
	uint64_t type;
	union {
		VKSK_Config ini;
		JUFont bitmapFont;
		_vksk_RuntimeTexture texture;
		VK2DTexture surface;
		_vksk_RuntimeSprite sprite;
		_vksk_RuntimeCamera camera;
		JUSound audioData;
		JUPlayingSound audio;
		_vksk_RuntimeTiledMap tiled;
		_vksk_RuntimeBuffer buffer;
		_vksk_RuntimeShader shader;
		VK2DPolygon polygon;
		VK2DModel model;
		_vksk_RuntimePolygonHitbox polygonHitbox;
	};
} VKSK_RuntimeForeign;