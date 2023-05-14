---
layout: default
title: Tileset
parent: API Documentation
---

## Tileset
Tilesets are a tool for managing large amounts of static objects that all have uniform
width and height. Typically you would use a tileset (or several because they're so light)
to represent the static walls or similar objects in your game. Drawing the tileset is
somewhat expensive, however, so you will often want to draw the tileset to a [Surface](Surface)
and just render that to offset the cost. Collisions on the other hand are extremely light
weight regardless of how big the tileset is.

 + [new()](#new)
 + [add_tileset()](#add_tileset)
 + [width](#width)
 + [height](#height)
 + [tiles_wide](#tiles_wide)
 + [tiles_tall](#tiles_tall)
 + [tile_width](#tile_width)
 + [tile_height](#tile_height)
 + [[x, y]](#x-y)
 + [snap_left()](#snap_left)
 + [snap_right()](#snap_right)
 + [snap_up()](#snap_up)
 + [snap_down()](#snap_down)
 + [collision()](#collision)
 + [draw()](#draw)
 + [draw_to_surface()](#draw_to_surface)
 + [draw_tiling_background()](#draw_tiling_background)

### new
`construct new(tileset, texture, w, h)`

Parameters
 + `tileset -> List` List of all indices in the tileset. See below.
 + `texture -> Texture` or `Surface` Texture that will be treated as a spritesheet and indexed into.
 + `w -> Num` Width of each cell in the texture
 + `h -> Num` Height of each cell in the texture

Creates a tileset. The list that is provided to this method should be a list of lists
representing a 2D grid of indices in the tileset. Each "cell" of the 2D list represents
an index for the tileset + 1 (if you want the first cell in the tileset you would put 1
and not 0). For example, you might give it a list like this:

    var tileset = [
        [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
        [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
        [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
        [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
        [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
        [00, 00, 00, 00, 00, 29, 30, 30, 31, 00, 00, 00],
        [00, 00, 00, 00, 00, 29, 30, 30, 31, 00, 00, 00],
        [07, 08, 08, 08, 08, 30, 30, 30, 30, 08, 08, 09],
        [51, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 53]
    ]

This would create a 12x9 tileset, where the top-left of that grid is (0, 0) in the tileset and
the bottom-right is at position (11, 8). Each non-zero value represents a cell - 1.

### add_tileset
`add_tileset(texture, gid)`

Parameters
 + `texture -> Texture` or `Surface` Texture for the tileset.
 + `gid -> Num` What number to start indexing this tileset at.

Adds a new tileset to the tileset, using the GID to differentiate between textures.
When you create a tileset, the texture you provide automatically has GID 1. This means all
indices in the grid 1 or higher are treated as frames for that tileset - 1. If you introduce
another tileset at GID 100, anytime you have a cell with value 100 or higher, it will be drawn
from the new tileset with frame cell - 100. This is primarily for Tiled map loading.

### width
Read Only: `width`

Variable Type: `Num` - Total width of the tileset in pixels.

### height
Read Only: `height`

Variable Type: `Num` - Total height of the tileset in pixels.

### tiles_wide
Read Only: `tiles_wide`

Variable Type: `Num` - Width of the tileset in tiles.

### tiles_tall
Read Only: `tiles_tall`

Variable Type: `Num` - Height of the tileset in tiles.

### tile_width
Read Only: `tile_width`

Variable Type: `Num` - Width of a tile in the tileset.

### tile_height
Read Only: `tile_height`

Variable Type: `Num` - Height of a tile in the tileset.

### [x, y]
`[x, y]` `[x, y]=`

Variable Type: `Num` - Cell value located at `(x, y)`.

{: .note }
This returns 0 if you attempt to get a cell value out of bounds, and will
do nothing if you attempt to set a cell out of bounds.

### snap_left
`snap_left(hitbox, x, y)`

Parameters
 + `hitbox -> Hitbox` Hitbox to use in snapping calculations.
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Snaps a hitbox's x coordinate to the nearest colliding space on the left. The x
resulting snapped x-position is returned.

### snap_right
`snap_right(hitbox, x, y)`

Parameters
 + `hitbox -> Hitbox` Hitbox to use in snapping calculations.
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Snaps a hitbox's x coordinate to the nearest colliding space on the right. The x
resulting snapped x-position is returned.

### snap_up
`snap_up(hitbox, x, y)`

Parameters
 + `hitbox -> Hitbox` Hitbox to use in snapping calculations.
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Snaps a hitbox's y coordinate to the nearest colliding space on the top. The y
resulting snapped y-position is returned.

### snap_down
`snap_down(hitbox, x, y)`

Parameters
 + `hitbox -> Hitbox` Hitbox to use in snapping calculations.
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Snaps a hitbox's y coordinate to the nearest colliding space on the bottom. The y
resulting snapped y-position is returned.


### collision
`collision(hitbox, x, y)`

Parameters
 + `hitbox -> Hitbox` Hitbox to check a collision against.
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Returns true if any of the hitbox's bounding box intersect with a tile
that has a non-zero value.

### draw
`draw()`
`draw(x, y, w, h)`

Parameters
 + `x -> Num` If specified, the x position in the tileset to begin drawing at
 + `y -> Num` If specified, the y position in the tileset to begin drawing at 
 + `w -> Num` If specified, the width of the region to draw
 + `h -> Num` If specified, the height of the region to draw

Draws either the entire tileset or a specified region in the tileset to (0,0) on
screen.

### draw_to_surface
`draw_to_surface()`
`draw_to_surface(x, y, w, h)`

Parameters
 + `x -> Num` If specified, the x position in the tileset to begin drawing at
 + `y -> Num` If specified, the y position in the tileset to begin drawing at 
 + `w -> Num` If specified, the width of the region to draw
 + `h -> Num` If specified, the height of the region to draw

Draws the entire tileset to a surface if no arguments are specified. If a region is
provided, a surface only big enough for that region is created and that region is
drawn to it. In either case, the surface's background will be transparent.

### draw_tiling_background
`static draw_tiling_background(texture, parallax, camera)`

Parameters
+ `texture -> Texture` or `Surface` Texture to use as the tiling background.
+ `parallax -> Num` Parallax amount from 0-1, where 0 represents a static background and 1 means a background that follows the camera exactly.
+ `camera -> Camera` or `null` Camera to use for parallax measurements or `null` for no camera.

Draws a background tiling to cover the entire camera and with a parallax value. If you specify
a `parallax` value other that 0 or 1 you can create effects where it appears that the background
is far away from the camera. 

