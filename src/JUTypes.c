/// \file JUTypes.c
/// \author Paolo Mazzon
#include <src/JamUtil.h>
#include <src/stb_vorbis.h>
#define CUTE_SOUND_IMPLEMENTATION
#define CUTE_SOUND_FORCE_SDL
#define STB_VORBIS_INCLUDE_STB_VORBIS_H
#include <src/cute_sound.h>

#include "src/Validation.h"
#include "src/JUTypes.h"
#include "src/IntermediateTypes.h"

/********************* Bitmap Fonts *********************/
void vksk_RuntimeJUBitmapFontAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *font = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	font->bitmapFont = juFontLoadFromImage(
			wrenGetSlotString(vm, 1),
			(int)wrenGetSlotDouble(vm, 2),
			(int)wrenGetSlotDouble(vm, 3),
			(int)wrenGetSlotDouble(vm, 4),
			(int)wrenGetSlotDouble(vm, 5)
			);
	font->type = FOREIGN_BITMAP_FONT;
}

void vksk_RuntimeJUBitmapFontFinalize(void *data) {
	VKSK_RuntimeForeign *font = data;
	vk2dRendererWait();
	juFontFree(font->bitmapFont);
}

void vksk_RuntimeJUBitmapFontSize(WrenVM *vm) {
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

void vksk_RuntimeJUBitmapFontFree(WrenVM *vm) {
	VKSK_RuntimeForeign *font = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	juFontFree(font->bitmapFont);
	font->bitmapFont = NULL;
}

/********************* Sprites *********************/
void vksk_RuntimeJUSpriteAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	spr->type = FOREIGN_SPRITE;
	spr->sprite = juSpriteCreate(
			wrenGetSlotString(vm, 1),
			wrenGetSlotDouble(vm, 2),
			wrenGetSlotDouble(vm, 3),
			wrenGetSlotDouble(vm, 4),
			wrenGetSlotDouble(vm, 5),
			wrenGetSlotDouble(vm, 6),
			(int)wrenGetSlotDouble(vm, 7)
	);
}

void vksk_RuntimeJUSpriteFinalize(void *data) {
	VKSK_RuntimeForeign *spr = data;
	vk2dRendererWait();
	juSpriteFree(spr->sprite);
}

void vksk_RuntimeJUSpriteFree(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	juSpriteFree(spr->sprite);
	spr->sprite = NULL;
}

void vksk_RuntimeJUSpriteCopy(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenGetVariable(vm, "lib/Drawing", "Sprite", 0);
	VKSK_RuntimeForeign *newspr = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	newspr->sprite = juSpriteCopy(spr->sprite);
	newspr->type = FOREIGN_SPRITE;
}

void vksk_RuntimeJUSpriteFrom(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_TEXTURE | FOREIGN_SURFACE, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *tex = wrenGetSlotForeign(vm, 1);
	double x = wrenGetSlotDouble(vm, 2);
	double y = wrenGetSlotDouble(vm, 3);
	double w = wrenGetSlotDouble(vm, 4);
	double h = wrenGetSlotDouble(vm, 5);
	double delay = wrenGetSlotDouble(vm, 6);
	int frames = (int)wrenGetSlotDouble(vm, 7);
	wrenGetVariable(vm, "lib/Drawing", "Sprite", 0);
	VKSK_RuntimeForeign *newspr = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	newspr->sprite = juSpriteFrom(
			tex->type == FOREIGN_SURFACE ? tex->surface : tex->texture,
			x,
			y,
			w,
			h,
			delay,
			frames);
	newspr->type = FOREIGN_SPRITE;
}

void vksk_RuntimeJUSpriteGetFrame(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->Internal.frame);
}

void vksk_RuntimeJUSpriteSetFrame(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	int frame = wrenGetSlotDouble(vm, 1);
	if (frame == -1)
		spr->sprite->Internal.frame = spr->sprite->Internal.frames - 1;
	else if (frame < 0 || frame > spr->sprite->Internal.frames - 1)
		spr->sprite->Internal.frame = 0;
	else
		spr->sprite->Internal.frame = frame;
	spr->sprite->Internal.lastTime = SDL_GetPerformanceCounter();
}

void vksk_RuntimeJUSpriteGetFrameCount(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->Internal.frames);
}

void vksk_RuntimeJUSpriteGetDelay(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->delay);
}

void vksk_RuntimeJUSpriteSetDelay(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	spr->sprite->delay = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetOriginX(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->originX);
}

void vksk_RuntimeJUSpriteSetOriginX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	spr->sprite->originX = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetOriginY(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->originY);
}

void vksk_RuntimeJUSpriteSetOriginY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	spr->sprite->originY = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetScaleX(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->scaleX);
}

void vksk_RuntimeJUSpriteSetScaleX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	spr->sprite->scaleX = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetScaleY(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->scaleY);
}

void vksk_RuntimeJUSpriteSetScaleY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	spr->sprite->scaleY = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetRotation(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->rotation);
}

void vksk_RuntimeJUSpriteSetRotation(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	spr->sprite->rotation = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetWidth(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->Internal.w);
}

void vksk_RuntimeJUSpriteGetHeight(WrenVM *vm) {
	VKSK_RuntimeForeign *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, spr->sprite->Internal.h);
}


/********************* Audio data *********************/
void vksk_RuntimeJUAudioDataAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	VKSK_RuntimeForeign *snd = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	const char *fname = wrenGetSlotString(vm, 1);
	const char *ext = strrchr(fname, '.');
	if (strcmp(ext, ".wav") == 0) {
		snd->audioData = juSoundLoad(fname);
	} else if (strcmp(ext, ".ogg") == 0) {
		snd->audioData = malloc(sizeof(struct JUSound));
		snd->audioData->sound = cs_load_ogg(fname);
		memset(&snd->audioData->soundInfo, 0, sizeof(snd->audioData->soundInfo));
	} else {
		snd->audioData = NULL;
		vksk_Log("Unrecognized sound file type for file \"%s\"", fname);
	}

	snd->type = FOREIGN_AUDIO_DATA;
}

void vksk_RuntimeJUAudioDataFinalize(void *data) {
	VKSK_RuntimeForeign *d = data;
	juSoundFree(d->audioData);
}

void vksk_RuntimeJUAudioDataFree(WrenVM *vm) {
	VKSK_RuntimeForeign *snd = wrenGetSlotForeign(vm, 1);
	juSoundFree(snd->audioData);
	snd->audioData = NULL;
}

/********************* Audio *********************/
void vksk_RuntimeJUAudioAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_AUDIO_DATA, FOREIGN_BOOL, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *snd = wrenSetSlotNewForeign(vm, 0, 0, sizeof(VKSK_RuntimeForeign));
	VKSK_RuntimeForeign *aud = wrenGetSlotForeign(vm, 1);
	snd->audio = juSoundPlay(aud->audioData, wrenGetSlotBool(vm, 2), wrenGetSlotDouble(vm, 3), wrenGetSlotDouble(vm, 4));
	snd->type = FOREIGN_AUDIO;
}

void vksk_RuntimeJUAudioFinalize(void *data) {

}

void vksk_RuntimeJUAudioUpdate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
	VKSK_RuntimeForeign *snd = wrenGetSlotForeign(vm, 0);
	juSoundUpdate(snd->audio, wrenGetSlotBool(vm, 1), wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vksk_RuntimeJUAudioStop(WrenVM *vm) {
	VKSK_RuntimeForeign *snd = wrenGetSlotForeign(vm, 0);
	juSoundStop(snd->audio);
}

void vksk_RuntimeJUAudioPause(WrenVM *vm) {
	VKSK_RuntimeForeign *snd = wrenGetSlotForeign(vm, 0);
	cs_pause_sound(snd->audio.playingSound, 1);
}

void vksk_RuntimeJUAudioUnpause(WrenVM *vm) {
	VKSK_RuntimeForeign *snd = wrenGetSlotForeign(vm, 0);
	cs_pause_sound(snd->audio.playingSound, 0);
}

void vksk_RuntimeJUAudioStopAll(WrenVM *vm) {
	juSoundStopAll();
}
