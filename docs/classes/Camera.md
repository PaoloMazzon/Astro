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
is drawn to unless told otherwise (see: [lock_cameras](Renderer.md#lock_cameras)), you can
do things like 4-player split screen, or have the first default camera for the UI and next
4 for split screen, or minimaps, or whatever.

 + [new](#new)
 + [type](#type-getter)
 + [type=](#type-setter)
 + [x](#x-getter)
 + [x=](#x-setter)
 + [y](#y-getter)
 + [y=](#y-setter)
 + [width](#width-getter)
 + [width=](#width-setter)
 + [height](#height-getter)
 + [height=](#height-setter)
 + [zoom](#zoom-getter)
 + [zoom=](#zoom-setter)
 + [rotation](#rotation-getter)
 + [rotation=](#rotation-setter)
 + [x_on_screen](#x_on_screen-getter)
 + [x_on_screen=](#x_on_screen-setter)
 + [y_on_screen](#y_on_screen-getter)
 + [y_on_screen=](#y_on_screen-setter)
 + [w_on_screen](#w_on_screen-getter)
 + [w_on_screen=](#w_on_screen-setter)
 + [h_on_screen](#h_on_screen-getter)
 + [h_on_screen=](#h_on_screen-setter)
 + [eyes](#eyes-getter)
 + [eyes=](#eyes-setter)
 + [centre](#centre-getter)
 + [centre=](#centre-setter)
 + [up](#up-getter)
 + [up=](#up-setter)
 + [fov](#fov-getter)
 + [fov=](#fov-setter)
 + [update](#update)

Additionally, cameras are all one of these class-defined types:

 + `CAMERA_TYPE_DEFAULT` - 2D camera
 + `CAMERA_TYPE_PERSPECTIVE` - Perspective camera for 3D rendering
 + `CAMERA_TYPE_ORTHOGRAPHIC` - Orthographic camera for 3D rendering

### new
`construct new()`

Creates a new camera, the new camera is a 1:1 copy of the default camera, meaning its the size
of the window.

### type (getter)
`type`

Returns the camera's type, will be one of .

### type (setter)
`type=(type)`

Parameters
 + `type -> Num` Should be one of the `CAMERA_TYPE_*` constants defined above.

Sets the camera's type.

### x (getter)
`x`

Returns `x`.

### x (setter)
`x=(x)`

Parameters
 + `x -> Num` X position in the game world.
 
Sets the `x`.

### y (getter)
`y`

Returns `y`.

### y (setter)
`y=(y)`

Parameters
 + `y -> Num` Y position in the game world.
 
Sets the `y`.

### width (getter)
`width`

Returns `width`.

### width (setter)
`width=(w)`

Parameters
 + `width -> Num` Width of the camera.
 
Sets the `width`.

### height (getter)
`height`

Returns `height`.

### height (setter)
`height=(h)`

Parameters
 + `height -> Num` Height of the camera.
 
Sets the `height`.

### zoom (getter)
`zoom`

Returns `zoom`.

### zoom (setter)
`zoom=(zoom)`

Parameters
 + `zoom -> Num` Zoom multiplier for the camera (default 1).
 
Sets the `zoom`.

### rotation (getter)
`rotation`

Returns `rotation`.

### rotation (setter)
`rotation=(rotation)`

Parameters
 + `rotation -> Num` Camera's rotation in radians.
 
Sets the `rotation`.

### x_on_screen (getter)
`x_on_screen`

Returns `x_on_screen`.

### x_on_screen (setter)
`x_on_screen=(x_on_screen)`

Parameters
 + `x_on_screen -> Num` X position in the window the camera should be displayed.
 
Sets the `x_on_screen`.

### y_on_screen (getter)
`y_on_screen`

Returns `y_on_screen`.

### y_on_screen (setter)
`y_on_screen=(y_on_screen)`

Parameters
 + `y_on_screen -> Num` Y position in the window the camera should be displayed.
 
Sets the `y_on_screen`.

### w_on_screen (getter)
`w_on_screen`

Returns `w_on_screen`.

### w_on_screen (setter)
`w_on_screen=(w_on_screen)`

Parameters
 + `w_on_screen -> Num` Width on screen for the camera.
 
Sets the `w_on_screen`.

### h_on_screen (getter)
`h_on_screen`

Returns `h_on_screen`.

### h_on_screen (setter)
`h_on_screen=(h_on_screen)`

Parameters
 + `h_on_screen -> Num` Height on screen for the camera.
 
Sets the `h_on_screen`.

### eyes (Getter)
`eyes`

Returns the eyes vector as a list `[x, y, z]`.

### eyes= (Setter)
`eyes=(eyes)`

Parameters
 + `eyes -> List` A list containing three elements specifying the x, y, and z components of the eyes vector.

Sets the eyes vector for 3D rendering.

### centre (Getter)
`centre`

Returns the centre vector as a list `[x, y, z]`.

### centre= (Setter)
`centre=(centre)`

Parameters
 + `centre -> List` A list containing three elements specifying the x, y, and z components of the centre vector.

Sets the centre vector for 3D rendering.

### up (Getter)
`up`

Returns the up vector as a list `[x, y, z]`.

### up= (Setter)
`up=(up)`

Parameters
 + `up -> List` A list containing three elements specifying the x, y, and z components of the up vector.

Sets the up vector for 3D rendering.

### fov (Getter)
`fov`

Returns this camera's field of view in radians.

### fov= (Setter)
`fov=(fov)`

Parameters
 + `fov -> List` Field of view in radians.

Sets the fov vector for 3D rendering.

### update
`update()`

Applies whatever changes you made to this camera, you may call this whenever but the effects
will only be visible on the start of the next frame.

