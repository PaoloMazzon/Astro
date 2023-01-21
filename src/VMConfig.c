/// \file VMConfig.c
/// \author Paolo Mazzon
#include <string.h>
#include <stdio.h>

#include "src/VMConfig.h"
#include "src/RendererBindings.h"
#include "src/Runtime.h"
#include "src/VK2DTypes.h"
#include "src/Input.h"
#include "src/JUTypes.h"
#include "src/InternalBindings.h"

// Globals
extern const char *gAssetsFile;

void vksk_WrenWriteFn(WrenVM* vm, const char* text) {
	printf("%s", text);
	fflush(stdout);
}

void vksk_WrenErrorFn(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg) {
	switch (errorType) {
		case WREN_ERROR_COMPILE: {
			printf("[%s line %d] [Error] %s\n", module, line, msg);
		} break;
		case WREN_ERROR_STACK_TRACE: {
			printf("[%s line %d] in %s\n", module, line, msg);
		} break;
		case WREN_ERROR_RUNTIME: {
			printf("[Runtime Error] %s\n", msg);
		} break;
	}
	fflush(stdout);
}

const char* loadFile(const char *filename) {
	FILE* file = fopen(filename, "rb");
	unsigned char *buffer = NULL;
	int size = 0;

	if (file != NULL) {
		// Find file size
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		buffer = malloc(size + 1);
		buffer[size] = 0;

		if (buffer != NULL) {
			// Fill the buffer
			fread(buffer, 1, size, file);
		}
		fclose(file);
	}

	return (const char*)buffer;
}

void vksk_WrenLoadModuleComplete(WrenVM* vm, const char* module, WrenLoadModuleResult result)
{
	if (result.source) {
		free((void*)result.source);
	}
}

WrenLoadModuleResult vksk_WrenLoadModule(WrenVM* vm, const char* name) {
	static const char *ext = ".wren";
	char fname[200] = "prog/";
	strncat(fname, name, 200 - 1 - 5 - 5);
	strcat(fname, ext);
	WrenLoadModuleResult result = {0};
	result.onComplete = vksk_WrenLoadModuleComplete;
	if (strcmp(name, "Assets") != 0)
		result.source = loadFile(fname);
	else
		result.source = gAssetsFile;
	return result;
}

/*************** Foreign Class bindings ***************/
WrenForeignClassMethods vksk_WrenBindForeignClass(WrenVM* vm, const char* module, const char* className) {
	WrenForeignClassMethods methods = {};

	if (strcmp(module, "lib/Drawing") == 0) {
		if (strcmp(className, "BitmapFont") == 0) {
			methods.allocate = vksk_RuntimeJUBitmapFontAllocate;
			methods.finalize = vksk_RuntimeJUBitmapFontFinalize;
		} else if (strcmp(className, "Sprite") == 0) {
			methods.allocate = vksk_RuntimeJUSpriteAllocate;
			methods.finalize = vksk_RuntimeJUSpriteFinalize;
		} else if (strcmp(className, "Surface") == 0) {
			methods.allocate = vksk_RuntimeVK2DSurfaceAllocate;
			methods.finalize = vksk_RuntimeVK2DSurfaceFinalize;
		} else if (strcmp(className, "Texture") == 0) {
			methods.allocate = vksk_RuntimeVK2DTextureAllocate;
			methods.finalize = vksk_RuntimeVK2DTextureFinalize;
		}
	} else if (strcmp(module, "lib/Renderer") == 0) {
		if (strcmp(className, "Camera") == 0) {
			methods.allocate = vksk_RuntimeVK2DCameraAllocate;
			methods.finalize = vksk_RuntimeVK2DCameraFinalize;
		}
	} else if (strcmp(module, "lib/File") == 0) {
		if (strcmp(className, "INI") == 0) {
			methods.allocate = vksk_RuntimeINIAllocate;
			methods.finalize = vksk_RuntimeINIFinalize;
		}
	} else if (strcmp(module, "lib/Audio") == 0) {
		if (strcmp(className, "AudioData") == 0) {
			methods.allocate = vksk_RuntimeJUAudioDataAllocate;
			methods.finalize = vksk_RuntimeJUAudioDataFinalize;
		} else if (strcmp(className, "Audio") == 0) {
			methods.allocate = vksk_RuntimeJUAudioAllocate;
			methods.finalize = vksk_RuntimeJUAudioFinalize;
		}
	}

	return methods;
}

/*************** Foreign Function bindings ***************/

// Macro to make binding functions cleaner
#define BIND_METHOD(cn, st, s, f) if (strcmp(className, cn) == 0 && isStatic == st && strcmp(signature, s) == 0) {return f;}

WrenForeignMethodFn vksk_WrenBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature) {
	if (strcmp(module, "lib/Renderer") == 0) {
		BIND_METHOD("Renderer", true, "draw_circle(_,_,_)", vksk_RuntimeRendererDrawCircle)
		BIND_METHOD("Renderer", true, "draw_texture(_,_,_)", vksk_RuntimeRendererDrawTexture)
		BIND_METHOD("Renderer", true, "draw_texture(_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawTextureExt)
		BIND_METHOD("Renderer", true, "draw_texture_part(_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawTexturePart)
		BIND_METHOD("Renderer", true, "draw_texture_part(_,_,_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawTexturePartExt)
		BIND_METHOD("Renderer", true, "get_config()", vksk_RuntimeRendererGetConfig)
		BIND_METHOD("Renderer", true, "set_config(_)", vksk_RuntimeRendererSetConfig)
		BIND_METHOD("Renderer", true, "set_target(_)", vksk_RuntimeRendererSetTarget)
		BIND_METHOD("Renderer", true, "set_blend_mode(_)", vksk_RuntimeRendererSetBlendMode)
		BIND_METHOD("Renderer", true, "get_blend_mode()", vksk_RuntimeRendererGetBlendMode)
		BIND_METHOD("Renderer", true, "set_colour_mod(_)", vksk_RuntimeRendererSetColourMod)
		BIND_METHOD("Renderer", true, "get_colour_mod()", vksk_RuntimeRendererGetColourMod)
		BIND_METHOD("Renderer", true, "set_texture_camera(_)", vksk_RuntimeRendererSetTextureCamera)
		BIND_METHOD("Renderer", true, "average_frame_time()", vksk_RuntimeRendererGetAverageFrameTime)
		BIND_METHOD("Renderer", true, "lock_cameras(_)", vksk_RuntimeRendererLockCameras)
		BIND_METHOD("Renderer", true, "unlock_cameras()", vksk_RuntimeRendererUnlockCameras)
		BIND_METHOD("Renderer", true, "set_viewport(_,_,_,_)", vksk_RuntimeRendererSetViewport)
		BIND_METHOD("Renderer", true, "get_viewport()", vksk_RuntimeRendererGetViewport)
		BIND_METHOD("Renderer", true, "clear()", vksk_RuntimeRendererClear)
		BIND_METHOD("Renderer", true, "draw_rectangle(_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawRectangle)
		BIND_METHOD("Renderer", true, "draw_rectangle_outline(_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawRectangleOutline)
		BIND_METHOD("Renderer", true, "draw_circle_outline(_,_,_,_)", vksk_RuntimeRendererDrawCircleOutline)
		BIND_METHOD("Renderer", true, "draw_line(_,_,_,_)", vksk_RuntimeRendererDrawLine)
		BIND_METHOD("Renderer", true, "draw_shader(_,_,_,_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawShader)
		BIND_METHOD("Renderer", true, "draw_polygon(_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawPolygon)
		BIND_METHOD("Renderer", true, "draw_model(_,_,_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawModel)
		BIND_METHOD("Renderer", true, "draw_font(_,_,_,_)", vksk_RuntimeRendererDrawFont)
		BIND_METHOD("Renderer", true, "draw_sprite(_,_,_)", vksk_RuntimeRendererDrawSpritePos)
		BIND_METHOD("Renderer", true, "draw_sprite(_,_,_,_)", vksk_RuntimeRendererDrawSpriteFrame)
		BIND_METHOD("Camera", false, "get_type()", vksk_RuntimeVK2DCameraGetType)
		BIND_METHOD("Camera", false, "set_type(_)", vksk_RuntimeVK2DCameraSetType)
		BIND_METHOD("Camera", false, "get_x()", vksk_RuntimeVK2DCameraGetX)
		BIND_METHOD("Camera", false, "set_x(_)", vksk_RuntimeVK2DCameraSetX)
		BIND_METHOD("Camera", false, "get_y()", vksk_RuntimeVK2DCameraGetY)
		BIND_METHOD("Camera", false, "set_y(_)", vksk_RuntimeVK2DCameraSetY)
		BIND_METHOD("Camera", false, "get_w()", vksk_RuntimeVK2DCameraGetW)
		BIND_METHOD("Camera", false, "set_w(_)", vksk_RuntimeVK2DCameraSetW)
		BIND_METHOD("Camera", false, "get_h()", vksk_RuntimeVK2DCameraGetH)
		BIND_METHOD("Camera", false, "set_h(_)", vksk_RuntimeVK2DCameraSetH)
		BIND_METHOD("Camera", false, "get_zoom()", vksk_RuntimeVK2DCameraGetZoom)
		BIND_METHOD("Camera", false, "set_zoom(_)", vksk_RuntimeVK2DCameraSetZoom)
		BIND_METHOD("Camera", false, "get_rotation()", vksk_RuntimeVK2DCameraGetRotation)
		BIND_METHOD("Camera", false, "set_rotation(_)", vksk_RuntimeVK2DCameraSetRotation)
		BIND_METHOD("Camera", false, "get_x_on_screen()", vksk_RuntimeVK2DCameraGetXOnScreen)
		BIND_METHOD("Camera", false, "set_x_on_screen(_)", vksk_RuntimeVK2DCameraSetXOnScreen)
		BIND_METHOD("Camera", false, "get_y_on_screen()", vksk_RuntimeVK2DCameraGetYOnScreen)
		BIND_METHOD("Camera", false, "set_y_on_screen(_)", vksk_RuntimeVK2DCameraSetYOnScreen)
		BIND_METHOD("Camera", false, "get_w_on_screen()", vksk_RuntimeVK2DCameraGetWOnScreen)
		BIND_METHOD("Camera", false, "set_w_on_screen(_)", vksk_RuntimeVK2DCameraSetWOnScreen)
		BIND_METHOD("Camera", false, "get_h_on_screen()", vksk_RuntimeVK2DCameraGetHOnScreen)
		BIND_METHOD("Camera", false, "set_h_on_screen(_)", vksk_RuntimeVK2DCameraSetHOnScreen)
		BIND_METHOD("Camera", false, "get_eyes()", vksk_RuntimeVK2DCameraGetEyes)
		BIND_METHOD("Camera", false, "set_eyes(_)", vksk_RuntimeVK2DCameraSetEyes)
		BIND_METHOD("Camera", false, "get_centre()", vksk_RuntimeVK2DCameraGetCentre)
		BIND_METHOD("Camera", false, "set_centre(_)", vksk_RuntimeVK2DCameraSetCentre)
		BIND_METHOD("Camera", false, "get_up()", vksk_RuntimeVK2DCameraGetUp)
		BIND_METHOD("Camera", false, "set_up(_)", vksk_RuntimeVK2DCameraSetUp)
		BIND_METHOD("Camera", false, "get_fov()", vksk_RuntimeVK2DCameraGetFov)
		BIND_METHOD("Camera", false, "set_fov(_)", vksk_RuntimeVK2DCameraSetFov)
		BIND_METHOD("Camera", false, "update()", vksk_RuntimeVK2DCameraUpdate)
	} else if (strcmp(module, "lib/Engine") == 0) {
		BIND_METHOD("Engine", true, "switch_level(_)", vksk_RuntimeSwitchLevel)
		BIND_METHOD("Engine", true, "quit()", vksk_RuntimeQuit)
		BIND_METHOD("Engine", true, "cap_fps(_)", vksk_RuntimeCapFPS)
		BIND_METHOD("Engine", true, "delta()", vksk_RuntimeDelta)
		BIND_METHOD("Engine", true, "time()", vksk_RuntimeTime)
	} else if (strcmp(module, "lib/Drawing") == 0) {
		BIND_METHOD("Texture", false, "width()", vksk_RuntimeVK2DTextureWidth)
		BIND_METHOD("Texture", false, "height()", vksk_RuntimeVK2DTextureHeight)
		BIND_METHOD("Texture", false, "free()", vksk_RuntimeVK2DTextureFree)
		BIND_METHOD("Surface", false, "width()", vksk_RuntimeVK2DSurfaceWidth)
		BIND_METHOD("Surface", false, "height()", vksk_RuntimeVK2DSurfaceHeight)
		BIND_METHOD("Surface", false, "free()", vksk_RuntimeVK2DSurfaceFree)
		BIND_METHOD("BitmapFont", false, "free()", vksk_RuntimeJUBitmapFontFree)
		BIND_METHOD("Sprite", false, "free()", vksk_RuntimeJUSpriteFree)
		BIND_METHOD("Sprite", false, "copy()", vksk_RuntimeJUSpriteCopy)
		BIND_METHOD("Sprite", false, "get_delay()", vksk_RuntimeJUSpriteGetDelay)
		BIND_METHOD("Sprite", false, "set_delay(_)", vksk_RuntimeJUSpriteSetDelay)
		BIND_METHOD("Sprite", false, "get_origin_x()", vksk_RuntimeJUSpriteGetOriginX)
		BIND_METHOD("Sprite", false, "set_origin_x(_)", vksk_RuntimeJUSpriteSetOriginX)
		BIND_METHOD("Sprite", false, "get_origin_y()", vksk_RuntimeJUSpriteGetOriginY)
		BIND_METHOD("Sprite", false, "set_origin_y(_)", vksk_RuntimeJUSpriteSetOriginY)
		BIND_METHOD("Sprite", false, "get_scale_x()", vksk_RuntimeJUSpriteGetScaleX)
		BIND_METHOD("Sprite", false, "set_scale_x(_)", vksk_RuntimeJUSpriteSetScaleX)
		BIND_METHOD("Sprite", false, "get_scale_y()", vksk_RuntimeJUSpriteGetScaleY)
		BIND_METHOD("Sprite", false, "set_scale_y(_)", vksk_RuntimeJUSpriteSetScaleY)
		BIND_METHOD("Sprite", false, "get_rotation()", vksk_RuntimeJUSpriteGetRotation)
		BIND_METHOD("Sprite", false, "set_rotation(_)", vksk_RuntimeJUSpriteSetRotation)
	} else if (strcmp(module, "lib/Input") == 0) {
		BIND_METHOD("Input", true, "keyboard_get_key(_)", vksk_RuntimeInputCheckKey)
		BIND_METHOD("Input", true, "keyboard_get_key_pressed(_)", vksk_RuntimeInputCheckKeyPressed)
		BIND_METHOD("Input", true, "keyboard_get_key_released(_)", vksk_RuntimeInputCheckKeyReleased)
		BIND_METHOD("Input", true, "get_mouse_position(_)", vksk_RuntimeInputGetMousePosition)
		BIND_METHOD("Input", true, "get_mouse_left()", vksk_RuntimeInputGetMouseLeftButton)
		BIND_METHOD("Input", true, "get_mouse_left_pressed()", vksk_RuntimeInputGetMouseLeftButtonPressed)
		BIND_METHOD("Input", true, "get_mouse_left_released()", vksk_RuntimeInputGetMouseLeftButtonReleased)
		BIND_METHOD("Input", true, "get_mouse_right()", vksk_RuntimeInputGetMouseRightButton)
		BIND_METHOD("Input", true, "get_mouse_right_pressed()", vksk_RuntimeInputGetMouseRightButtonPressed)
		BIND_METHOD("Input", true, "get_mouse_right_released()", vksk_RuntimeInputGetMouseRightButtonReleased)
		BIND_METHOD("Input", true, "get_mouse_middle()", vksk_RuntimeInputGetMouseMiddleButton)
		BIND_METHOD("Input", true, "get_mouse_middle_pressed()", vksk_RuntimeInputGetMouseMiddleButtonPressed)
		BIND_METHOD("Input", true, "get_mouse_middle_released()", vksk_RuntimeInputGetMouseMiddleButtonReleased)
	} else if (strcmp(module, "lib/File") == 0) {
		BIND_METHOD("INI", false, "flush(_)", vksk_RuntimeINIFlush)
		BIND_METHOD("INI", false, "key_exists(_,_)", vksk_RuntimeINIKeyExists)
		BIND_METHOD("INI", false, "get_string(_,_,_)", vksk_RuntimeINIGetString)
		BIND_METHOD("INI", false, "set_string(_,_,_)", vksk_RuntimeINISetString)
		BIND_METHOD("INI", false, "get_bool(_,_,_)", vksk_RuntimeINIGetBool)
		BIND_METHOD("INI", false, "set_bool(_,_,_)", vksk_RuntimeINISetBool)
		BIND_METHOD("INI", false, "get_num(_,_,_)", vksk_RuntimeINIGetNum)
		BIND_METHOD("INI", false, "set_num(_,_,_)", vksk_RuntimeINISetNum)
		BIND_METHOD("File", true, "read(_)", vksk_RuntimeFileRead)
		BIND_METHOD("File", true, "write(_,_)", vksk_RuntimeFileWrite)
		BIND_METHOD("File", true, "exists(_)", vksk_RuntimeFileExists)
	} else if (strcmp(module, "lib/Audio") == 0) {
		BIND_METHOD("AudioData", false, "free()", vksk_RuntimeJUAudioDataFree)
		BIND_METHOD("Audio", false, "update(_,_,_)", vksk_RuntimeJUAudioUpdate)
		BIND_METHOD("Audio", false, "stop()", vksk_RuntimeJUAudioStop)
		BIND_METHOD("Audio", true, "stop_all()", vksk_RuntimeJUAudioStopAll)
	}
	return NULL;
}