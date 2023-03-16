#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "src/Packer.h"

int main(int argc, const char *argv[]) {
	/*VKSK_Pak pak = vksk_PakCreate();
	vksk_PakAddDirectory(pak, "assets");
	vksk_PakSave(pak, "game.pak");*/

	VKSK_Pak pak = vksk_PakLoad("game.pak");
	vksk_PakFree(pak);
	return 0;
}
