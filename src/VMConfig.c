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
#include "src/UtilBindings.h"
#include "src/WrenPreprocessor.h"

// Globals
extern const char *gAssetsFile;

void vksk_WrenWriteFn(WrenVM* vm, const char* text) {
	printf("%s", text);
	fflush(stdout);
}

void vksk_WrenErrorFn(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg) {
	FILE *f = fopen("astroerror.txt", "a");
	int subtract = line < 10 ? 0 : 9; // - 9 because of preprocessor
	switch (errorType) {
		case WREN_ERROR_COMPILE: {
			printf("[%s line %d] [Error] %s\n", module, line - subtract, msg);
			fprintf(f, "[%s line %d] [Error] %s\n", module, line - subtract, msg);
		} break;
		case WREN_ERROR_STACK_TRACE: {
			printf("[%s line %d] in %s\n", module, line - subtract, msg);
			fprintf(f, "[%s line %d] in %s\n", module, line - subtract, msg);
		} break;
		case WREN_ERROR_RUNTIME: {
			printf("[Runtime Error] %s\n", msg);
			fprintf(f, "[Runtime Error] %s\n", msg);
		} break;
	}
	fflush(stdout);
	fclose(f);
}

void vksk_WrenLoadModuleComplete(WrenVM* vm, const char* module, WrenLoadModuleResult result)
{
	if (result.source) {
		free((void*)result.source);
	}
}
WrenLoadModuleResult vksk_WrenLoadModule(WrenVM* vm, const char* name) {
	static const char *ext = ".wren";
	char fname[200] = "data/game/";
	strncat(fname, name, 200 - 1 - 10 - 5);
	strcat(fname, ext);
	WrenLoadModuleResult result = {0};
	result.onComplete = vksk_WrenLoadModuleComplete;
	if (strcmp(name, "Assets") != 0)
		result.source = vksk_PreprocessSource(fname);
	else
		result.source = gAssetsFile;
	return result;
}

/*************** Foreign Class bindings ***************/
WrenForeignClassMethods vksk_WrenBindForeignClass(WrenVM* vm, const char* module, const char* className) {
	WrenForeignClassMethods methods = {0};

	if (strcmp(module, "lib/Drawing") == 0) {
		if (strcmp(className, "BitmapFont") == 0) {
			methods.allocate = vksk_RuntimeJUBitmapFontAllocate;
			methods.finalize = vksk_RuntimeJUBitmapFontFinalize;
		} else if (strcmp(className, "Font") == 0) {
			methods.allocate = vksk_RuntimeFontAllocate;
			methods.finalize = vksk_RuntimeFontFinalize;
		} else if (strcmp(className, "Sprite") == 0) {
			methods.allocate = vksk_RuntimeJUSpriteAllocate;
			methods.finalize = vksk_RuntimeJUSpriteFinalize;
		} else if (strcmp(className, "Surface") == 0) {
			methods.allocate = vksk_RuntimeVK2DSurfaceAllocate;
			methods.finalize = vksk_RuntimeVK2DSurfaceFinalize;
		} else if (strcmp(className, "Texture") == 0) {
			methods.allocate = vksk_RuntimeVK2DTextureAllocate;
			methods.finalize = vksk_RuntimeVK2DTextureFinalize;
		} else if (strcmp(className, "Model") == 0) {
			methods.allocate = vksk_RuntimeVK2DModelAllocate;
			methods.finalize = vksk_RuntimeVK2DModelFinalize;
		} else if (strcmp(className, "Polygon") == 0) {
			methods.allocate = vksk_RuntimeVK2DPolygonAllocate;
			methods.finalize = vksk_RuntimeVK2DPolygonFinalize;
		}
	} else if (strcmp(module, "lib/Renderer") == 0) {
		if (strcmp(className, "Camera") == 0) {
			methods.allocate = vksk_RuntimeVK2DCameraAllocate;
			methods.finalize = vksk_RuntimeVK2DCameraFinalize;
		} else if (strcmp(className, "Shader") == 0) {
			methods.allocate = vksk_RuntimeVK2DShaderAllocate;
			methods.finalize = vksk_RuntimeVK2DShaderFinalize;
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
	} else if (strcmp(module, "lib/Tiled") == 0) {
		if (strcmp(className, "TiledMap") == 0) {
			methods.allocate = vksk_RuntimeTiledAllocate;
			methods.finalize = vksk_RuntimeTiledFinalize;
		}
	} else if (strcmp(module, "lib/Util") == 0) {
        if (strcmp(className, "Buffer") == 0) {
            methods.allocate = vksk_RuntimeBufferAllocate;
            methods.finalize = vksk_RuntimeBufferFinalize;
        } else if (strcmp(className, "Hitbox") == 0) {
            methods.allocate = vksk_RuntimeUtilHitboxAllocate;
            methods.finalize = vksk_RuntimeUtilHitboxFinalize;
        }
	}

	return methods;
}

/*************** Foreign Function bindings ***************/

// Macro to make binding functions cleaner
#define BIND_METHOD(cn, st, s, f) if (strcmp(className, cn) == 0 && isStatic == st && strcmp(signature, s) == 0) {return f;}

WrenForeignMethodFn vksk_WrenBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature) {
	if (strcmp(module, "lib/Renderer") == 0) {
		BIND_METHOD("Renderer", true, "set_window_size(_,_)", vksk_RuntimeRendererSetWindowSize)
		BIND_METHOD("Renderer", true, "window_width", vksk_RuntimeRendererGetWindowWidth)
		BIND_METHOD("Renderer", true, "window_height", vksk_RuntimeRendererGetWindowHeight)
		BIND_METHOD("Renderer", true, "fullscreen", vksk_RuntimeRendererGetWindowFullscreen)
		BIND_METHOD("Renderer", true, "fullscreen=(_)", vksk_RuntimeRendererSetWindowFullscreen)
		BIND_METHOD("Renderer", true, "draw_circle(_,_,_)", vksk_RuntimeRendererDrawCircle)
		BIND_METHOD("Renderer", true, "draw_texture(_,_,_)", vksk_RuntimeRendererDrawTexture)
		BIND_METHOD("Renderer", true, "draw_texture(_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawTextureExt)
		BIND_METHOD("Renderer", true, "draw_texture_part(_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawTexturePart)
		BIND_METHOD("Renderer", true, "draw_texture_part(_,_,_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawTexturePartExt)
		BIND_METHOD("Renderer", true, "config", vksk_RuntimeRendererGetConfig)
		BIND_METHOD("Renderer", true, "config=(_)", vksk_RuntimeRendererSetConfig)
		BIND_METHOD("Renderer", true, "target=(_)", vksk_RuntimeRendererSetTarget)
		BIND_METHOD("Renderer", true, "blend_mode=(_)", vksk_RuntimeRendererSetBlendMode)
		BIND_METHOD("Renderer", true, "blend_mode", vksk_RuntimeRendererGetBlendMode)
		BIND_METHOD("Renderer", true, "colour_mod=(_)", vksk_RuntimeRendererSetColourMod)
		BIND_METHOD("Renderer", true, "colour_mod", vksk_RuntimeRendererGetColourMod)
		BIND_METHOD("Renderer", true, "shader=(_)", vksk_RuntimeRendererSetShader)
		BIND_METHOD("Renderer", true, "use_cameras_on_surfaces=(_)", vksk_RuntimeRendererSetTextureCamera)
		BIND_METHOD("Renderer", true, "average_frame_time", vksk_RuntimeRendererGetAverageFrameTime)
		BIND_METHOD("Renderer", true, "lock_cameras(_)", vksk_RuntimeRendererLockCameras)
		BIND_METHOD("Renderer", true, "unlock_cameras()", vksk_RuntimeRendererUnlockCameras)
		BIND_METHOD("Renderer", true, "clear()", vksk_RuntimeRendererClear)
		BIND_METHOD("Renderer", true, "draw_rectangle(_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawRectangle)
		BIND_METHOD("Renderer", true, "draw_rectangle_outline(_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawRectangleOutline)
		BIND_METHOD("Renderer", true, "draw_circle_outline(_,_,_,_)", vksk_RuntimeRendererDrawCircleOutline)
		BIND_METHOD("Renderer", true, "draw_line(_,_,_,_)", vksk_RuntimeRendererDrawLine)
		BIND_METHOD("Renderer", true, "draw_polygon(_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawPolygon)
		BIND_METHOD("Renderer", true, "draw_font(_,_,_,_)", vksk_RuntimeRendererDrawFont)
		BIND_METHOD("Renderer", true, "draw_font_wrapped(_,_,_,_,_)", vksk_RuntimeRendererDrawFontWrapped)
		BIND_METHOD("Renderer", true, "draw_font_ext(_,_,_,_)", vksk_RuntimeRendererDrawFontExt)
		BIND_METHOD("Renderer", true, "draw_font_ext_wrapped(_,_,_,_,_)", vksk_RuntimeRendererDrawFontExtWrapped)
		BIND_METHOD("Renderer", true, "draw_sprite(_,_,_)", vksk_RuntimeRendererDrawSpritePos)
		BIND_METHOD("Renderer", true, "draw_sprite(_,_,_,_)", vksk_RuntimeRendererDrawSpriteFrame)
		BIND_METHOD("Renderer", true, "clear_blank()", vksk_RuntimeRendererClearBlank)
		BIND_METHOD("Renderer", true, "draw_model(_,_,_,_)", vksk_RuntimeRendererDrawModel)
		BIND_METHOD("Renderer", true, "draw_model(_,_,_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawModelExt)
        BIND_METHOD("Renderer", true, "draw_polygon(_,_,_)", vksk_RuntimeRendererDrawPolygon)
        BIND_METHOD("Renderer", true, "draw_polygon(_,_,_,_,_,_,_,_,_,_)", vksk_RuntimeRendererDrawPolygonExt)
        BIND_METHOD("Renderer", true, "draw_lighting(_,_,_)", vksk_RuntimeRendererDrawLighting)
        BIND_METHOD("Renderer", true, "draw_fov(_,_)", vksk_RuntimeRendererDrawFOV)
        BIND_METHOD("Renderer", true, "setup_lighting(_,_,_,_)", vksk_RuntimeRendererSetupLighting)
        BIND_METHOD("Renderer", true, "hide_cursor=(_)", vksk_RuntimeRendererHideCursor)
        BIND_METHOD("Camera", false, "type", vksk_RuntimeVK2DCameraGetType)
		BIND_METHOD("Camera", false, "type=(_)", vksk_RuntimeVK2DCameraSetType)
		BIND_METHOD("Camera", false, "x", vksk_RuntimeVK2DCameraGetX)
		BIND_METHOD("Camera", false, "x=(_)", vksk_RuntimeVK2DCameraSetX)
		BIND_METHOD("Camera", false, "y", vksk_RuntimeVK2DCameraGetY)
		BIND_METHOD("Camera", false, "y=(_)", vksk_RuntimeVK2DCameraSetY)
		BIND_METHOD("Camera", false, "width", vksk_RuntimeVK2DCameraGetW)
		BIND_METHOD("Camera", false, "width=(_)", vksk_RuntimeVK2DCameraSetW)
		BIND_METHOD("Camera", false, "height", vksk_RuntimeVK2DCameraGetH)
		BIND_METHOD("Camera", false, "height=(_)", vksk_RuntimeVK2DCameraSetH)
		BIND_METHOD("Camera", false, "zoom", vksk_RuntimeVK2DCameraGetZoom)
		BIND_METHOD("Camera", false, "zoom=(_)", vksk_RuntimeVK2DCameraSetZoom)
		BIND_METHOD("Camera", false, "rotation", vksk_RuntimeVK2DCameraGetRotation)
		BIND_METHOD("Camera", false, "rotation=(_)", vksk_RuntimeVK2DCameraSetRotation)
		BIND_METHOD("Camera", false, "x_on_screen", vksk_RuntimeVK2DCameraGetXOnScreen)
		BIND_METHOD("Camera", false, "x_on_screen=(_)", vksk_RuntimeVK2DCameraSetXOnScreen)
		BIND_METHOD("Camera", false, "y_on_screen", vksk_RuntimeVK2DCameraGetYOnScreen)
		BIND_METHOD("Camera", false, "y_on_screen=(_)", vksk_RuntimeVK2DCameraSetYOnScreen)
		BIND_METHOD("Camera", false, "w_on_screen", vksk_RuntimeVK2DCameraGetWOnScreen)
		BIND_METHOD("Camera", false, "w_on_screen=(_)", vksk_RuntimeVK2DCameraSetWOnScreen)
		BIND_METHOD("Camera", false, "h_on_screen", vksk_RuntimeVK2DCameraGetHOnScreen)
		BIND_METHOD("Camera", false, "h_on_screen=(_)", vksk_RuntimeVK2DCameraSetHOnScreen)
		BIND_METHOD("Camera", false, "eyes", vksk_RuntimeVK2DCameraGetEyes)
		BIND_METHOD("Camera", false, "eyes=(_)", vksk_RuntimeVK2DCameraSetEyes)
		BIND_METHOD("Camera", false, "centre", vksk_RuntimeVK2DCameraGetCentre)
		BIND_METHOD("Camera", false, "centre=(_)", vksk_RuntimeVK2DCameraSetCentre)
		BIND_METHOD("Camera", false, "up", vksk_RuntimeVK2DCameraGetUp)
		BIND_METHOD("Camera", false, "up=(_)", vksk_RuntimeVK2DCameraSetUp)
		BIND_METHOD("Camera", false, "fov", vksk_RuntimeVK2DCameraGetFov)
		BIND_METHOD("Camera", false, "fov=(_)", vksk_RuntimeVK2DCameraSetFov)
		BIND_METHOD("Camera", false, "update()", vksk_RuntimeVK2DCameraUpdate)
		BIND_METHOD("Shader", false, "data=(_)", vksk_RuntimeVK2DShaderSetData)
	} else if (strcmp(module, "lib/Engine") == 0) {
		BIND_METHOD("Engine", true, "switch_level(_)", vksk_RuntimeSwitchLevel)
		BIND_METHOD("Engine", true, "quit()", vksk_RuntimeQuit)
		BIND_METHOD("Engine", true, "fps_limit=(_)", vksk_RuntimeCapFPS)
		BIND_METHOD("Engine", true, "delta", vksk_RuntimeDelta)
		BIND_METHOD("Engine", true, "time", vksk_RuntimeTime)
		BIND_METHOD("Engine", true, "fps", vksk_RuntimeFPS)
		BIND_METHOD("Engine", true, "info", vksk_RuntimeInfo)
		BIND_METHOD("Engine", true, "timestep=(_)", vksk_RuntimeTimestep)
		BIND_METHOD("Engine", true, "process_frame", vksk_RuntimeProcessFrame)
		BIND_METHOD("Engine", true, "timestep_percent", vksk_RuntimeTimeStepPercent)
		BIND_METHOD("Engine", true, "get_class(_)", vksk_RuntimeGetClass)
		BIND_METHOD("Engine", true, "report_debug(_)", vksk_RuntimeReportDebug)
		BIND_METHOD("Engine", true, "argv", vksk_RuntimeArgv)
		BIND_METHOD("Engine", true, "using_pak", vksk_RuntimeUsingPak)
		BIND_METHOD("Engine", true, "import_exists(_)", vksk_RuntimeImportExists)
		BIND_METHOD("Engine", true, "delta_max=(_)", vksk_RuntimeSetDeltaMax)
	} else if (strcmp(module, "lib/Drawing") == 0) {
		BIND_METHOD("Texture", false, "width", vksk_RuntimeVK2DTextureWidth)
		BIND_METHOD("Texture", false, "height", vksk_RuntimeVK2DTextureHeight)
		BIND_METHOD("Texture", false, "free()", vksk_RuntimeVK2DTextureFree)
		BIND_METHOD("Surface", false, "width", vksk_RuntimeVK2DSurfaceWidth)
		BIND_METHOD("Surface", false, "height", vksk_RuntimeVK2DSurfaceHeight)
		BIND_METHOD("Surface", false, "free()", vksk_RuntimeVK2DSurfaceFree)
		BIND_METHOD("BitmapFont", false, "free()", vksk_RuntimeJUBitmapFontFree)
		BIND_METHOD("BitmapFont", false, "size(_)", vksk_RuntimeJUBitmapFontSize)
		BIND_METHOD("BitmapFont", false, "size_wrapped(_,_)", vksk_RuntimeJUBitmapFontSizeWrapped)
		BIND_METHOD("BitmapFont", false, "size_ext(_)", vksk_RuntimeJUBitmapFontSizeExt)
		BIND_METHOD("BitmapFont", false, "size_ext_wrapped(_,_)", vksk_RuntimeJUBitmapFontSizeExtWrapped)
		BIND_METHOD("Font", false, "free()", vksk_RuntimeJUBitmapFontFree)
		BIND_METHOD("Font", false, "size(_)", vksk_RuntimeJUBitmapFontSize)
		BIND_METHOD("Font", false, "size_wrapped(_,_)", vksk_RuntimeJUBitmapFontSizeWrapped)
		BIND_METHOD("Font", false, "size_ext(_)", vksk_RuntimeJUBitmapFontSizeExt)
		BIND_METHOD("Font", false, "size_ext_wrapped(_,_)", vksk_RuntimeJUBitmapFontSizeExtWrapped)
		BIND_METHOD("Model", false, "free()", vksk_RuntimeVK2DModelFree)
		BIND_METHOD("Model", true, "load(_,_)", vksk_RuntimeVK2DModelLoad)
		BIND_METHOD("Polygon", false, "free()", vksk_RuntimeVK2DPolygonFree)
        BIND_METHOD("Polygon", true, "create(_)", vksk_RuntimeVK2DPolygonCreate)
		BIND_METHOD("Sprite", true, "from(_,_,_,_,_,_,_)", vksk_RuntimeJUSpriteFrom)
		BIND_METHOD("Sprite", false, "frame", vksk_RuntimeJUSpriteGetFrame)
		BIND_METHOD("Sprite", false, "frame=(_)", vksk_RuntimeJUSpriteSetFrame)
		BIND_METHOD("Sprite", false, "frame_count", vksk_RuntimeJUSpriteGetFrameCount)
		BIND_METHOD("Sprite", false, "free()", vksk_RuntimeJUSpriteFree)
		BIND_METHOD("Sprite", false, "copy()", vksk_RuntimeJUSpriteCopy)
		BIND_METHOD("Sprite", false, "delay", vksk_RuntimeJUSpriteGetDelay)
		BIND_METHOD("Sprite", false, "delay=(_)", vksk_RuntimeJUSpriteSetDelay)
		BIND_METHOD("Sprite", false, "origin_x", vksk_RuntimeJUSpriteGetOriginX)
		BIND_METHOD("Sprite", false, "origin_x=(_)", vksk_RuntimeJUSpriteSetOriginX)
		BIND_METHOD("Sprite", false, "origin_y", vksk_RuntimeJUSpriteGetOriginY)
		BIND_METHOD("Sprite", false, "origin_y=(_)", vksk_RuntimeJUSpriteSetOriginY)
		BIND_METHOD("Sprite", false, "scale_x", vksk_RuntimeJUSpriteGetScaleX)
		BIND_METHOD("Sprite", false, "scale_x=(_)", vksk_RuntimeJUSpriteSetScaleX)
		BIND_METHOD("Sprite", false, "scale_y", vksk_RuntimeJUSpriteGetScaleY)
		BIND_METHOD("Sprite", false, "scale_y=(_)", vksk_RuntimeJUSpriteSetScaleY)
        BIND_METHOD("Sprite", false, "rotation", vksk_RuntimeJUSpriteGetRotation)
        BIND_METHOD("Sprite", false, "rotation=(_)", vksk_RuntimeJUSpriteSetRotation)
        BIND_METHOD("Sprite", false, "width", vksk_RuntimeJUSpriteGetWidth)
        BIND_METHOD("Sprite", false, "height", vksk_RuntimeJUSpriteGetHeight)
        BIND_METHOD("Lighting", true, "add_shadow(_)", vksk_RuntimeLightingAddShadow)
        BIND_METHOD("Lighting", true, "add_light(_,_,_,_,_,_)", vksk_RuntimeLightingAddLight)
        BIND_METHOD("Lighting", true, "reset()", vksk_RuntimeLightingReset)
        BIND_METHOD("Lighting", true, "flush_vbo()", vksk_RuntimeLightingFlushVBO)
        BIND_METHOD("Shadow", false, "position=(_)", vksk_RuntimeShadowPositionSet)
        BIND_METHOD("Shadow", false, "update(_,_,_,_,_,_,_)", vksk_RuntimeShadowUpdate)
        BIND_METHOD("Shadow", false, "enabled", vksk_RuntimeShadowEnabledSet)
        BIND_METHOD("Shadow", false, "enabled=(_)", vksk_RuntimeShadowEnabledSet)
        BIND_METHOD("LightSource", false, "position", vksk_RuntimeLightSourcePositionGet)
        BIND_METHOD("LightSource", false, "position=(_)", vksk_RuntimeLightSourcePositionSet)
        BIND_METHOD("LightSource", false, "x", vksk_RuntimeLightSourceXGet)
        BIND_METHOD("LightSource", false, "x=(_)", vksk_RuntimeLightSourceXSet)
        BIND_METHOD("LightSource", false, "y", vksk_RuntimeLightSourceYGet)
        BIND_METHOD("LightSource", false, "y=(_)", vksk_RuntimeLightSourceYSet)
        BIND_METHOD("LightSource", false, "rotation", vksk_RuntimeLightSourceRotationGet)
        BIND_METHOD("LightSource", false, "rotation=(_)", vksk_RuntimeLightSourceRotationSet)
        BIND_METHOD("LightSource", false, "origin_x", vksk_RuntimeLightSourceOriginXGet)
        BIND_METHOD("LightSource", false, "origin_x=(_)", vksk_RuntimeLightSourceOriginXSet)
        BIND_METHOD("LightSource", false, "origin_y", vksk_RuntimeLightSourceOriginYGet)
        BIND_METHOD("LightSource", false, "origin_y=(_)", vksk_RuntimeLightSourceOriginYSet)
        BIND_METHOD("LightSource", false, "scale_x", vksk_RuntimeLightSourceScaleXGet)
        BIND_METHOD("LightSource", false, "scale_x=(_)", vksk_RuntimeLightSourceScaleXSet)
        BIND_METHOD("LightSource", false, "scale_y", vksk_RuntimeLightSourceScaleYGet)
        BIND_METHOD("LightSource", false, "scale_y=(_)", vksk_RuntimeLightSourceScaleYSet)
        BIND_METHOD("LightSource", false, "texture=(_)", vksk_RuntimeLightSourceTextureSet)
        BIND_METHOD("LightSource", false, "colour", vksk_RuntimeLightSourceColourGet)
        BIND_METHOD("LightSource", false, "colour=(_)", vksk_RuntimeLightSourceColourSet)
        BIND_METHOD("LightSource", false, "delete()", vksk_RuntimeLightSourceDelete)
    } else if (strcmp(module, "lib/Input") == 0) {
		BIND_METHOD("Keyboard", true, "key(_)", vksk_RuntimeInputCheckKey)
		BIND_METHOD("Keyboard", true, "key_pressed(_)", vksk_RuntimeInputCheckKeyPressed)
		BIND_METHOD("Keyboard", true, "key_released(_)", vksk_RuntimeInputCheckKeyReleased)
		BIND_METHOD("Keyboard", true, "last_key", vksk_RuntimeInputLastKey)
		BIND_METHOD("Keyboard", true, "last_key_code", vksk_RuntimeInputLastKeyCode)
		BIND_METHOD("Keyboard", true, "key_string(_)", vksk_RuntimeInputKeyString)
		BIND_METHOD("Mouse", true, "position(_)", vksk_RuntimeInputGetMousePosition)
		BIND_METHOD("Mouse", true, "left", vksk_RuntimeInputGetMouseLeftButton)
		BIND_METHOD("Mouse", true, "left_pressed", vksk_RuntimeInputGetMouseLeftButtonPressed)
		BIND_METHOD("Mouse", true, "left_released", vksk_RuntimeInputGetMouseLeftButtonReleased)
		BIND_METHOD("Mouse", true, "right", vksk_RuntimeInputGetMouseRightButton)
		BIND_METHOD("Mouse", true, "right_pressed", vksk_RuntimeInputGetMouseRightButtonPressed)
		BIND_METHOD("Mouse", true, "right_released", vksk_RuntimeInputGetMouseRightButtonReleased)
		BIND_METHOD("Mouse", true, "middle", vksk_RuntimeInputGetMouseMiddleButton)
		BIND_METHOD("Mouse", true, "middle_pressed", vksk_RuntimeInputGetMouseMiddleButtonPressed)
		BIND_METHOD("Mouse", true, "middle_released", vksk_RuntimeInputGetMouseMiddleButtonReleased)
		BIND_METHOD("Gamepad", true, "stick_deadzone", vksk_RuntimeControllerStickDeadzone)
		BIND_METHOD("Gamepad", true, "stick_deadzone=(_)", vksk_RuntimeControllerStickDeadzoneSet)
		BIND_METHOD("Gamepad", true, "trigger_deadzone", vksk_RuntimeControllerTriggerDeadzone)
		BIND_METHOD("Gamepad", true, "trigger_deadzone=(_)", vksk_RuntimeControllerTriggerDeadzoneSet)
		BIND_METHOD("Gamepad", true, "controllers_connected", vksk_RuntimeControllerConnectedCount)
		BIND_METHOD("Gamepad", true, "button_pressed(_,_)", vksk_RuntimeControllerButtonPressed)
		BIND_METHOD("Gamepad", true, "button_released(_,_)", vksk_RuntimeControllerButtonReleased)
		BIND_METHOD("Gamepad", true, "button(_,_)", vksk_RuntimeControllerButton)
		BIND_METHOD("Gamepad", true, "left_stick_x(_)", vksk_RuntimeControllerLeftStickX)
		BIND_METHOD("Gamepad", true, "left_stick_y(_)", vksk_RuntimeControllerLeftStickY)
		BIND_METHOD("Gamepad", true, "right_stick_x(_)", vksk_RuntimeControllerRightStickX)
		BIND_METHOD("Gamepad", true, "right_stick_y(_)", vksk_RuntimeControllerRightStickY)
		BIND_METHOD("Gamepad", true, "left_trigger(_)", vksk_RuntimeControllerLeftTrigger)
		BIND_METHOD("Gamepad", true, "right_trigger(_)", vksk_RuntimeControllerRightTrigger)
		BIND_METHOD("Gamepad", true, "rumble(_,_,_)", vksk_RuntimeControllerRumble)
		BIND_METHOD("Gamepad", true, "is_connected(_)", vksk_RuntimeControllerIsConnected)
		BIND_METHOD("Gamepad", true, "name(_)", vksk_RuntimeControllerName)
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
		BIND_METHOD("File", true, "read_from_pak(_)", vksk_RuntimeFileReadFromPak)
		BIND_METHOD("File", true, "write(_,_)", vksk_RuntimeFileWrite)
		BIND_METHOD("File", true, "exists(_)", vksk_RuntimeFileExists)
		BIND_METHOD("File", true, "exists_in_pak(_)", vksk_RuntimeFileExistsInPak)
		BIND_METHOD("File", true, "remove(_)", vksk_RuntimeFileRemove)
		BIND_METHOD("File", true, "rename(_,_)", vksk_RuntimeFileRename)
		BIND_METHOD("File", true, "dir_exists(_)", vksk_RuntimeFileDirExists)
		BIND_METHOD("File", true, "get_directory(_)", vksk_RuntimeFileGetDirectory)
	} else if (strcmp(module, "lib/Audio") == 0) {
		BIND_METHOD("AudioData", false, "free()", vksk_RuntimeJUAudioDataFree)
		BIND_METHOD("Audio", false, "update(_,_,_)", vksk_RuntimeJUAudioUpdate)
		BIND_METHOD("Audio", false, "stop()", vksk_RuntimeJUAudioStop)
		BIND_METHOD("Audio", true, "stop_all()", vksk_RuntimeJUAudioStopAll)
		BIND_METHOD("Audio", false, "pause()", vksk_RuntimeJUAudioPause)
		BIND_METHOD("Audio", false, "unpause()", vksk_RuntimeJUAudioUnpause)
	} else if (strcmp(module, "lib/Tiled") == 0) {
		BIND_METHOD("TiledMap", false, "width", vksk_RuntimeTiledWidth)
		BIND_METHOD("TiledMap", false, "height", vksk_RuntimeTiledHeight)
		BIND_METHOD("TiledMap", false, "cell_width", vksk_RuntimeTiledCellWidth)
		BIND_METHOD("TiledMap", false, "cell_height", vksk_RuntimeTiledCellHeight)
		BIND_METHOD("TiledMap", false, "layer_name", vksk_RuntimeTiledLayerName)
		BIND_METHOD("TiledMap", false, "next_layer()", vksk_RuntimeTiledNextLayer)
		BIND_METHOD("TiledMap", false, "get_objects()", vksk_RuntimeTiledGetObjects)
		BIND_METHOD("TiledMap", false, "get_tiles()", vksk_RuntimeTiledGetTiles)
		BIND_METHOD("TiledMap", false, "get_tilesets()", vksk_RuntimeTiledGetTilesets)
	} else if (strcmp(module, "lib/Util") == 0)  {
		BIND_METHOD("Buffer", false, "resize(_)", vksk_RuntimeBufferResize)
		BIND_METHOD("Buffer", false, "size", vksk_RuntimeBufferSize)
		BIND_METHOD("Buffer", true, "open(_)", vksk_RuntimeBufferOpen)
		BIND_METHOD("Buffer", true, "open_from_pak(_)", vksk_RuntimeBufferOpenFromPak)
		BIND_METHOD("Buffer", false, "pointer=(_)", vksk_RuntimeBufferSetPointer)
		BIND_METHOD("Buffer", false, "pointer", vksk_RuntimeBufferGetPointer)
		BIND_METHOD("Buffer", false, "read_double()", vksk_RuntimeBufferReadDouble)
		BIND_METHOD("Buffer", false, "write_double(_)", vksk_RuntimeBufferWriteDouble)
		BIND_METHOD("Buffer", false, "read_float()", vksk_RuntimeBufferReadFloat)
		BIND_METHOD("Buffer", false, "write_float(_)", vksk_RuntimeBufferWriteFloat)
		BIND_METHOD("Buffer", false, "read_uint64()", vksk_RuntimeBufferReadUint64)
		BIND_METHOD("Buffer", false, "write_uint64(_)", vksk_RuntimeBufferWriteUint64)
		BIND_METHOD("Buffer", false, "read_uint32()", vksk_RuntimeBufferReadUint32)
		BIND_METHOD("Buffer", false, "write_uint32(_)", vksk_RuntimeBufferWriteUint32)
		BIND_METHOD("Buffer", false, "read_uint16()", vksk_RuntimeBufferReadUint16)
		BIND_METHOD("Buffer", false, "write_uint16(_)", vksk_RuntimeBufferWriteUint16)
		BIND_METHOD("Buffer", false, "read_uint8()", vksk_RuntimeBufferReadUint8)
		BIND_METHOD("Buffer", false, "write_uint8(_)", vksk_RuntimeBufferWriteUint8)
		BIND_METHOD("Buffer", false, "read_int64()", vksk_RuntimeBufferReadInt64)
		BIND_METHOD("Buffer", false, "write_int64(_)", vksk_RuntimeBufferWriteInt64)
		BIND_METHOD("Buffer", false, "read_int32()", vksk_RuntimeBufferReadInt32)
		BIND_METHOD("Buffer", false, "write_int32(_)", vksk_RuntimeBufferWriteInt32)
		BIND_METHOD("Buffer", false, "read_int16()", vksk_RuntimeBufferReadInt16)
		BIND_METHOD("Buffer", false, "write_int16(_)", vksk_RuntimeBufferWriteInt16)
		BIND_METHOD("Buffer", false, "read_int8()", vksk_RuntimeBufferReadInt8)
		BIND_METHOD("Buffer", false, "write_int8(_)", vksk_RuntimeBufferWriteInt8)
		BIND_METHOD("Buffer", false, "read_string(_)", vksk_RuntimeBufferReadString)
		BIND_METHOD("Buffer", false, "write_string(_)", vksk_RuntimeBufferWriteString)
		BIND_METHOD("Buffer", false, "read_bool()", vksk_RuntimeBufferReadBool)
		BIND_METHOD("Buffer", false, "write_bool(_)", vksk_RuntimeBufferWriteBool)
        BIND_METHOD("Math", true, "round_to(_,_)", vksk_RuntimeUtilMathRoundTo)
        BIND_METHOD("Math", true, "floor_to(_,_)", vksk_RuntimeUtilMathFloorTo)
        BIND_METHOD("Math", true, "ceil_to(_,_)", vksk_RuntimeUtilMathCeilTo)
        BIND_METHOD("Math", true, "point_angle(_,_,_,_)", vksk_RuntimeUtilMathPointAngle)
        BIND_METHOD("Math", true, "point_distance(_,_,_,_)", vksk_RuntimeUtilMathPointDistance)
        BIND_METHOD("Math", true, "point_in_rectangle(_,_,_,_,_,_)", vksk_RuntimeUtilMathPointInRectangle)
        BIND_METHOD("Math", true, "point_in_circle(_,_,_,_,_)", vksk_RuntimeUtilMathPointInCircle)
        BIND_METHOD("Math", true, "lerp(_,_,_)", vksk_RuntimeUtilMathLerp)
        BIND_METHOD("Math", true, "serp(_,_,_)", vksk_RuntimeUtilMathSerp)
        BIND_METHOD("Math", true, "cast_x(_,_)", vksk_RuntimeUtilMathCastX)
        BIND_METHOD("Math", true, "cast_y(_,_)", vksk_RuntimeUtilMathCastY)
        BIND_METHOD("Math", true, "clamp(_,_,_)", vksk_RuntimeUtilMathClamp)
        BIND_METHOD("Hitbox", true, "new_circle(_)", vksk_RuntimeUtilHitboxNewCircle)
        BIND_METHOD("Hitbox", true, "NO_HIT", vksk_RuntimeUtilHitboxNewVoid)
        BIND_METHOD("Hitbox", true, "new_rectangle(_,_)", vksk_RuntimeUtilHitboxNewRectangle)
        BIND_METHOD("Hitbox", true, "new_polygon(_)", vksk_RuntimeUtilHitboxNewPolygon)
        BIND_METHOD("Hitbox", false, "r", vksk_RuntimeUtilHitboxR)
        BIND_METHOD("Hitbox", false, "w", vksk_RuntimeUtilHitboxW)
        BIND_METHOD("Hitbox", false, "h", vksk_RuntimeUtilHitboxH)
        BIND_METHOD("Hitbox", false, "no_hit", vksk_RuntimeUtilHitboxNoHit)
        BIND_METHOD("Hitbox", false, "x_offset=(_)", vksk_RuntimeUtilHitboxXOffsetSetter)
        BIND_METHOD("Hitbox", false, "y_offset=(_)", vksk_RuntimeUtilHitboxYOffsetSetter)
        BIND_METHOD("Hitbox", false, "x_offset", vksk_RuntimeUtilHitboxXOffsetGetter)
        BIND_METHOD("Hitbox", false, "y_offset", vksk_RuntimeUtilHitboxYOffsetGetter)
        BIND_METHOD("Hitbox", false, "collision(_,_,_,_,_)", vksk_RuntimeUtilHitboxCollision)
        BIND_METHOD("Hitbox", false, "bounding_box(_,_)", vksk_RuntimeUtilHitboxBoundingBox)
        BIND_METHOD("Hitbox", false, "bb_left(_,_)", vksk_RuntimeUtilHitboxBbLeft)
        BIND_METHOD("Hitbox", false, "bb_right(_,_)", vksk_RuntimeUtilHitboxBbRight)
        BIND_METHOD("Hitbox", false, "bb_top(_,_)", vksk_RuntimeUtilHitboxBbTop)
        BIND_METHOD("Hitbox", false, "bb_bottom(_,_)", vksk_RuntimeUtilHitboxBbBottom)
    }
	return NULL;
}