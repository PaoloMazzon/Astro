---
layout: default
title: Mouse
parent: API Documentation
---

## Mouse
Easy access for the mouse.

 + [position](#position)
 + [left](#left)
 + [left_pressed](#left_pressed)
 + [left_released](#left_released)
 + [right](#right)
 + [right_pressed](#right_pressed)
 + [right_released](#right_released)
 + [middle](#middle)
 + [middle_pressed](#middle_pressed)
 + [middle_released](#middle_released)

### position
`static position(camera)`

Parameters
 + `camera -> Camera` or `null` Camera to check mouse relative to.
 
Returns the mouse position relative to a given camera, or `null` for the mouse position
relative to the window. The mouse position is returned as a list with the values `[x, y]`.

{: .note }
This method adjusts the mouse position to the camera to make the position relative to
the game world if you want, but it does not work properly with rotated cameras or cameras
that use `zoom` instead of `width/height`.

### left
`static left`

Returns true if the left mouse buttons is currently held down.

### left_pressed
`static left_pressed`

Returns true if the left mouse button was just pressed this frame.

### left_released
`static left_released`

Returns true if the left mouse button was just released this frame.

### right
`static right`

Returns true if the right mouse buttons is currently held down.

### right_pressed
`static right_pressed`

Returns true if the right mouse button was just pressed this frame.

### right_released
`static right_released`

Returns true if the right mouse button was just released this frame.

### middle
`static middle`

Returns true if the middle mouse buttons is currently held down.

### middle_pressed
`static middle_pressed`

Returns true if the middle mouse button was just pressed this frame.

### middle_released
`static middle_released`

Returns true if the middle mouse button was just released this frame.

