/// \file IntermediateTypes.h
/// \author Paolo Mazzon
/// \brief Types that the VM needs but don't correspond 1:1 to what they come from
#pragma once

// To protect Textures from garbage collection kinda
typedef struct _vksk_RuntimeTexture {
	bool freed;
	VK2DTexture tex;
}_vksk_RuntimeTexture;

// VK2D cameras in Wren are both index and spec because there is no access to the default camera
typedef struct _vksk_RuntimeCamera {
	VK2DCameraIndex index;
	VK2DCameraSpec spec;
} _vksk_RuntimeCamera;