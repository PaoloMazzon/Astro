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

	// Draws a texture to the screen - texture MUST be a Wren Texture handle
	foreign static draw_texture(texture, x, y)
	
	// Draws a texture to the screen with transformations - texture MUST be a Wren Texture handle
	foreign static draw_texture(texture, x, y, x_scale, y_scale, rotation, x_origin, y_origin)

	// Draws a part of a texture to the screen - texture MUST be a Wren Texture handle
	foreign static draw_texture_part(texture, x, y, x_in_tex, y_in_tex, width, height)

	// Draws a part of a texture to the screen with transformations - texture MUST be a Wren Texture handle
	foreign static draw_texture_part(texture, x, y, x_in_tex, y_in_tex, width, height, x_scale, y_scale, rotation, x_origin, y_origin)
}
