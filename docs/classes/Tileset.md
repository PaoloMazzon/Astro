## Tileset
Tilesets are a tool for managing large amounts of static objects that all have uniform
width and height. Typically you would use a tileset (or several because they're so light)
to represent the static walls or similar objects in your game. Drawing the tileset is
somewhat expensive, however, so you will often want to draw the tileset to a [Surface](Surface.md)
and just render that to offset the cost. Collisions on the other hand are extremely light
weight regardless of how big the tileset is.

 + [new](#new)
 + [add_tileset](#add_tileset)
 + [width](#width)
 + [height](#height)
 + [tile_width](#tile_width)
 + [tile_height](#tile_height)
 + [[x, y]](#x-y-getter)
 + [[x, y]=](#x-y-setter)
 + [collision](#collision)
 + [draw](#draw)
 + [draw_tiling_background](#draw_tiling_background)

### new
`construct new(tileset, texture, w, h)`

Parameters
 + `tileset -> List` List of all indices in the tileset. See below.
 + `texture -> lib/Drawing::Texture` or `lib/Drawing::Surface` Texture that will be treated as a spritesheet and indexed into.
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
 + `texture -> lib/Drawing::Texture` or `lib/Drawing::Surface` Texture for the tileset.
 + `gid -> Num` What number to start indexing this tileset at.

Adds a new tileset to the tileset, using the GID to differentiate between textures.
When you create a tileset, the texture you provide automatically has GID 1. This means all
indices in the grid 1 or higher are treated as frames for that tileset - 1. If you introduce
another tileset at GID 100, anytime you have a cell with value 100 or higher, it will be drawn
from the new tileset with frame cell - 100. This is primarily for Tiled map loading.

### width
`width`

Returns the total width of the tileset in pixels.

### height
`height`

Returns the total height of the tileset in pixels.

### tile_width
`tile_width`

Returns the width of the tileset in tiles.

### tile_height
`tile_height`

Returns the height of the tileset in tiles.

### [x, y] (getter)
`[x, y]`

Returns the cell value located at `(x, y)`.

> 📝 This returns 0 if you attempt to get a cell value out of bounds.

### [x, y]= (setter)
`[x, y]=(cell)`

Parameters
 + `cell -> Num` Sprite index + 1 to set at this location.

Sets the cell's value at `(x, y)`.

> 📝 This does nothing if you attempt to set a cell value out of bounds.

### collision
`collision(hitbox, x, y)`

Parameters
 + `hitbox -> lib/Util::Hitbox` Hitbox to check a collision against.
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Returns true if any of the hitbox's bounding box corners intersect with a tile
that has a non-zero value.

> 📝 Because this only checks the hitbox's bounding box corners, if the hitbox has a bounding
> box larger than the size of a cell this method can visually miss. In general you either want
> to design your worlds around this limitation or simply make your tiles bigger/hitboxes smaller.

### draw
`draw()`

Draws the entire tileset.

### draw_tiling_background
`static draw_tiling_background(texture, parallax, camera)`

Parameters
+ `texture -> lib/Drawing::Texture` or `lib/Drawing::Surface` Texture to use as the tiling background.
+ `parallax -> Num` Parallax amount from 0-1, where 0 represents a static background and 1 means a background that follows the camera exactly.
+ `camera -> lib/Renderer::Camera` or `null` Camera to use for parallax measurements or `null` for no camera.

Draws a background tiling to cover the entire camera and with a parallax value. If you specify
a `parallax` value other that 0 or 1 you can create effects where it appears that the background
is far away from the camera. 

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)