#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>

#include "src/Runtime.h"
#include "src/ConfigFile.h"

VKSK_EngineConfig gEngineConfig = {};

int main(int argc, const char *argv[]) {
	// Read engine config ini
	VKSK_Config engineConfig = vksk_ConfigLoad("Astro.ini");
	gEngineConfig.enableTypeChecking = vksk_ConfigGetBool(engineConfig, "engine", "enableTypeChecking", false);
	gEngineConfig.enableDebugOverlay = vksk_ConfigGetBool(engineConfig, "engine", "enableDebugOverlay", false);
	gEngineConfig.enableAssetsPrint = vksk_ConfigGetBool(engineConfig, "engine", "enableAssetsPrint", false);
	gEngineConfig.disableGamePak = vksk_ConfigGetBool(engineConfig, "engine", "disableGamePak", false);
	vksk_ConfigFree(engineConfig);
	gEngineConfig.argc = argc;
	gEngineConfig.argv = argv;

	vksk_Start();
	return 0;
}
