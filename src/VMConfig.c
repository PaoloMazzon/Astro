/// \file VMConfig.c
/// \author Paolo Mazzon
#include <string.h>
#include <stdio.h>

#include "src/VMConfig.h"
#include "src/RendererBindings.h"
#include "src/Runtime.h"
#include "src/VK2DTypes.h"

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

static const char* loadFile(const char *filename) {
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
	result.source = loadFile(fname);
	return result;
}

/*************** Foreign Class bindings ***************/
WrenForeignClassMethods vksk_WrenBindForeignClass(WrenVM* vm, const char* module, const char* className) {
	WrenForeignClassMethods methods = {};

	if (strcmp(module, "lib/Texture") == 0) {
		if (strcmp(className, "Texture") == 0) {
			methods.allocate = vksk_RuntimeVK2DTextureAllocate;
			methods.finalize = vksk_RuntimeVK2DTextureFinalize;
		}
	} else if (strcmp(module, "lib/Surface") == 0) {
		if (strcmp(className, "Surface") == 0) {
			methods.allocate = vksk_RuntimeVK2DSurfaceAllocate;
			methods.finalize = vksk_RuntimeVK2DSurfaceFinalize;
		}
	}

	return methods;
}

/*************** Foreign Function bindings ***************/
WrenForeignMethodFn vksk_WrenBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature) {
	if (strcmp(module, "lib/Renderer") == 0) {
		if (strcmp(className, "Renderer") == 0) {
			if (isStatic && strcmp(signature, "draw_circle(_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawCircle;
			} else if (isStatic && strcmp(signature, "draw_texture(_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawTexture;
			} else if (isStatic && strcmp(signature, "draw_texture(_,_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawTextureExt;
			} else if (isStatic && strcmp(signature, "draw_texture_part(_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawTexturePart;
			} else if (isStatic && strcmp(signature, "draw_texture_part(_,_,_,_,_,_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawTexturePartExt;
			} else if (isStatic && strcmp(signature, "get_config()") == 0) {
				return vksk_RuntimeRendererGetConfig;
			} else if (isStatic && strcmp(signature, "set_config(_)") == 0) {
				return vksk_RuntimeRendererSetConfig;
			} else if (isStatic && strcmp(signature, "set_target(_)") == 0) {
				return vksk_RuntimeRendererSetTarget;
			} else if (isStatic && strcmp(signature, "set_blend_mode(_)") == 0) {
				return vksk_RuntimeRendererSetBlendMode;
			} else if (isStatic && strcmp(signature, "get_blend_mode()") == 0) {
				return vksk_RuntimeRendererGetBlendMode;
			} else if (isStatic && strcmp(signature, "set_colour_mod(_)") == 0) {
				return vksk_RuntimeRendererSetColourMod;
			} else if (isStatic && strcmp(signature, "get_colour_mod()") == 0) {
				return vksk_RuntimeRendererGetColourMod;
			} else if (isStatic && strcmp(signature, "set_texture_camera(_)") == 0) {
				return vksk_RuntimeRendererSetTextureCamera;
			} else if (isStatic && strcmp(signature, "average_frame_time()") == 0) {
				return vksk_RuntimeRendererGetAverageFrameTime;
			} else if (isStatic && strcmp(signature, "set_camera(_)") == 0) {
				return vksk_RuntimeRendererSetCamera;
			} else if (isStatic && strcmp(signature, "get_camera()") == 0) {
				return vksk_RuntimeRendererGetCamera;
			} else if (isStatic && strcmp(signature, "lock_cameras(_)") == 0) {
				return vksk_RuntimeRendererLockCameras;
			} else if (isStatic && strcmp(signature, "unlock_cameras()") == 0) {
				return vksk_RuntimeRendererUnlockCameras;
			} else if (isStatic && strcmp(signature, "set_viewport(_,_,_,_)") == 0) {
				return vksk_RuntimeRendererSetViewport;
			} else if (isStatic && strcmp(signature, "get_viewport()") == 0) {
				return vksk_RuntimeRendererGetViewport;
			} else if (isStatic && strcmp(signature, "clear()") == 0) {
				return vksk_RuntimeRendererClear;
			} else if (isStatic && strcmp(signature, "draw_rectangle(_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawRectangle;
			} else if (isStatic && strcmp(signature, "draw_rectangle_outline(_,_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawRectangleOutline;
			} else if (isStatic && strcmp(signature, "draw_circle_outline(_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawCircleOutline;
			} else if (isStatic && strcmp(signature, "draw_line(_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawLine;
			} else if (isStatic && strcmp(signature, "draw_shader(_,_,_,_,_,_,_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawShader;
			} else if (isStatic && strcmp(signature, "draw_polygon(_,_,_,_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawPolygon;
			} else if (isStatic && strcmp(signature, "draw_model(_,_,_,_,_,_,_,_,_,_,_,_)") == 0) {
				return vksk_RuntimeRendererDrawModel;
			}
		}
	} else if (strcmp(module, "lib/Runtime") == 0) {
		if (strcmp(className, "Runtime") == 0) {
			if (isStatic && strcmp(signature, "switch_level(_)") == 0) {
				return vksk_RuntimeSwitchLevel;
			} else if (isStatic && strcmp(signature, "quit()") == 0) {
				return vksk_RuntimeQuit;
			}
		}
	} else if (strcmp(module, "lib/Texture") == 0) {
		if (strcmp(className, "Texture") == 0) {
			if (strcmp(signature, "width()") == 0) {
				return vksk_RuntimeVK2DTextureWidth;
			} else if (strcmp(signature, "height()") == 0) {
				return vksk_RuntimeVK2DTextureHeight;
			}
		}
	} else if (strcmp(module, "lib/Surface") == 0) {
		if (strcmp(className, "Surface") == 0) {
			if (strcmp(signature, "width()") == 0) {
				return vksk_RuntimeVK2DSurfaceWidth;
			} else if (strcmp(signature, "height()") == 0) {
				return vksk_RuntimeVK2DSurfaceHeight;
			}
		}
	}
	return NULL;
}