---
layout: default
title: Shadow
parent: API Documentation
---

## Shadow
Shadows are acquired from [add_shadow()](Lighting#add_shadow) and can be manipulated with
these methods.

+ [position=](position-setter)
+ [update()](update)
+ [enabled=](enabled-setter)
+ [enabled](enabled-getter)

## position (setter)
`position=(pos)`

Parameters
 + `pos -> List` List of x/y coordinates for this shadow.

Sets the position of this shadow.

## update(x, y, scale_x, scale_y, rotation, origin_x, origin_y)
`update(x, y, scale_x, scale_y, rotation, origin_x, origin_y)`

Parameters
 + `x -> Num` X position of the shadow.
 + `y -> Num` Y position of the shadow.
 + `scale_x -> Num` Horizontal scale of the shadow.
 + `scale_y -> Num` Vertical scale of the shadow.
 + `rotation -> Num` Rotation of the shadow in radians.
 + `origin_x -> Num` X origin of rotation.
 + `origin_y -> Num` Y origin of rotation.

Updates a shadow, this is a slightly more expensive method than the position setter so prefer
that unless this is necessary.

## enabled (setter)
`enabled=(enabled)`

Parameters
 + `enabled -> Bool` Whether or not this shadow object is enabled.

Allows you to enable/disable a shadow object for the purposes of hiding it without flushing
another VBO.

## enabled (getter)
`enabled`

Returns whether or not this shadow object is enabled.
