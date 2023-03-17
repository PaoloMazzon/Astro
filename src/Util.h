/// \file Util.h
/// \author Paolo Mazzon
/// \brief Various utilies
#pragma once

// Grabs a file buffer from either game.pak if it finds it or the filesystem
void *vksk_GetFileBuffer(const char *filename, int *size);