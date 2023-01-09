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