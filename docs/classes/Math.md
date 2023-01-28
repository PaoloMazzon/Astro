## Math
This is a core class that you are free to use before the engine is initialized.

Random math things that come up in game development. All parameters are simply `Num`s.

 + [round_to](#round_to)
 + [floor_to](#floor_to)
 + [ceil_to](#ceil_to)
 + [point_angle](#point_angle)
 + [point_distance](#point_distance)
 + [point_in_rectangle](#point_in_rectangle)
 + [point_in_circle](#point_in_circle)
 + [lerp](#lerp)
 + [serp](#serp)
 + [cast_x](#cast_x)
 + [cast_y](#cast_y)
 + [clamp](#clamp)

### round_to
`static round_to(x, y)`

Rounds `x` to the nearest `y` multiple.

### floor_to
`static floor_to(x, y)`

Floors `x` to the nearest `y` multiple.

### ceil_to
`static ceil_to(x, y)`

Raises `x` to the nearest `y` multiple.

### point_angle
`static point_angle(x1, y1, x2, y2)`

Returns the angle between two coordinate pairs in radians.

### point_distance
`static point_distance(x1, y1, x2, y2)`

Returns the distance between two points in pixels.

### point_in_rectangle
`static point_in_rectangle(x, y, w, h, px, py)`

Returns true if `(px, py)` is in the rectangle defined by `[x, y, w, h]`.

### point_in_circle
`static point_in_circle(x, y, r, px, py)`

Returns true if `(px, py)` is in the circle defined by `[x, y, r]`.

### lerp
`static lerp(percent, start, stop)`

Linear interpolation. Returns a value that is `percent`% between `start` and `stop`. For example,
`lerp(0.6, 0, 100)` would be 60.

### serp
`static serp(percent, start, stop)`

Similar to `lerp`, but it uses sin to smooth out the first and last bit of the output. This method
is only designed to be used for `percent` values between 0 and 1.

### cast_x
`static cast_x(length, angle)`

Projects an x coordinate `length` distance at `angle` radians. You might use `cast_x` and `cast_y`
to implement movement with direction and speed instead of x/y based movement.

### cast_y
`static cast_y(length, angle)`

Projects a y coordinate `length` distance at `angle` radians. You might use `cast_x` and `cast_y`
to implement movement with direction and speed instead of x/y based movement.

### clamp
`static clamp(x, min, max)`

If `x` is greater that `max`, `max` is returned. If `x` is less than `min`, `min` is returned. Otherwise, just
`x` is returned.
    
-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)