## Sprite
Sprites are small animations that also have the convenience of saving render settings. For
example, with a texture you need to set the x-scale/y-scale, origin, rotation, etc every
time you render it but a sprite saves this information. The majority of things in your game
world will be drawn with sprites.

 + [new](#new)
 + [free](#free)
 + [copy](#copy)
 + [get_delay](#get_delay)
 + [set_delay](#set_delay)
 + [get_origin_x](#get_origin_x)
 + [set_origin_x](#set_origin_x)
 + [get_origin_y](#get_origin_y)
 + [set_origin_y](#set_origin_y)
 + [get_scale_x](#get_scale_x)
 + [set_scale_x](#set_scale_x)
 + [get_scale_y](#get_scale_y)
 + [set_scale_y](#set_scale_y)
 + [get_rotation](#get_rotation)
 + [set_rotation](#set_rotation)
 + [get_width](#get_width)
 + [get_height](#get_height)

## new
`construct new(filename, x, y, w, h, delay, frames)`

Parameters
 + `filename -> String` Name of the file containing the sprite sheet this sprite will come from.
 + `x -> Num` X position in the sprite sheet to begin pulling the animation from.
 + `y -> Num` Y position in the sprite sheet to begin pulling the animation from.
 + `w -> Num` Width of each frame in pixels.
 + `h -> Num` Height of each frame in pixels.
 + `delay -> Num` Delay between each rendered frame in seconds.
 + `frames -> Num` Number of frames in the animation.

Creates a sprite from a given sprite sheet filename. The loader expects there to find at least
as many frames specified or this method will fail.

## free
`free()`

Forces a sprite to be freed without waiting for the garbage collector.

## copy
`copy()`

Returns a copy of this sprite as a new sprite, although the new sprite just stores a
reference to the internal sprite sheet so you may not free the original sprite while still
using copies.

> 📝 Because objects are references in Wren, you will generally want to use this for each
> new object that uses a sprite in your game world. If you do not, modifying the one sprite
> that everything uses will result in that transformation being shown every time it is
> drawn.

## get_delay
`delay`

Returns the sprite's delay.

## set_delay
`delay=`

Parameters
 + `delay -> Num` Delay between each frame in seconds.

Sets the sprite's delay.

## get_origin_x
`origin_x`

Returns a sprite's x origin.

## set_origin_x
`origin_x=`

Parameters
 + `origin_x -> Num` X origin of the sprite for rotation.

Sets a sprite's x origin.

## get_origin_y
`origin_y`

Returns a sprite's y origin.

## set_origin_y
`origin_y=`

Parameters
 + `origin_y -> Num` Y origin of the sprite for rotation.

Sets a sprite's y origin.

## get_scale_x
`scale_x`

Returns a sprite's x scale.

## set_scale_x
`scale_x=`

Parameters
 + `scale_x -> Num` X scale to set for this sprite.
 
Sets a sprite's x scale.

## get_scale_y
`scale_y`

Returns a sprite's y scale.

## set_scale_y
`scale_y=`

Parameters
 + `scale_y -> Num` Y scale to set for this sprite.
 
Sets a sprite's y scale.

## get_rotation
`rotation`

Returns the sprite's rotation.

## set_rotation
`rotation=`

Parameters
 + `rotation -> Num` Rotation for the sprite.

Sets a sprite's rotation.

## get_width
`width`

Returns the sprite's width.

## get_height
`height`

Returns the sprite's height.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)