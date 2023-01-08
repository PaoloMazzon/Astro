#include <stdio.h>
#include <string.h>
#include "wren.h"

typedef float vec3[3];

void writeFn(WrenVM* vm, const char* text) {
	printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg) {
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
}

void vec3Allocate(WrenVM *vm) {
	vec3* vec = (vec3*)wrenSetSlotNewForeign(vm,0, 0, sizeof(vec3));
	(*vec)[0] = wrenGetSlotDouble(vm, 1);
	(*vec)[1] = wrenGetSlotDouble(vm, 2);
	(*vec)[2] = wrenGetSlotDouble(vm, 3);
}

void vec3Finalize(void *data) {

}

WrenForeignClassMethods bindForeignClass(WrenVM* vm, const char* module, const char* className) {
	WrenForeignClassMethods methods = {};

	if (strcmp(className, "Vec3") == 0) {
		methods.allocate = vec3Allocate;
		methods.finalize = vec3Finalize;
	}

	return methods;
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

static void loadModuleComplete(WrenVM* vm, const char* module, WrenLoadModuleResult result)
{
	if (result.source) {
		free((void*)result.source);
	}
}

WrenLoadModuleResult myLoadModule(WrenVM* vm, const char* name) {
	static const char *ext = ".wren";
	char fname[200] = "prog/";
	strncat(fname, name, 200 - 1 - 5 - 5);
	strcat(fname, ext);
	WrenLoadModuleResult result = {0};
	result.onComplete = loadModuleComplete;
	result.source = loadFile(fname);
	return result;
}

void mathAdd(WrenVM* vm)
{
	double a = wrenGetSlotDouble(vm, 1);
	double b = wrenGetSlotDouble(vm, 2);
	wrenSetSlotDouble(vm, 0, a + b);
}

void vec3Add(WrenVM *vm) {
	vec3 *vec = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, *vec[0] + *vec[1] + *vec[2]);
}

void vec3Dot(WrenVM *vm) {
	vec3 *vec1 = wrenGetSlotForeign(vm, 0);
	vec3 *vec2 = wrenGetSlotForeign(vm, 1);
	wrenSetSlotDouble(vm, 0, ((*vec1)[0] * (*vec2)[0]) + ((*vec1)[1] * (*vec2)[1]) + ((*vec1)[2] * (*vec2)[2]));
}

WrenForeignMethodFn bindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature) {
	if (strcmp(module, "test-suite") == 0) {
		if (strcmp(className, "Math") == 0) {
			if (isStatic && strcmp(signature, "add(_,_)") == 0) {
				return mathAdd; // C function for Math.add(_,_).
			}
			// Other foreign methods on Math...
		}
		if (strcmp(className, "Vec3") == 0) {
			if (strcmp(signature, "add()") == 0) {
				return vec3Add;
			} else if (strcmp(signature, "dot(_)") == 0) {
				return vec3Dot;
			}
		}
	}
	// Other modules...
}

void testWren() {
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &writeFn;
	config.errorFn = &errorFn;
	config.loadModuleFn = &myLoadModule;
	config.bindForeignMethodFn = &bindForeignMethod;
	config.bindForeignClassFn = &bindForeignClass;

	WrenVM* vm = wrenNewVM(&config);


	wrenInterpret(vm, "top", "import \"test-suite\"");
	WrenHandle *mathClassPrintHandle = wrenMakeCallHandle(vm, "print()");
	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, "test-suite", "Math", 0);
	WrenHandle *mathClassHandle = wrenGetSlotHandle(vm, 0);
	WrenInterpretResult result = wrenCall(vm, mathClassPrintHandle);

	switch (result) {
		case WREN_RESULT_COMPILE_ERROR:
		{ printf("Compile Error!\n"); } break;
		case WREN_RESULT_RUNTIME_ERROR:
		{ printf("Runtime Error!\n"); } break;
		case WREN_RESULT_SUCCESS:
		{ printf("Success!\n"); } break;
	}

	fflush(stdout);
	wrenFreeVM(vm);
}
