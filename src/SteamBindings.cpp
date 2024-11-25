/// \file SteamBindings.cpp
/// \author Paolo Mazzon
#include "src/SteamBindings.h"
#include "src/Validation.h"

#include "steam/steam_api.h"

// True if the Steam API was init successfully
bool static gSteamAPIInit = false;

void vksk_SteamInit() {
	SteamErrMsg error;
	ESteamAPIInitResult ret = SteamAPI_InitEx(&error);
	if (ret != k_ESteamAPIInitResult_OK) {
		vksk_Log("Failed to initialize Steam API: %s.", error);
	}
	else {
		gSteamAPIInit = true;
	}
}

void vksk_SteamQuit() {
	SteamAPI_Shutdown();
	gSteamAPIInit = false;
}