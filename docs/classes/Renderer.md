## Renderer
The `Renderer` class contains methods that are used in drawing and managing the display.

 + [draw_circle](#draw_circle)
 + [draw_texture](#draw_texture)
 + [draw_texture_part](#draw_texture_part)
 + [config (Setter)](#config-setter)
 + [config (Getter)](#config-getter)
 + [target (Setter)](#target-setter)
 + [blend_mode (Setter)](#blend_mode-setter)
 + [blend_mode (Getter)](#blend_mode-getter)
 + [colour_mod (Setter)](#colour_mod-setter)
 + [colour_mod (Getter)](#colour_mod-getter)
 + [shader](#shader)
 + [set_texture_camera](#use_cameras_on_surfaces)
 + [average_frame_time](#average_frame_time)
 + [lock_cameras](#lock_cameras)
 + [unlock_cameras](#unlock_cameras)
 + [clear](#clear)
 + [clear_blank](#clear_blank)
 + [draw_rectangle](#draw_rectangle)
 + [draw_rectangle_outline](#draw_rectangle_outline)
 + [draw_circle_outline](#draw_circle_outline)
 + [draw_line](#draw_line)
 + [draw_polygon](#draw_polygon)
 + [draw_model](#draw_model)
 + [draw_font](#draw_font)
 + [draw_font_wrapped](#draw_font_wrapped)
 + [draw_font_ext](#draw_font_ext)
 + [draw_font_ext_wrapped](#draw_font_ext_wrapped)
 + [draw_sprite](#draw_sprite)
 + [draw_polygon](#draw_polygon)
 + [draw_model](#draw_model)
 
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

## config (getter)
`static config`

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

## config (setter)
`static config=(config)`

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

## target (setter)
`static target=(target)`

Parameters
 + `target -> lib/Drawing::Surface` Surface to set as the new render target.

Changes the render target to `target`, specify `RENDER_TARGET_DEFAULT` to draw to the
window instead. This value is write-only.

> 📝 Switching targets is a somewhat resource-intensive task that must be done a
> specific way. Anytime you switch the target to a surface you *MUST* switch the
> target back to the `RENDER_TARGET_DEFAULT` before switching to a new surface.

## blend_mode (setter)
`static blend_mode=(blend_mode)`

Parameters
 + `blend_mode -> Num` A `BLEND_MODE_*` value.

Changes the blend mode.

## blend_mode (getter)
`static blend_mode`

Returns the current blend mode as a `BLEND_MODE_*` value.

## colour_mod (setter)
`static colour_mod=(colour)`

Parameters
 + `colour -> List` A list of `[red, green, blue, alpha]` values from 0-1
 
Sets the colour mod for the renderer, this means all textures will have their colours
multiplied by this value and all shapes you draw will be this colour.

## colour_mod (getter)
`static colour_mod`

Returns the current colour mod as a 4-element list of `[red, green, blue, alpha]` values
from 0-1.

## use_cameras_on_surfaces
`static use_cameras_on_surfaces=(enable)`

Parameters
 + `enable -> Bool` Whether or not to use cameras when rendering to surfaces.

If enabled, when rendering to surfaces, the current renderer's camera will be used for
vertex transformations. This is useful when rendering the game world to a surface. By default
this is disabled which means when rendering to surfaces, no cameras are applied and
for example, a coordinate of (400, 200) is relative to the top-left of the surface like you
would see in an image editor.

## average_frame_time
`static average_frame_time`

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

## clear_blank
`static clear_blank()`

Fills the render target with no colour and no transparency. This is primarily for blanking out
new surfaces.

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

## shader
`static shader=(shader)`

Parameters
 + `shader -> lib/Renderer::Shader` Shader to use for texture rendering.
 
Sets the shader that will be used for all further texture rendering (including
sprites). Set `shader` to `null` to use the default texture shader.

> 📝 It is an error to bind and use a shader without providing a buffer to the shader
> if it expects one.

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

## draw_font
`static draw_font(font, text, x, y)`

Parameters
 + `font -> lib/Drawing::BitmapFont` Font to draw.
 + `text -> String` Text to render with the font.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.

Draws a font. 

## draw_font_wrapped
`static draw_font_wrapped(font, text, x, y, w)`

Parameters
 + `font -> lib/Drawing::BitmapFont` Font to draw.
 + `text -> String` Text to render with the font.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.
 + `w -> Num` Width in pixels to draw text before wrapping.

Draws a font, wrapping text after `w` pixels. For example, if the text would take
400 pixels horizontally to render but you specified a `w` of 300, only up to 300
pixels would be rendered horizontally before automatically moving to the next line. 

## draw_font_ext
`static draw_font_ext(font, text, x, y)`

Parameters
 + `font -> lib/Drawing::BitmapFont` Font to draw.
 + `text -> String` Text to render with the font.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.

Draws a font supporting tokens. 

You may use tokens in the text you render to alter how the text after the token is displayed.
This allows for more fine-tuned text rendering on-the-fly. Supported tokens are as follows:
 
 + `[#FF12E1]` - Sets the colour to specified hex colour
 + `[15, -15]` - Text will be displaced by a given x/y offset
 + `[~2]` - Text will be wavy, waving up to a number of pixels up and down
 + `[!2]` - Text will shake by up to a number of pixels
 + `[*]` - Colour modifiers will be ignored and text will be rainbow.
 + `[]` - Clears all active modifiers
 + `#[...]` - The leading # signals that this should be treated as text and not a modifier

## draw_font_wrapped_ext
`static draw_font_wrapped_ext(font, text, x, y, w)`

Parameters
 + `font -> lib/Drawing::BitmapFont` Font to draw.
 + `text -> String` Text to render with the font.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.
 + `w -> Num` Width in pixels to draw text before wrapping.

Draws a font, wrapping text after `w` pixels. For example, if the text would take
400 pixels horizontally to render but you specified a `w` of 300, only up to 300
pixels would be rendered horizontally before automatically moving to the next line. 

You may use tokens in the text you render to alter how the text after the token is displayed.
This allows for more fine-tuned text rendering on-the-fly. Supported tokens are as follows:
 
 + `[#FF12E1]` - Sets the colour to specified hex colour
 + `[15, -15]` - Text will be displaced by a given x/y offset
 + `[~2]` - Text will be wavy, waving up to a number of pixels up and down
 + `[!2]` - Text will shake by up to a number of pixels
 + `[*]` - Colour modifiers will be ignored and text will be rainbow.
 + `[]` - Clears all active modifiers
 + `#[...]` - The leading # signals that this should be treated as text and not a modifier

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

## draw_polygon
`static draw_polygon(polygon, x, y)`

`static draw_polygon(polygon, x, y, x_scale, y_scale, rotation, origin_x, origin_y, outline, line_width)`

Parameters
  + `polygon -> Polygon` Polygon to draw.
  + `x -> Num` X position to draw to.
  + `y -> Num` Y position to draw to.
  + `x_scale -> Num` X scale of the polygon.
  + `y_scale -> Num` Y scale of the polygon.
  + `rotation -> Num` Rotation of the polygon.
  + `origin_x -> Num` X origin for rotation.
  + `origin_y -> Num` Y origin for rotation.
  + `outline -> Bool` Whether or not to draw it only as an outline.
  + `line_width -> Num` If drawn as an outline, how thick should the outline be.

Draws a polygon. See [Polygon](Polygon.md).

## draw_model
`static draw_model(model, x, y, z)`

`static draw_model(model, x, y, z, x_scale, y_scale, z_scale, rotation, axis, origin_x, origin_y, origin_z)`

Parameters
 + `model -> Model` Model to draw.
 + `x -> Num` X position of the model.
 + `y -> Num` Y position of the model.
 + `z -> Num` Z position of the model.
 + `x_scale -> Num` X scale of the model.
 + `y_scale -> Num` Y scale of the model.
 + `z_scale -> Num` Z scale of the model.
 + `rotation -> Num` Rotation of the model.
 + `axis -> List` Axis to rotate on as a list of `[x, y, z]`.
 + `origin_x -> Num` X origin for rotation.
 + `origin_y -> Num` Y origin for rotation.
 + `origin_z -> Num` Z origin for rotation.

Draws a model. See [Model](Model.md).

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)