## Sprite
Sprites are small animations that also have the convenience of saving render settings. For
example, with a texture you need to set the x-scale/y-scale, origin, rotation, etc every
time you render it but a sprite saves this information. The majority of things in your game
world will be drawn with sprites.

 + [new](#new)
 + [from](#from)
 + [free](#free)
 + [copy](#copy)
 + [frame](#frame-getter)
 + [frame=](#frame-setter)
 + [frame_count](#frame_count)
 + [delay](#delay-setter)
 + [delay=](#delay-setter)
 + [origin_x](#origin_x-setter)
 + [origin_x=](#origin_x-setter)
 + [origin_y](#origin_y-setter)
 + [origin_y=](#origin_y-setter)
 + [scale_x](#scale_x-setter)
 + [scale_x=](#scale_x-setter)
 + [scale_y](#scale_y-setter)
 + [scale_y=](#scale_y-setter)
 + [rotation](#rotation-setter)
 + [rotation=](#rotation-setter)
 + [width](#width)
 + [height](#height)

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

## from
`static from(texture, x, y, w, h, delay, frames)`

Parameters:
 + `texture -> lib/Drawing::Texture` or `lib/Drawing::Surface` Texture to create the sprite from.
 + `x -> Num` X position in the sprite sheet to begin pulling the animation from.
 + `y -> Num` Y position in the sprite sheet to begin pulling the animation from.
 + `w -> Num` Width of each frame in pixels.
 + `h -> Num` Height of each frame in pixels.
 + `delay -> Num` Delay between each rendered frame in seconds.
 + `frames -> Num` Number of frames in the animation.

Identical to [new](#new) except it uses an already loaded texture/surface instead of loading a new
one from a file.

## free
`free()`

Forces a sprite to be freed without waiting for the garbage collector.

## copy
`copy()`

Returns a copy of this sprite as a new sprite, although the new sprite just stores a
reference to the internal sprite sheet so you may not free the original sprite while still
using copies.

> ???? Because objects are references in Wren, you will generally want to use this for each
> new object that uses a sprite in your game world. If you do not, modifying the one sprite
> that everything uses will result in that transformation being shown every time it is
> drawn.

## frame (Getter)
`frame`

Returns the current frame in the sprite's animations.

## frame= (Setter)
`frame=(frame)`

Parameters
 + `frame -> Num` Index of the frame to go to.
 
Sets the current animation frame. If `frame` is out of bounds, the frame is set to 0,
if `frame` is -1, it is set to the last frame in the animation.

## frame_count

Returns the number of frames in the animation.

## delay (getter)
`delay`

Returns the sprite's delay.

## delay (setter)
`delay=`

Parameters
 + `delay -> Num` Delay between each frame in seconds.

Sets the sprite's delay.

## origin_x (getter)
`origin_x`

Returns a sprite's x origin.

## origin_x (setter)
`origin_x=`

Parameters
 + `origin_x -> Num` X origin of the sprite for rotation.

Sets a sprite's x origin.

## origin_y (getter)
`origin_y`

Returns a sprite's y origin.

## origin_y (setter)
`origin_y=`

Parameters
 + `origin_y -> Num` Y origin of the sprite for rotation.

Sets a sprite's y origin.

## scale_x (getter)
`scale_x`

Returns a sprite's x scale.

## scale_x (setter)
`scale_x=`

Parameters
 + `scale_x -> Num` X scale to set for this sprite.
 
Sets a sprite's x scale.

## scale_y (getter)
`scale_y`

Returns a sprite's y scale.

## scale_y (setter)
`scale_y=`

Parameters
 + `scale_y -> Num` Y scale to set for this sprite.
 
Sets a sprite's y scale.

## rotation (getter)
`rotation`

Returns the sprite's rotation.

## rotation (setter)
`rotation=`

Parameters
 + `rotation -> Num` Rotation for the sprite.

Sets a sprite's rotation.

## width
`width`

Returns the sprite's width.

## height
`height`

Returns the sprite's height.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)