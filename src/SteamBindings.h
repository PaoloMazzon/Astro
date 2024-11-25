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

#ifdef __cplusplus
}
#endif