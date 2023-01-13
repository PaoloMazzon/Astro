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
    static COLOUR_RED { [255, 0, 0, 255] }
    static COLOUR_GREEN { [0, 255, 0, 255] }
    static COLOUR_BLUE { [0, 0, 255, 255] }
    static COLOUR_YELLOW { [255, 255, 0, 255] }
    static COLOUR_CYAN { [0, 255, 255, 255] }
    static COLOUR_PURPLE { [255, 0, 255, 255] }
    static COLOUR_BLACK { [0, 0, 0, 255] }
    static COLOUR_WHITE { [255, 255, 255, 255] }
    static COLOUR_DEFAULT { [255, 255, 255, 255] }
    
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
    foreign static get_config()

    // Sets the renderer's config using a provided map formatted in the same way as
    // the map you set in `init.wren`
    foreign static set_config(config)

    // Sets the target the renderer will draw to, can either be a Surface or
    // `Renderer.RENDER_TARGET_DEFAULT`
    foreign static set_target(target)

    // Sets the renderer's blend mode, must be one of the `Renderer.BLEND_MODE_*`
    // values
    foreign static set_blend_mode(blend_mode)

    // Returns the current blend mode, will be one of the `Renderer.BLEND_MODE_*`
    // values
    foreign static get_blend_mode()

    // Sets the render colour all things are drawn with, must be a 4-element
    // RGBA list of values in the range [0,255]. You may also use the
    // `Renderer.COLOUR_*` values
    foreign static set_colour_mod(colour)

    // Returns a list of 4 RGBA values from 0 to 255
    foreign static get_colour_mod()

    // If true, the renderer's cameras will be used when drawing to surfaces.
    // Otherwise everything drawn to a surface will be drawn from that surface's
    // origin in the top-left
    foreign static set_texture_camera(enable)

    // Returns the average frame time in seconds
    foreign static average_frame_time()

    // Locks the cameras so only a selected camera will be rendered to, may
    // be either a `Camera` instance or `Renderer.DEFAULT_CAMERA`
    foreign static lock_cameras(camera)

    // Unlocks the cameras so they will all be rendered to again
    foreign static unlock_cameras()

    // Set's the screen's viewport
    foreign static set_viewport(x, y, w, h)

    // Returns the current viewport as a 4-element list as [x, y, w, h]
    foreign static get_viewport()

    // Wipes the screen with the current rendering colour
    foreign static clear()

    // Draws a rectangle with the current render colour
    foreign static draw_rectangle(x, y, w, h, rotation, x_origin, y_origin)

    // Draws a rectangle outline with the current render colour
    foreign static draw_rectangle_outline(x, y, w, h, rotation, x_origin, y_origin, outline_thickness)

    // Draws a circle outline with the current render colour
    foreign static draw_circle_outline(x, y, r, outline_width)

    // Draws a line with the current render colour
    foreign static draw_line(x1, y1, x2, y2)

    // Draws a texture using a provided shader, tex MUST be `Texture` or `Surface`
    // and shader MUST be `Shader`
    foreign static draw_shader(shader, tex, x, y, x_scale, y_scale, rot, origin_x, origin_y, x_in_tex, y_in_tex, tex_width, tex_height)

    // Draws a polygon, polygon MUST be `Polygon`
    foreign static draw_polygon(polygon, x, y, filled, line_width, x_scale, y_scale, rot, origin_x, origin_y)

    // Draws a model, model MUST be `Model`, axis MUST be a 3-element list
    foreign static draw_model(model, x, y, z, xscale, yscale, zscale, rot, axis, origin_x, origin_y, origin_z)
}
