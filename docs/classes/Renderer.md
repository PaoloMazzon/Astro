---
layout: default
title: Renderer
parent: API Documentation
---

## Renderer
The `Renderer` class contains methods that are used in drawing and managing the display.

 + [draw_circle()](#draw_circle)
 + [draw_texture()](#draw_texture)
 + [draw_texture_part()](#draw_texture_part)
 + [config](#config)
 + [target](#target)
 + [blend_mode](#blend_mode)
 + [colour_mod](#colour_mod)
 + [shader](#shader)
 + [set_window_size()](#set_window_size)
 + [window_width](#window_width)
 + [window_height](#window_height)
 + [fullscreen](#fullscreen)
 + [set_texture_camera](#use_cameras_on_surfaces)
 + [average_frame_time](#average_frame_time)
 + [lock_cameras()](#lock_cameras)
 + [unlock_cameras()](#unlock_cameras)
 + [clear()](#clear)
 + [clear_blank()](#clear_blank)
 + [draw_rectangle()](#draw_rectangle)
 + [draw_rectangle_outline()](#draw_rectangle_outline)
 + [draw_circle_outline()](#draw_circle_outline)
 + [draw_line()](#draw_line)
 + [draw_polygon()](#draw_polygon)
 + [draw_model()](#draw_model)
 + [draw_font()](#draw_font)
 + [draw_font_wrapped()](#draw_font_wrapped)
 + [draw_font_ext()](#draw_font_ext)
 + [draw_font_ext_wrapped()](#draw_font_ext_wrapped)
 + [draw_sprite()](#draw_sprite)
 + [draw_polygon()](#draw_polygon)
 + [draw_model()](#draw_model)
 
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
 + `texture -> Texture or Surface` Texture to draw
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
 + `texture -> Texture or Surface` Texture to draw
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

## config
`static config=(config)` `static config`

Variable Type: `Map` - Renderer settings to use.

You may set or get a number of different renderer configuration settings through
this variable. Namely,

+ `"window_title"` - A string with the window's new title.
+ `"window_width"` - Width of the window in pixels .
+ `"window_height"` - Height of the window in pixels.
+ `"fullscreen"` - Whether or not the window is fullscreen.
+ `"msaa"` - Desired MSAA setting, will be an `MSAA_*` value.
+ `"screen_mode"` - Desired screen mode setting, will be a `SCREEN_MODE_*` value.
+ `"filter_type"` - Desired filter type, will be a `FILTER_TYPE_*` value.

## target
Write only: `static target=(target)`

Variable Type: `Surface` - Surface to set as the new render target.

Changes the render target to `target`, specify `RENDER_TARGET_DEFAULT` to draw to the
window instead.

## blend_mode
`static blend_mode=(blend_mode)` `static blend_mode` 

Variable Type: `Num` - A `BLEND_MODE_*` value.

The renderer's current blend mode.

## colour_mod
`static colour_mod=(colour)` `static colour_mod`

Variable Type: `List or String` - A list of `[red, green, blue, alpha]` values from 0-1, or a string containing a hex colour.
 
The renderer's colour modifier. You may set it with a hex colour, but only a list of
R, G, B, A normalized values will be returned by `colour_mod`.

```javascript
Renderer.colour_mod = "#ff0000"
System.print(Renderer.colour_mod) // [1, 0, 0, 1]
```

## use_cameras_on_surfaces
Write only: `static use_cameras_on_surfaces=(enable)`

Variable Type: `Bool` - Whether or not to use cameras when rendering to surfaces.

If enabled, when rendering to surfaces, the current renderer's camera will be used for
vertex transformations. This is useful when rendering the game world to a surface. By default
this is disabled which means when rendering to surfaces, no cameras are applied and
for example, a coordinate of (400, 200) is relative to the top-left of the surface like you
would see in an image editor.

## average_frame_time
Read only: `static average_frame_time`

Variable Type: `Num`

Returns the average time it takes to render a frame, slightly different from the engine's
average frame time.

## lock_cameras
`static lock_cameras(camera)`

Parameters
 + `camera -> Camera` Camera to lock rendering to.
 
By default, everytime you draw something it is drawn to every camera which allows you to do
something like a minimap or split screen, but if you lock the camera with this method, you
tell the renderer to only draw to the specified camera. This is often useful when using one camera
for the UI and another for game world.

## unlock_cameras
`static unlock_cameras()`

Undoes the effects of [lock_cameras](#lock_cameras).

## set_window_size
`static set_window_size(w, h)`

Parameters
 + `w -> Num` Width of the window.
 + `h -> Num` Height of the window.

Sets the size of the window in pixels.

## window_width
Read only: `static window_width`

Variable Type: `Num`

Returns the window's width in pixels.

## window_height
Read only: `static window_height`

Variable Type: `Num`

Returns the window's height in pixels.

## fullscreen
`static fullscreen=(fullscreen)` `static fullscreen`

Variable Type: `Bool` - Whether or not the window should be fullscreen.

Controls whether or not the window is fullscreen.

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
Write only: `static shader=(shader)`

Parameters
 + `shader -> Shader` Shader to use for texture rendering.
 
Sets the shader that will be used for all further texture rendering (including
sprites). Set `shader` to `null` to use the default texture shader.

{: .note }
It is an error to bind and use a shader without providing a buffer to the shader
if it expects one.

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
 + `font -> BitmapFont` Font to draw.
 + `text -> String` Text to render with the font.
 + `x -> Num` X position to draw to.
 + `y -> Num` Y position to draw to.

Draws a font. 

## draw_font_wrapped
`static draw_font_wrapped(font, text, x, y, w)`

Parameters
 + `font -> BitmapFont` Font to draw.
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
 + `font -> BitmapFont` Font to draw.
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

## draw_font_ext_wrapped
`static draw_font_ext_wrapped(font, text, x, y, w)`

Parameters
 + `font -> BitmapFont` Font to draw.
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
 + `sprite -> Sprite` Sprite to draw.
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

Draws a polygon. See [Polygon](Polygon).

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

Draws a model. See [Model](Model).

