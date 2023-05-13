## Hitbox
Allows for checking for collisions between two hitboxes and tilesets.

 + [type](#type)
 + [r](#r)
 + [w](#w)
 + [h](#h)
 + [offset_x](#offset_x-getter)
 + [offset_x=](#offset_x-setter)
 + [offset_y](#offset_y-getter)
 + [offset_y=](#offset_y-setter)
 + [new_circle](#new_circle)
 + [new_rectangle](#new_rectangle)
 + [collision](#collision)
 + [bounding_box](#bounding_box)
 + [bb_left](#bb_left)
 + [bb_right](#bb_right)
 + [bb_top](#bb_top)
 + [bb_bottom](#bb_bottom)
 
The following values are also provided:

 + `TYPE_CIRCLE`
 + `TYPE_RECTANGLE`
 + `NO_HIT` This is a hitbox that is guaranteed to return false with any collision check.

### type
`type`

Returns the hitbox's type.

### r
`r`

Returns the hitbox's radius.

### w
`w`

Returns the hitbox's width.

### h
`h`

Returns the hitbox's height.

### offset_x (Getter)
`offset_x`

Returns the horizontal offset of the hitbox.

### offset_x= (Setter)
`offset_x=(offset)`

Parameters
 + `offset -> Num` Horizontal offset.

Sets the horizontal offset of the hitbox.

### offset_y (Getter)
`offset_y`

Returns the vertical offset of the hitbox.

### offset_y= (Setter)
`offset_y=(offset)`

Parameters
 + `offset -> Num` Vertical offset.

Sets the vertical offset of the hitbox.

### new_circle
`construct new_circle(r)`

Parameters
 + `r -> Num` Radius of the circle with a center origin.
 
Creates a new circular hitbox. This type has their origin at the center (for bounding
box and collision checks).

### new_rectangle
`construct new_rectangle(w, h)`

Parameters
 + `w -> Num` Width of the hitbox.
 + `h -> Num` Height of the hitbox.

Creates a new rectangular hitbox. This type has its origin at the top left corner (for bounding
box and collision checks).

### collision
`collision(x1, y1, x2, y2, hitbox2)`

Parameters
 + `x1 -> Num` X coordinate for the first hitbox.
 + `y1 -> Num` Y coordinate for the first hitbox.
 + `x2 -> Num` X coordinate for the second hitbox.
 + `y2 -> Num` Y coordinate for the second hitbox.
 + `hitbox2 -> Hitbox` The second hitbox to check for collisions against.

Checks for a collision between two hitboxes, returning true if a collision has occurred.
If either hitboxes are from `Hitbox.NO_HIT`, this method is guaranteed to return false.

### bounding_box
`bounding_box(x, y)`

Parameters
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.
 
Returns a list containing the bounding box coordinates of the hitbox. The list is in the form
`[x1, y1, x2, y2]` where `(x1, y1)` is the top-left corner of the bounding box and `(x2, y2)`
is the bottom-right of the bounding box.

### bb_left
`bb_left(x, y)`

Parameters
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.


Returns the left x coordinate of this hitbox's bounding box.

### bb_right
`bb_right(x, y)`

Parameters
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.
 
Returns the right x coordinate of this hitbox's bounding box.

### bb_top
`bb_top(x, y)`

Parameters
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.

Returns the top y coordinate of this hitbox's bounding box.

### bb_bottom
`bb_bottom(x, y)`

Parameters
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.
 
Returns the bottom y coordinate of this hitbox's bounding box.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)