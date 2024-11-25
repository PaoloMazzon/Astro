/// \file SteamBindings.cpp
/// \author Paolo Mazzon
#include "src/SteamBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"

#include "steam/steam_api.h"
#include "steam/isteamfriends.h"
#include "steam/isteamuserstats.h"

// True if the Steam API was init successfully
static bool gSteamAPIInit = false;
static CSteamID gSteamUserID;

void vksk_SteamInit() {
	SteamErrMsg error;
	ESteamAPIInitResult ret = SteamAPI_InitEx(&error);
	if (ret != k_ESteamAPIInitResult_OK) {
		vksk_Log("Failed to initialize Steam API: %s.", error);
	}
	else {
		gSteamAPIInit = true;
		gSteamUserID = SteamUser()->GetSteamID();
	}
}

void vksk_SteamQuit() {
	SteamAPI_Shutdown();
	gSteamAPIInit = false;
}

void vksk_RuntimeSteamGetAchievement(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	bool achieved = false;
	if (gSteamAPIInit)
		SteamUserStats()->GetAchievement(wrenGetSlotString(vm, 1), &achieved);
	wrenSetSlotBool(vm, 0, achieved);
}

void vksk_RuntimeSteamGetAchievementAchievedPercent(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	float achievedPercent = 1.0f;
	if (gSteamAPIInit)
		SteamUserStats()->GetAchievement(wrenGetSlotString(vm, 1), &achievedPercent);
	wrenSetSlotDouble(vm, 0, achievedPercent);
}

void vksk_RuntimeSteamGetAchievementAndUnlockTime(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamGetAchievementDisplayAttribute(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamGetAchievementName(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamIndicateAchievementProgress(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamSetAchievement(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamActivateGameOverlay(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamActivateGameOverlayToWebPage(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)

}

void vksk_RuntimeSteamGetPersonaName(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
	if (gSteamAPIInit)
		wrenSetSlotString(vm, 0, SteamFriends()->GetPersonaName());
	else
		wrenSetSlotString(vm, 0, "");
}
