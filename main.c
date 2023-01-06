#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>

#include "wren.h"

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

int main() {
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &writeFn;
	config.errorFn = &errorFn;
	config.loadModuleFn = &myLoadModule;

	WrenVM* vm = wrenNewVM(&config);


	wrenInterpret(vm, "top", "import \"main\"");
	WrenHandle *mathClassPrintHandle = wrenMakeCallHandle(vm, "print()");
	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, "main", "Math", 0);
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

	wrenFreeVM(vm);
	return 0;
}
