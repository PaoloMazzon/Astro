---
layout: default
title: Sprite
parent: API Documentation
---

## Sprite
Sprites are small animations that also have the convenience of saving render settings. For
example, with a texture you need to set the x-scale/y-scale, origin, rotation, etc every
time you render it but a sprite saves this information. The majority of things in your game
world will be drawn with sprites.

 + [new()](#new)
 + [from()](#from)
 + [free()](#free)
 + [copy()](#copy)
 + [frame](#frame)
 + [frame_count](#frame_count)
 + [delay](#delay)
 + [origin_x](#origin_x)
 + [origin_y](#origin_y)
 + [scale_x](#scale_x)
 + [scale_y](#scale_y)
 + [rotation](#rotation)
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
 + `texture -> Texture` or `Surface` Texture to create the sprite from.
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

{: .note }
Because objects are references in Wren, you will generally want to use this for each
new object that uses a sprite in your game world. If you do not, modifying the one sprite
that everything uses will result in that transformation being shown every time it is
drawn.

## frame
`frame=(frame)` `frame`

Variable Type: `Num` - Index of the frame to go to.

if `frame` is -1, it is set to the last frame in the animation.

## frame_count
Read Only: `frame_count`

Variable Type: `Num` - Returns the number of frames in the animation.

## delay
`delay=` `delay`

Variable Type: `Num` - Delay between each frame in seconds.

## origin_x
`origin_x=` `origin_x`

Variable Type: `Num` - X origin of the sprite for rotation.

## origin_y
`origin_y=` `origin_y`

Variable Type: `Num` - Y origin of the sprite for rotation.

## scale_x
`scale_x=` `scale_x`

Variable Type: `Num` - X scale to set for this sprite.

## scale_y
`scale_y=` `scale_y`

Variable Type: `Num` - Y scale to set for this sprite.

## rotation
`rotation=` `rotation`

Variable Type: `Num` - Rotation for the sprite.

## width
Read Only: `width`

Variable Type: `Num` - Returns the sprite's width.

## height
Read Only: `height`

Variable Type: `Num` - Returns the sprite's height.

