/// \file SteamBindings.h
/// \author Paolo Mazzon
/// \brief Bindings for the Steam API (C wrapper)
#pragma once

#include "wren.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void vksk_SteamInit();
void vksk_SteamQuit();

void vksk_RuntimeSteamIsInitialized(WrenVM *vm);
void vksk_RuntimeSteamGetAchievement(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementAchievedPercent(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementAndUnlockTime(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementDisplayAttributes(WrenVM *vm);
void vksk_RuntimeSteamSetAchievement(WrenVM *vm);
void vksk_RuntimeSteamActivateGameOverlay(WrenVM *vm);
void vksk_RuntimeSteamActivateGameOverlayToWebPage(WrenVM *vm);
void vksk_RuntimeSteamGetPersonaName(WrenVM *vm);
void vksk_RuntimeSteamAttemptStoreStats(WrenVM *vm);

#ifdef __cplusplus
}
#endif