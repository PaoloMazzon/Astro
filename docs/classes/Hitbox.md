---
layout: default
title: Hitbox
parent: API Documentation
---

## Hitbox
Allows for checking for collisions between two hitboxes and tilesets.

 + [r](#r)
 + [w](#w)
 + [h](#h)
 + [no_hit](#no_hit)
 + [vertices](#vertices)
 + [x_offset](#x_offset)
 + [y_offset](#y_offset)
 + [new_circle()](#new_circle)
 + [new_rectangle()](#new_rectangle)
 + [new_polygon()](#new_polygon)
 + [NO_HIT](#NO_HIT)
 + [collision()](#collision)
 + [bounding_box()](#bounding_box)
 + [bb_left()](#bb_left)
 + [bb_right()](#bb_right)
 + [bb_top()](#bb_top)
 + [bb_bottom()](#bb_bottom)
 
### r
Read Only: `r`

Variable Type: `Num` - the hitbox's radius.

### w
Read Only: `w`

Variable Type: `Num` - the hitbox's width.

### h
Read Only: `h`

Variable Type: `Num` - the hitbox's height.

### no_hit
Read Only: `no_hit`

Variable Type: `Bool` - if this hitbox is capable of colliding.

New entities are created with void hitboxes that will always return false for any
collision check.

### vertices
Read Only: `vertices`

Variable Type: `List` - List of vertices if this is a polygonal hitbox.

### x_offset
`x_offset=(offset)` `offset_x`

Variable Type: `Num` - Horizontal offset.

Horizontal offset of the hitbox. This value is subtracted from any x value given
to another hitbox related method. This is primarily for coordinating with
a given sprite's origin.

### y_offset
`y_offset=(offset)` `offset_y`

Variable Type: `Num` - Vertical offset.

Vertical offset of the hitbox. This value is subtracted from any y value given
to another hitbox related method. This is primarily for coordinating with
a given sprite's origin.

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

### new_polygon
`construct new_polygon(vertices)`

Parameters
 + `vertices -> List` List of vertices for the polygon hitbox in the form `[[x, y], [x, y], ...]`.

Creates a new polygonal hitbox. The origin is at (0,0) relative to your vertex list. The polygon must
be convex and the vertex list does not need to connect, the first vertex is implied to be connected
to the last. The vertex list is stored locally in case the user calls [vertices](#vertices)

{: .note }
Vertical lines are treated as if they have a slope of 0.1. For all intents and purposes this is
unnoticable, but if you are doing something really up close with a high need for resolution you
may notice this limitation.

### NO_HIT
`construct NO_HIT`

Returns a new hitbox that is guaranteed to return false on all collision checks.
Its x/y offset and bounding boxes will additionally always return 0.

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

