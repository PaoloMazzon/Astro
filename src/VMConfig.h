/// \file VMConfig.h
/// \author Paolo Mazzon
/// \brief Core functions the VM needs to operate
#pragma once
#include "wren.h"


void vksk_WrenWriteFn(WrenVM* vm, const char* text);

void vksk_WrenErrorFn(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg);

WrenForeignClassMethods vksk_WrenBindForeignClass(WrenVM* vm, const char* module, const char* className);

WrenForeignMethodFn vksk_WrenBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature);

WrenLoadModuleResult vksk_WrenLoadModule(WrenVM* vm, const char* name);