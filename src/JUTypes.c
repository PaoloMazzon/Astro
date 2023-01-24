/// \file JUTypes.c
/// \author Paolo Mazzon
#include <JamUtil/JamUtil.h>

#include "src/Validation.h"
#include "src/JUTypes.h"

/********************* Bitmap Fonts *********************/
void vksk_RuntimeJUBitmapFontAllocate(WrenVM *vm) {
	JUFont *font = wrenSetSlotNewForeign(vm, 0, 0, sizeof(JUFont));
	*font = juFontLoadFromImage(
			wrenGetSlotString(vm, 1),
			(int)wrenGetSlotDouble(vm, 2),
			(int)wrenGetSlotDouble(vm, 3),
			(int)wrenGetSlotDouble(vm, 4),
			(int)wrenGetSlotDouble(vm, 5)
			);

}

void vksk_RuntimeJUBitmapFontFinalize(void *data) {
	JUFont *font = data;
	vk2dRendererWait();
	juFontFree(*font);
}

void vksk_RuntimeJUBitmapFontFree(WrenVM *vm) {
	JUFont *font = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	juFontFree(*font);
	*font = NULL;
}

/********************* Sprites *********************/
void vksk_RuntimeJUSpriteAllocate(WrenVM *vm) {
	JUSprite *spr = wrenSetSlotNewForeign(vm, 0, 0, sizeof(JUSprite));
	*spr = juSpriteCreate(
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
	JUSprite *spr = data;
	vk2dRendererWait();
	juSpriteFree(*spr);
}

void vksk_RuntimeJUSpriteFree(WrenVM *vm) {
	JUSprite *font = wrenGetSlotForeign(vm, 0);
	vk2dRendererWait();
	juSpriteFree(*font);
	*font = NULL;
}

void vksk_RuntimeJUSpriteCopy(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenGetVariable(vm, "lib/Sprite", "Sprite", 0);
	JUSprite *newspr = wrenSetSlotNewForeign(vm, 0, 0, sizeof(JUSprite));
	*newspr = juSpriteCopy(*spr);
}

void vksk_RuntimeJUSpriteGetDelay(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->delay);
}

void vksk_RuntimeJUSpriteSetDelay(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	(*spr)->delay = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetOriginX(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->originX);
}

void vksk_RuntimeJUSpriteSetOriginX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	(*spr)->originX = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetOriginY(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->originY);
}

void vksk_RuntimeJUSpriteSetOriginY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	(*spr)->originY = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetScaleX(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->scaleX);
}

void vksk_RuntimeJUSpriteSetScaleX(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	(*spr)->scaleX = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetScaleY(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->scaleY);
}

void vksk_RuntimeJUSpriteSetScaleY(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	(*spr)->scaleY = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetRotation(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->rotation);
}

void vksk_RuntimeJUSpriteSetRotation(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	(*spr)->rotation = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeJUSpriteGetWidth(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->Internal.w);
}

void vksk_RuntimeJUSpriteGetHeight(WrenVM *vm) {
	JUSprite *spr = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, (*spr)->Internal.h);
}


/********************* Audio data *********************/
void vksk_RuntimeJUAudioDataAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING)
	JUSound *snd = wrenSetSlotNewForeign(vm, 0, 0, sizeof(JUSound));
	*snd = juSoundLoad(wrenGetSlotString(vm, 1));
}

void vksk_RuntimeJUAudioDataFinalize(void *data) {
	juSoundFree(*((JUSound*)data));
}

void vksk_RuntimeJUAudioDataFree(WrenVM *vm) {
	JUSound *snd = wrenGetSlotForeign(vm, 1);
	juSoundFree(*snd);
	*snd = NULL;
}

/********************* Audio *********************/
void vksk_RuntimeJUAudioAllocate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, "AudioData", FOREIGN_BOOL, FOREIGN_NUM, FOREIGN_NUM)
	JUPlayingSound *snd = wrenSetSlotNewForeign(vm, 0, 0, sizeof(JUPlayingSound));
	JUSound *aud = wrenGetSlotForeign(vm, 1);
	*snd = juSoundPlay(*aud, wrenGetSlotBool(vm, 2), wrenGetSlotDouble(vm, 3), wrenGetSlotDouble(vm, 4));
}

void vksk_RuntimeJUAudioFinalize(void *data) {

}

void vksk_RuntimeJUAudioUpdate(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_BOOL, FOREIGN_NUM, FOREIGN_NUM)
	JUPlayingSound *snd = wrenGetSlotForeign(vm, 0);
	juSoundUpdate(*snd, wrenGetSlotBool(vm, 1), wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vksk_RuntimeJUAudioStop(WrenVM *vm) {
	JUPlayingSound *snd = wrenGetSlotForeign(vm, 0);
	juSoundStop(*snd);
}

void vksk_RuntimeJUAudioStopAll(WrenVM *vm) {
	juSoundStopAll();
}
