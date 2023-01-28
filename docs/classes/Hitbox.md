## Hitbox
Allows for checking for collisions between two hitboxes and tilesets.

 + [type](#type)
 + [r](#r)
 + [w](#w)
 + [h](#h)
 + [new_circle](#new_circle)
 + [new_rectangle](#new_rectangle)
 + [collision](#collision)
 + [bounding_box](#bounding_box)
 
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
 + `hitbox2 -> lib/Util::Hitbox` The second hitbox to check for collisions against.

Checks for a collision between two hitboxes, returning true if a collision has occurred.

### bounding_box
`bounding_box(x, y)`

Parameters
 + `x -> Num` X position of the hitbox.
 + `y -> Num` Y position of the hitbox.
 
Returns a list containing the bounding box coordinates of the hitbox. The list is in the form
`[x1, y1, x2, y2]` where `(x1, y1)` is the top-left corner of the bounding box and `(x2, y2)`
is the bottom-right of the bounding box.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)