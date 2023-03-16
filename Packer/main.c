#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "src/Packer.h"

int main(int argc, const char *argv[]) {
	// Creates a pak from specified directories
	VKSK_Pak pak = vksk_PakCreate();
	for (int i = 1; i < argc; i++)
		vksk_PakAddDirectory(pak, argv[i]);
	vksk_PakSave(pak, "game.pak");
	return 0;
}
