---
layout: default
title: Camera
parent: API Documentation
---

## Camera
Cameras are a powerful interface for controlling what is rendered on screen and where.
Cameras allow you to display specific portions of the game world onto the screen any
way you like. For example, you may showcase a 300x200 part of the game world at position
(35, 20) to a window that 1000x700. There are no restrictions on the part of the world
you wish to view and the portions of the window you wish to render to. Since every camera
is drawn to unless told otherwise (see: [lock_cameras](Renderer#lock_cameras)), you can
do things like 4-player split screen, or have the first default camera for the UI and next
4 for split screen, or minimaps, or whatever.

 + [new()](#new)
 + [type](#type)
 + [x](#x)
 + [y](#y)
 + [width](#width)
 + [height](#height)
 + [zoom](#zoom)
 + [rotation](#rotation)
 + [x_on_screen](#x_on_screen)
 + [y_on_screen](#y_on_screen)
 + [w_on_screen](#w_on_screen)
 + [h_on_screen](#h_on_screen)
 + [eyes](#eyes)
 + [centre](#centre)
 + [up](#up)
 + [fov](#fov)
 + [update()](#update)

Additionally, cameras are all one of these class-defined types:

 + `CAMERA_TYPE_DEFAULT` - 2D camera
 + `CAMERA_TYPE_PERSPECTIVE` - Perspective camera for 3D rendering
 + `CAMERA_TYPE_ORTHOGRAPHIC` - Orthographic camera for 3D rendering

### new
`construct new()`

Creates a new camera, the new camera is a 1:1 copy of the default camera, meaning its the size
of the window.

### type
`type=(type)` `type`

Variable Type: `Num` - Should be one of the `CAMERA_TYPE_*` constants defined above.

### x (setter)
`x=(x)` `x`

Variable Type: `Num` - X position in the game world.

### y
`y=(y)` `y`

Variable Type: `Num` - Y position in the game world.

### width
`width=(w)` `width`

Variable Type: `Num` - Width of the camera.

### height
`height=(h)` `height`

Variable Type: `Num` - Height of the camera.

### zoom
`zoom=(zoom)` `zoom`

Variable Type: `Num` - Zoom multiplier for the camera (default 1).

### rotation
`rotation=(rotation)` `rotation`

Variable Type: `Num` - Camera's rotation in radians.

### x_on_screen
`x_on_screen=(x_on_screen)` `x_on_screen`

Variable Type: `Num` - X position in the window the camera should be displayed.

### y_on_screen
`y_on_screen=(y_on_screen)` `y_on_screen`

Variable Type: `Num` - Y position in the window the camera should be displayed.

### w_on_screen
`w_on_screen=(w_on_screen)` `w_on_screen`

Variable Type: `Num` - Width on screen for the camera.

### h_on_screen
`h_on_screen=(h_on_screen)` `h_on_screen`

Variable Type: `Num` - Height on screen for the camera.

### eyes
`eyes=(eyes)` `eyes`

Variable Type: `List` - A list containing three elements specifying the x, y, and z components of the eyes vector.

Represents where the camera is in 3D space. Take the following example:

```python
camera.up = [0, 1, 0]
camera.eyes = [2, 1, 0]
camera.centre = [0, 0, 0]
```

This would make the 3D camera look towards the origin from two units away and 1 unit
above.

### centre
`centre=(centre)` `centre`

Variable Type: `List` - A list containing three elements specifying the x, y, and z components of the centre vector.

Controls what the camera is looking at in 3D space.

### up
`up=(up)` `up`

Variable Type: `List` - A list containing three elements specifying the x, y, and z components of the up vector.

Represents which direction is up in your 3D space. For example, Minecraft uses the y
axis as up, so for coordinates similar to that you might want `camera.up = [0, 1, 0]`.

### fov
`fov=(fov)` `fov`

Variable Type: `Num` - Field of view in radians.

Controls how much of the 3D space the user can see at once. `Num.pi / 2` is generally
a decent value to start with.

### update
`update()`

Applies whatever changes you made to this camera, you may call this whenever but the effects
will only be visible on the start of the next frame.

