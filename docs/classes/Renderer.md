## Renderer
The `Renderer` class contains methods that are used in drawing and managing the display.

 + [draw_circle](#draw_circle)
 + [draw_texture](#draw_texture)
 + [draw_texture_part](#draw_texture_part)
 + [get_config](#get_config)
 + [set_config](#set_config)
 + [set_target](#set_target)
 + [set_blend_mode](#set_blend_mode)
 + [get_blend_mode](#get_blend_mode)
 + [set_colour_mod](#set_colour_mod)
 + [get_colour_mod](#get_colour_mod)
 + [set_texture_camera](#set_texture_camera)
 + [average_frame_time](#average_frame_time)
 + [lock_cameras](#lock_cameras)
 + [unlock_cameras](#unlock_cameras)
 + [clear](#clear)
 + [draw_rectangle](#draw_rectangle)
 + [draw_rectangle_outline](#draw_rectangle_outline)
 + [draw_circle_outline](#draw_circle_outline)
 + [draw_line](#draw_line)
 + [draw_shader](#draw_shader)
 + [draw_polygon](#draw_polygon)
 + [draw_model](#draw_model)
 + [draw_font](#draw_font)
 + [draw_sprite](#draw_sprite)
 
As well as the above methods, the renderer also contains a number of values that some of its
methods expect as parameters:

 + `MSAA_1X`
 + `MSAA_2X`
 + `MSAA_4X`
 + `MSAA_8X`
 + `MSAA_16X`
 + `MSAA_32X`
 + `SCREEN_MODE_IMMEDIATE`
 + `SCREEN_MODE_VSYNC`
 + `SCREEN_MODE_TRIPLE_BUFFER`
 + `FILTER_TYPE_NEAREST`
 + `FILTER_TYPE_LINEAR`
 + `RENDER_TARGET_DEFAULT`
 + `DEFAULT_CAMERA`
 + `BLEND_MODE_BLEND`
 + `BLEND_MODE_NONE`
 + `BLEND_MODE_ADD`
 + `BLEND_MODE_SUBTRACT`
 + `COLOUR_RED`
 + `COLOUR_GREEN`
 + `COLOUR_BLUE`
 + `COLOUR_YELLOW`
 + `COLOUR_CYAN`
 + `COLOUR_PURPLE`
 + `COLOUR_BLACK`
 + `COLOUR_WHITE`
 + `COLOUR_DEFAULT`

## draw_circle
`static draw_circle(x, y, radius)`

Parameters
 + `x -> Num` X of the circle to draw.
 + `y -> Num` Y of the circle to draw.
 + `radius -> Num` Radius of the circle to draw.

Draws a circle using the current draw colour.

## draw_texture
`static draw_texture(texture, x, y)`

`static draw_texture(texture, x, y, x_scale, y_scale, rotation, x_origin, y_origin)`

Parameters
 + `texture -> lib/Drawing::Texture or lib/Drawing::Surface` Texture to draw
 + `x -> Num` X position to draw the texture at.
 + `y -> Num` Y position to draw the texture at.
 + `x_scale -> Num` If specified, the x scale of the texture.
 + `y_scale -> Num` If specified, the y scale of the texture.
 + `rotation -> Num` If specified, the rotation of the texture.
 + `x_origin -> Num` If specified, the x origin of the texture for rotation.
 + `y_origin -> Num` If specified, the y origin of the texture for rotation.

Draws a texture.

## draw_texture_part
`static draw_texture_part(texture, x, y, x_in_tex, y_in_tex, width, height)`

`static draw_texture_part(texture, x, y, x_in_tex, y_in_tex, width, height, x_scale, y_scale, rotation, x_origin, y_origin)`

Parameters
 + `texture -> lib/Drawing::Texture or lib/Drawing::Surface` Texture to draw
 + `x -> Num` X position to draw the texture at.
 + `y -> Num` Y position to draw the texture at.
 + `x_in_tex -> Num` X position of the part to draw in the texture.
 + `y_in_tex -> Num` Y position of the part to draw in the texture.
 + `width -> Num` Width of the portion of the texture to draw.
 + `height -> Num` Height of the portion of the texture to draw.
 + `x_scale -> Num` If specified, the x scale of the texture.
 + `y_scale -> Num` If specified, the y scale of the texture.
 + `rotation -> Num` If specified, the rotation of the texture.
 + `x_origin -> Num` If specified, the x origin of the texture for rotation.
 + `y_origin -> Num` If specified, the y origin of the texture for rotation.

Draws a specific portion of a texture.

## get_config
`static get_config()`

Returns a map containing the renderer's current configuration. This map will have the same
keys as the map you pass in `init.wren`. For convenience, those keys are as follows:

+ `"window_title"` - A string with the window's current title.
+ `"window_width"` - Width of the window in pixels .
+ `"window_height"` - Height of the window in pixels.
+ `"fullscreen"` - Whether or not the window is fullscreen.
+ `"msaa"` - Current MSAA setting, will be an `MSAA_*` value.
+ `"screen_mode"` - Current screen mode setting, will be a `SCREEN_MODE_*` value.
+ `"filter_type"` - Current filter type, will be a `FILTER_TYPE_*` value.

> 📝 This returns what the window is actually using, so for example if you requested
> 32x MSAA but the host system only supports up to 8x MSAA, this will return 
> MSAA_8X since that is what is actually being used.

## set_config
`static set_config(config)`

Parameters
 + `config -> Map` Renderer settings to use.

This method expects the same keys from the map you pass in `init.wren`. Specifically,
the map must have the following keys:

+ `"window_title"` - A string with the window's new title.
+ `"window_width"` - Width of the window in pixels .
+ `"window_height"` - Height of the window in pixels.
+ `"fullscreen"` - Whether or not the window is fullscreen.
+ `"msaa"` - Desired MSAA setting, will be an `MSAA_*` value.
+ `"screen_mode"` - Desired screen mode setting, will be a `SCREEN_MODE_*` value.
+ `"filter_type"` - Desired filter type, will be a `FILTER_TYPE_*` value.

## set_target
`static set_target(target)`

Parameters
 + `target -> lib/Drawing::Surface` Surface to set as the new render target.

Changes the render target to `target`, specify `RENDER_TARGET_DEFAULT` to draw to the
window instead.

## set_blend_mode
`static set_blend_mode(blend_mode)`

Parameters
 + `blend_mode -> Num` A `BLEND_MODE_*` value.

Changes the blend mode.

## get_blend_mode
`static get_blend_mode()`

Returns the current blend mode as a `BLEND_MODE_*` value.

## set_colour_mod
`static set_colour_mod(colour)`

Parameters
 + `colour -> List` A list of `[red, green, blue, alpha]` values from 0-1
 
Sets the colour mod for the renderer, this means all textures will have their colours
multiplied by this value and all shapes you draw will be this colour.

## get_colour_mod
`static get_colour_mod()`

Returns the current colour mod as a 4-element list of `[red, green, blue, alpha]` values
from 0-1.

## set_texture_camera
`static set_texture_camera(enable)`

Parameters
 + `enable -> Bool` Whether or not to use cameras when rendering to surfaces.

If enabled, when rendering to surfaces, the current renderer's camera will be used for
vertex transformations. This is useful when rendering the game world to a surface.

## average_frame_time
`static average_frame_time()`

Returns the average time it takes to render a frame, slightly different from the engine's
average frame time.

## lock_cameras
`static lock_cameras(camera)`

Parameters
 + `camera -> lib/Renderer::Camera` Camera to lock rendering to.
 
By default, everytime you draw something it is drawn to every camera which allows you to do
something like a minimap or split screen, but if you lock the camera with this method, you
tell the renderer to only draw to the specified camera. This is often useful when using one camera
for the UI and another for game world.

## unlock_cameras
`static unlock_cameras()`

Undoes the effects of [lock_cameras](#lock_cameras).

## clear
`static clear()`

Fills the render target with the current colour mod.

## draw_rectangle
`static draw_rectangle(x, y, w, h, rotation, x_origin, y_origin)`

Parameters
 + `x -> Num` X to draw the rectangle at.
 + `y -> Num` Y to draw the rectangle at.
 + `w -> Num` Width of the rectangle.
 + `h -> Num` Height of the rectangle.
 + `rotation -> Num` Rotation of the rectangle.
 + `x_origin -> Num` X origin of rotation.
 + `y_origin -> Num` Y origin of rotation.

Draws a rectangle to the render target.

## draw_rectangle_outline
`static draw_rectangle_outline(x, y, w, h, rotation, x_origin, y_origin, outline_thickness)`

Parameters
 + `x -> Num` X to draw the rectangle at.
 + `y -> Num` Y to draw the rectangle at.
 + `w -> Num` Width of the rectangle.
 + `h -> Num` Height of the rectangle.
 + `rotation -> Num` Rotation of the rectangle.
 + `x_origin -> Num` X origin of rotation.
 + `y_origin -> Num` Y origin of rotation.
 + `outline_thickness` Line thickness.

Draws a rectangle outline to the render target.

## draw_circle_outline
`static draw_circle_outline(x, y, r, outline_width)`

Parameters
 + `x -> Num` X of the circle to draw.
 + `y -> Num` Y of the circle to draw.
 + `radius -> Num` Radius of the circle to draw.
 + `outline_thickness` Line thickness.

Draws a circle outline using the current draw colour.

## draw_line
`static draw_line(x1, y1, x2, y2)`

Parameters
 + `x1 -> Num` First pair of coordinates for the line
 + `y1 -> Num` First pair of coordinates for the line
 + `x2 -> Num` Second pair of coordinates for the line
 + `y2 -> Num` Second pair of coordinates for the line

## draw_shader
`static draw_shader(shader, tex, x, y, x_scale, y_scale, rot, origin_x, origin_y, x_in_tex, y_in_tex, tex_width, tex_height)`

Not yet implemented.

## draw_polygon
`static draw_polygon(polygon, x, y, filled, line_width, x_scale, y_scale, rot, origin_x, origin_y)`

Not yet implemented.

## draw_model
`static draw_model(model, x, y, z, xscale, yscale, zscale, rot, axis, origin_x, origin_y, origin_z)`

Not yet implemented.

## draw_font
`static draw_font(font, text, x, y)`

Parameters
 + `font -> lib/Drawing::BitmapFont` Font to draw.
 + `text -> String` Text to render with the font.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.

Draws a font. 

## draw_sprite
`static draw_sprite(sprite, x, y)`

`static draw_sprite(sprite, frame, x, y)` 

Parameters
 + `sprite -> lib/Drawing::Sprite` Sprite to draw.
 + `frame -> Num` If specified, the specific frame of the sprite to draw.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.
 
Draws a sprite, automatically handling its animations. If you specify a frame to
draw, the animation will not be played out or manipulated.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)