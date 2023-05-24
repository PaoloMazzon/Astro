#include <VK2D/VK2D.h>

#include "src/Runtime.h"
#include "src/ConfigFile.h"

VKSK_EngineConfig gEngineConfig = {0};

int main(int argc, char *argv[]) {
	// Read engine config ini
	VKSK_Config engineConfig = vksk_ConfigLoad("Astro.ini");
	gEngineConfig.enableTypeChecking = vksk_ConfigGetBool(engineConfig, "engine", "enableTypeChecking", false);
	gEngineConfig.enableDebugOverlay = vksk_ConfigGetBool(engineConfig, "engine", "enableDebugOverlay", false);
	gEngineConfig.enableAssetsPrint = vksk_ConfigGetBool(engineConfig, "engine", "enableAssetsPrint", false);
	gEngineConfig.disableGamePak = vksk_ConfigGetBool(engineConfig, "engine", "disableGamePak", false);
	gEngineConfig.gcBetweenLevels = vksk_ConfigGetBool(engineConfig, "engine", "gcBetweenLevels", true);
	vksk_ConfigFree(engineConfig);
	gEngineConfig.argc = argc;
	gEngineConfig.argv = argv;

	vksk_Start();
	return 0;
}
