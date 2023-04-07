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
    static RENDER_TARGET_DEFAULT { null }
    static DEFAULT_CAMERA { null }
    static BLEND_MODE_BLEND { 0 }
    static BLEND_MODE_NONE { 1 }
    static BLEND_MODE_ADD { 2 }
    static BLEND_MODE_SUBTRACT { 3 }
    static COLOUR_RED { [1, 0, 0, 1] }
    static COLOUR_GREEN { [0, 1, 0, 1] }
    static COLOUR_BLUE { [0, 0, 1, 1] }
    static COLOUR_YELLOW { [1, 1, 0, 1] }
    static COLOUR_CYAN { [0, 1, 1, 1] }
    static COLOUR_PURPLE { [1, 0, 1, 1] }
    static COLOUR_BLACK { [0, 0, 0, 1] }
    static COLOUR_WHITE { [1, 1, 1, 1] }
    static COLOUR_DEFAULT { [1, 1, 1, 1] }
    
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

    // Returns a map of the current renderer config
    foreign static config

    // Sets the renderer's config using a provided map formatted in the same way as
    // the map you set in `init.wren`
    foreign static config=(config)

    // Sets the target the renderer will draw to, can either be a Surface or
    // `Renderer.RENDER_TARGET_DEFAULT`
    foreign static target=(target)

    // Sets the renderer's blend mode, must be one of the `Renderer.BLEND_MODE_*`
    // values
    foreign static blend_mode=(blend_mode)

    // Returns the current blend mode, will be one of the `Renderer.BLEND_MODE_*`
    // values
    foreign static blend_mode

    // Sets the render colour all things are drawn with, must be a 4-element
    // RGBA list of values in the range [0,1]. You may also use the
    // `Renderer.COLOUR_*` values
    foreign static colour_mod=(colour)

    // Returns a list of 4 RGBA values from 0 to 1
    foreign static colour_mod

    // If true, the renderer's cameras will be used when drawing to surfaces.
    // Otherwise everything drawn to a surface will be drawn from that surface's
    // origin in the top-left
    foreign static use_cameras_on_surfaces=(enable)

    // Returns the average frame time in seconds
    foreign static average_frame_time

    // Locks the cameras so only a selected camera will be rendered to, may
    // be either a `Camera` instance or `Renderer.DEFAULT_CAMERA`
    foreign static lock_cameras(camera)

    // Unlocks the cameras so they will all be rendered to again
    foreign static unlock_cameras()

    // Sets the current rendering shader for textures (and sprites by extension)
    foreign static shader=(shader)

    // Wipes the screen with the current rendering colour
    foreign static clear()

    // Wipes the screen to 0 everything
    foreign static clear_blank()

    // Draws a rectangle with the current render colour
    foreign static draw_rectangle(x, y, w, h, rotation, x_origin, y_origin)

    // Draws a rectangle outline with the current render colour
    foreign static draw_rectangle_outline(x, y, w, h, rotation, x_origin, y_origin, outline_thickness)

    // Draws a circle outline with the current render colour
    foreign static draw_circle_outline(x, y, r, outline_width)

    // Draws a line with the current render colour
    foreign static draw_line(x1, y1, x2, y2)

    // Draws text, font MUST be a `BitmapFont`
    foreign static draw_font(font, text, x, y)

    // Draws a sprite at a specific position, sprite MUST be a `Sprite`
    foreign static draw_sprite(sprite, x, y)

    // Draws a specific frame of a sprite at a specific position, sprite MUST be a `Sprite`
    foreign static draw_sprite(sprite, frame, x, y)

    // Draws a model with less arguments
    foreign static draw_model(model, x, y, z)

    // Draws a model in detail
    foreign static draw_model(model, x, y, z, x_scale, y_scale, z_scale, rotation, axis, origin_x, origin_y, origin_z)

    // Draws a polygon with less arguments
    foreign static draw_polygon(polygon, x, y)

    // Draws a polygon in detail
    foreign static draw_polygon(polygon, x, y, x_scale, y_scale, rotation, origin_x, origin_y, outline, line_width)
}

// Cameras that allow you to control what is displayed on screen
foreign class Camera {
    static CAMERA_TYPE_DEFAULT { 0 }
    static CAMERA_TYPE_PERSPECTIVE { 1 }
    static CAMERA_TYPE_ORTHOGRAPHIC { 2 }

    construct new() {}

    // Gets this camera's type
    foreign type

    // Sets this camera's type (should be one of `Camera.CAMERA_TYPE_*`)
    foreign type=(type)

    // Gets this camera's x
	foreign x

    // Sets this camera's x
    foreign x=(x)

    // Gets this camera's y
	foreign y

    // Sets this camera's y
    foreign y=(y)

    // Gets this camera's w
	foreign width

    // Sets this camera's w
    foreign width=(w)

    // Gets this camera's h
	foreign height

    // Sets this camera's h
    foreign height=(h)

    // Gets this camera's zoom
	foreign zoom

    // Sets this camera's zoom
    foreign zoom=(zoom)

    // Gets this camera's rotation
	foreign rotation

    // Sets this camera's rotation
    foreign rotation=(rotation)

    // Gets this camera's x_on_screen
	foreign x_on_screen

    // Sets this camera's x_on_screen
    foreign x_on_screen=(x_on_screen)

    // Gets this camera's y_on_screen
	foreign y_on_screen

    // Sets this camera's y_on_screen
    foreign y_on_screen=(y_on_screen)

    // Gets this camera's w_on_screen
	foreign w_on_screen

    // Sets this camera's w_on_screen
    foreign w_on_screen=(w_on_screen)

    // Gets this camera's h_on_screen
	foreign h_on_screen

    // Sets this camera's h_on_screen
    foreign h_on_screen=(h_on_screen)

    foreign eyes
    foreign eyes=(s)
    foreign centre
    foreign centre=(s)
    foreign up
    foreign up=(s)
    foreign fov
    foreign fov=(s)

    // Applies current settings for the camera
    foreign update()
}

// Shader support
foreign class Shader {
    // Loads a shader
    construct load(vertex_file, frag_file, buffer_size) {}

    // Sets the data for a shader to a buffer
    foreign data=(data)
}