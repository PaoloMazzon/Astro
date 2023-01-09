/// \file RendererBindings.c
/// \author Paolo Mazzon
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>
#include "wren.h"

#include "src/RendererBindings.h"

// Globals
extern SDL_Window *gWindow; // -- from src/Runtime.c

VK2DMSAA convertToMSAAEnum(double wrenVal) {
	if (wrenVal == 2)
		return msaa_2x;
	if (wrenVal == 4)
		return msaa_4x;
	if (wrenVal == 8)
		return msaa_8x;
	if (wrenVal == 16)
		return msaa_16x;
	if (wrenVal == 32)
		return msaa_32x;
	return msaa_1x;
}

VK2DFilterType convertToFilterTypeEnum(double wrenVal) {
	if (wrenVal == 1)
		return ft_Linear;
	return ft_Nearest;
}

VK2DScreenMode convertToScreenModeEnum(double wrenVal) {
	if (wrenVal == 1)
		return sm_VSync;
	if (wrenVal == 2)
		return sm_TripleBuffer;
	return sm_Immediate;
}

// Uses 3 slots in front of mapSlot
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, VK2DRendererConfig *config) {
	wrenEnsureSlots(vm, 3 + mapSlot);
	int keySlot = mapSlot + 1;
	int valueSlot = mapSlot + 2;

	// Window name
	wrenSetSlotString(vm, mapSlot + 1, "window_title");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*windowTitle = wrenGetSlotString(vm, valueSlot);

	// Window width
	wrenSetSlotString(vm, mapSlot + 1, "window_width");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*windowWidth = (int)wrenGetSlotDouble(vm, valueSlot);

	// Window height
	wrenSetSlotString(vm, mapSlot + 1, "window_height");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	*windowHeight = (int)wrenGetSlotDouble(vm, valueSlot);

	// Config MSAA
	wrenSetSlotString(vm, mapSlot + 1, "msaa");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	config->msaa = convertToMSAAEnum(wrenGetSlotDouble(vm, valueSlot));

	// Config screen mode
	wrenSetSlotString(vm, mapSlot + 1, "screen_mode");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	config->screenMode = convertToScreenModeEnum(wrenGetSlotDouble(vm, valueSlot));

	// Config filter type
	wrenSetSlotString(vm, mapSlot + 1, "filter_type");
	wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	config->filterMode = convertToFilterTypeEnum(wrenGetSlotDouble(vm, valueSlot));
}

void vksk_RuntimeRendererDrawCircle(WrenVM *vm) {
	vk2dRendererDrawCircle(
			wrenGetSlotDouble(vm, 1),
			wrenGetSlotDouble(vm, 2),
			wrenGetSlotDouble(vm, 3)
			);
}