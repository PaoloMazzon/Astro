/// \file Runtime.c
/// \author Paolo Mazzon
#include "src/Runtime.h"
#include "src/VMConfig.h"
#include "src/RendererBindings.h"

void vksk_Start() {
	// Wren config and VM initialization
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &vksk_WrenWriteFn;
	config.errorFn = &vksk_WrenErrorFn;
	config.loadModuleFn = &vksk_WrenLoadModule;
	config.bindForeignMethodFn = &vksk_WrenBindForeignMethod;
	config.bindForeignClassFn = &vksk_WrenBindForeignClass;
	WrenVM* vm = wrenNewVM(&config);

	// Load the main module and run the main function
	wrenInterpret(vm, "top", "import \"main\"");
	WrenHandle *mainClassPrintHandle = wrenMakeCallHandle(vm, "main()");
	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, "main", "Main", 0);
	WrenHandle *mainClassHandle = wrenGetSlotHandle(vm, 0);
	WrenInterpretResult result = wrenCall(vm, mainClassPrintHandle);

	// Destroy resrouces
	vksk_RendererCleanup();
	wrenFreeVM(vm);
}