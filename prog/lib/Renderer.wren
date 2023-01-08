// Renderer.wren
// Author: Paolo Mazzon
// Wren-accessible interface for Vulkan2D's renderer (plus a bit more)

class Renderer {
	// Must be called before any other Vulkan2D/JamUtil related functions
	foreign static init(window_width, window_height, msaa, screen_mode, filter_type)

	// Call in a while loop - returns false if the program needs to quit and true otherwise.
	// It also updates the underlying window and renderer.
	foreign static update()
}