// Renderer.wren
// Author: Paolo Mazzon
// Wren-accessible interface for Vulkan2D's renderer (plus a bit more)

// Bindings for Vulkan2D's Renderer.h
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

	// Draws a circle to the screen
	foreign static draw_circle(x, y, radius)
}
