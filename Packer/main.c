#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "src/Packer.h"

int main(int argc, const char *argv[]) {
	printf("Creating game.pak...");

	// Creates a pak from prog and assets
	VKSK_Pak pak = vksk_PakCreate();
	printf("Packing assets...");
	vksk_PakAddDirectory(pak, "assets");
	printf("Packing source...");
	vksk_PakAddDirectory(pak, "prog");
	printf("Saving...");
	vksk_PakSave(pak, "game.pak");
	printf("Done.");
	vksk_PakFree(pak);

	pak = vksk_PakLoad("game.pak");
	vksk_PakPrintContents(pak);
	vksk_PakFree(pak);
	return 0;
}
