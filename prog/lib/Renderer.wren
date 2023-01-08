// Renderer.wren
// Author: Paolo Mazzon
// Wren-accessible interface for Vulkan2D's renderer (plus a bit more)

class Renderer {
	static MSAA_1X { 1 }
	static MSAA_2X { 2 }
	static MSAA_4X { 4 }
	static MSAA_8X { 8 }
	static MSAA_16X { 16 }
	static MSAA_32X { 32 }
	static SCREEN_MODE_IMMEDIATE { 0 }
	static SCREEN_MODE_VSYNC { 1 }
	static SCREEN_MODE_TRIPLE_BUFFER { 2 }
	static FILTER_TYPE_NEAREST { 0 }
	static FILTER_TYPE_LINEAR { 1 }

	// Must be called before any other Vulkan2D/JamUtil related functions
	foreign static init(window_width, window_height, msaa, screen_mode, filter_type)

	// Call in a while loop - returns false if the program needs to quit and true otherwise.
	// It also updates the underlying window and renderer.
	foreign static update()
}