/// \file SteamBindings.h
/// \author Paolo Mazzon
/// \brief Bindings for the Steam API (C wrapper)
#pragma once
#include "wren.h"

#ifdef __cplusplus
extern "C" {
#endif

void vksk_SteamInit();
void vksk_SteamQuit();

void vksk_RuntimeSteamGetAchievement(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementAchievedPercent(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementAndUnlockTime(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementDisplayAttribute(WrenVM *vm);
void vksk_RuntimeSteamGetAchievementName(WrenVM *vm);
void vksk_RuntimeSteamIndicateAchievementProgress(WrenVM *vm);
void vksk_RuntimeSteamSetAchievement(WrenVM *vm);
void vksk_RuntimeSteamActivateGameOverlay(WrenVM *vm);
void vksk_RuntimeSteamActivateGameOverlayToWebPage(WrenVM *vm);
void vksk_RuntimeSteamGetPersonaName(WrenVM *vm);

#ifdef __cplusplus
}
#endif