#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>

#include "src/Runtime.h"
#include "src/ConfigFile.h"

VKSK_EngineConfig gEngineConfig = {};

int main() {
	// Read engine config ini
	VKSK_Config engineConfig = vksk_ConfigLoad("Astro.ini");
	gEngineConfig.enableTypeChecking = vksk_ConfigGetBool(engineConfig, "engine", "enableTypeChecking", false);
	vksk_ConfigFree(engineConfig);

	vksk_Start();
	return 0;
}
