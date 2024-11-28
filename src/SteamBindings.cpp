/// \file SteamBindings.cpp
/// \author Paolo Mazzon
#include "src/SteamBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"

#include <ctime>
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
	} else {
		vksk_Log("SteamAPI initialized successfully.");
		gSteamAPIInit = true;
		gSteamUserID = SteamUser()->GetSteamID();
	}
}

void vksk_SteamQuit() {
	if (gSteamAPIInit) {
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	gSteamAPIInit = false;
}

// isInitialized -> bool
void vksk_RuntimeSteamIsInitialized(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
	wrenSetSlotBool(vm, 0, gSteamAPIInit);
}

// getAchievement(name) -> bool
void vksk_RuntimeSteamGetAchievement(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	bool achieved = false;
	if (gSteamAPIInit)
		SteamUserStats()->GetAchievement(wrenGetSlotString(vm, 1), &achieved);
	wrenSetSlotBool(vm, 0, achieved);
}

// getAchievementAchievedPercent(name) -> float
void vksk_RuntimeSteamGetAchievementAchievedPercent(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	float achievedPercent = 1.0f;
	if (gSteamAPIInit)
		SteamUserStats()->GetAchievementAchievedPercent(wrenGetSlotString(vm, 1), &achievedPercent);
	wrenSetSlotDouble(vm, 0, achievedPercent);
}

// getAchievementAndUnlockTime(name) -> [bool, string]
void vksk_RuntimeSteamGetAchievementAndUnlockTime(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	bool achieved = false;
	uint32_t seconds = 0;
	char *str;
	if (gSteamAPIInit)
		SteamUserStats()->GetAchievementAndUnlockTime(wrenGetSlotString(vm, 1), &achieved, &seconds);
	str = ctime((const time_t*)&seconds);
	wrenSetSlotNewList(vm, 0);
	wrenSetSlotBool(vm, 1, achieved);
	wrenInsertInList(vm, 0, -1, 1);
	wrenSetSlotString(vm, 1, str);
	wrenInsertInList(vm, 0, -1, 1);
}

// getAchievementDisplayAttributes(name) -> {"name": string, "desc": string, "hidden": bool, "unlocked": bool}
void vksk_RuntimeSteamGetAchievementDisplayAttributes(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	char *string;
	const char *pchName = wrenGetSlotString(vm, 1);
	const int keySlot = 1;
	const int valueSlot = 2;
	const int mapSlot = 0;
	bool achieved;
	wrenEnsureSlots(vm, 3);
	wrenSetSlotNewMap(vm, mapSlot);

	// Add the achievement attributes to a Wren map
	if (gSteamAPIInit) {
		wrenSetSlotString(vm, keySlot, "name");
		wrenSetSlotString(vm, valueSlot, SteamUserStats()->GetAchievementDisplayAttribute(pchName, "name"));
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);

		wrenSetSlotString(vm, keySlot, "desc");
		wrenSetSlotString(vm, valueSlot, SteamUserStats()->GetAchievementDisplayAttribute(pchName, "desc"));
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);

		wrenSetSlotString(vm, keySlot, "hidden");
		wrenSetSlotBool(vm, valueSlot, strcmp(SteamUserStats()->GetAchievementDisplayAttribute(pchName, "hidden"), "1") == 0);
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);
		
		wrenSetSlotString(vm, keySlot, "unlocked");
		SteamUserStats()->GetAchievement(pchName, &achieved);
		wrenSetSlotBool(vm, valueSlot, achieved);
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);
	} else {
		// Default values if the API is not initialized
		wrenSetSlotString(vm, keySlot, "name");
		wrenSetSlotString(vm, valueSlot, "");
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);

		wrenSetSlotString(vm, keySlot, "desc");
		wrenSetSlotString(vm, valueSlot, "");
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);

		wrenSetSlotString(vm, keySlot, "hidden");
		wrenSetSlotBool(vm, valueSlot, false);
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);

		wrenSetSlotString(vm, keySlot, "unlocked");
		wrenSetSlotBool(vm, valueSlot, false);
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);
	}
}

// setAchievement(name) -> bool
void vksk_RuntimeSteamSetAchievement(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	if (gSteamAPIInit) {
		SteamUserStats()->SetAchievement(wrenGetSlotString(vm, 0));
		wrenSetSlotBool(vm, 0, SteamUserStats()->StoreStats());
	}
}

// activateGameOverlay(dialogue)
void vksk_RuntimeSteamActivateGameOverlay(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	if (gSteamAPIInit) {
		SteamFriends()->ActivateGameOverlay(wrenGetSlotString(vm, 1));
	}
}

// activateGameOverlayToWebpage(website)
void vksk_RuntimeSteamActivateGameOverlayToWebPage(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	if (gSteamAPIInit) {
		SteamFriends()->ActivateGameOverlayToWebPage(wrenGetSlotString(vm, 1));
	}
}

// getPersonaName()
void vksk_RuntimeSteamGetPersonaName(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
	if (gSteamAPIInit)
		wrenSetSlotString(vm, 0, SteamFriends()->GetPersonaName());
	else
		wrenSetSlotString(vm, 0, "");
}

// attemptStoreStats() -> bool
void vksk_RuntimeSteamAttemptStoreStats(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
	if (gSteamAPIInit) {
		wrenSetSlotBool(vm, 0, SteamUserStats()->StoreStats());
	}
}

void vksk_RuntimeSteamGetStatFloat(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
		const char* pchName = wrenGetSlotString(vm, 1);
	float num = 0;
	if (gSteamAPIInit) {
		SteamUserStats()->GetStat(pchName, &num);
	}
	wrenSetSlotDouble(vm, 0, num);
}

void vksk_RuntimeSteamGetStatInt(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char* pchName = wrenGetSlotString(vm, 1);
	int num = 0;
	if (gSteamAPIInit) {
		SteamUserStats()->GetStat(pchName, &num);
	}
	wrenSetSlotDouble(vm, 0, num);
}

void vksk_RuntimeSteamSetStatFloat(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_END)
	const char *pchName = wrenGetSlotString(vm, 1);
	float num = (float)wrenGetSlotDouble(vm, 2);

	if (gSteamAPIInit) {
		SteamUserStats()->SetStat(pchName, num);
	}
}

void vksk_RuntimeSteamSetStatInt(WrenVM* vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_NUM, FOREIGN_END)
	const char* pchName = wrenGetSlotString(vm, 1);
	int num = (int)wrenGetSlotDouble(vm, 2);

	if (gSteamAPIInit) {
		SteamUserStats()->SetStat(pchName, num);
	}
}