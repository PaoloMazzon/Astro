---
layout: default
title: LightSource
parent: API Documentation
---

## LightSource
A light source from the [Lighting](#Lighting) system. You may control/update
any given light source at any time with these methods.

+ [position](#position)
+ [x](#x)
+ [y](#y)
+ [rotation](#rotation)
+ [origin_x](#origin_x)
+ [origin_y](#origin_y)
+ [scale_x](#scale_x)
+ [scale_y](#scale_y)
+ [colour](#colour)
+ [texture](#texture)
+ [delete()](#delete)

## position
`position=` `position`

Variable Type: `List` - X/Y position of the light in the form `[x,y]`.

## x
`x=` `x`

Variable Type: `Num` - X position.

## y
`y=` `y`

Variable Type: `Num` - Y position.

## rotation
`rotation=` `rotation`

Variable Type: `Num` - Rotation in radians.

## origin_x
`origin_x=` `origin_x`

Variable Type: `Num` - X origin of the light for rotation and position.

## origin_y
`origin_y=` `origin_y`

Variable Type: `Num` - Y origin of the light for rotation and position.

## scale_x
`scale_x=` `scale_x`

Variable Type: `Num` - Horizontal scale of the light drawn.

## scale_y
`scale_y=` `scale_y`

Variable Type: `Num` - Vertical scale of the light drawn.

## colour
`colour=` `colour`

Variable Type: `List` - Colour modifier of this lights in the form `[R, G, B, A]`, where rgba is in the range `[0, 1]`.

## texture
Write Only: `texture=`

Variable Type: `Texture` - Texture that will be used to draw this light.

## delete()
`delete()`

Deletes this light source.