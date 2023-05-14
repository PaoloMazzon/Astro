---
layout: default
title: Surface
parent: API Documentation
---

## Surface
Surfaces are textures that you may also render to. This means that you can use all
the `Renderer.draw_texture` methods on surfaces, as well as set the render target
to a surface. Using surfaces you can do things like draw an entire menu to it and
scroll through it, or render tile sets to the surface once and just render the surface
for a performance boost. See [set_target](Renderer.md#set_target).

+ [new](#new)
+ [width](#width)
+ [height](#height)
+ [free](#free)

### new
`construct new(width, height)`

Parameters
 + `width -> Num` Width of the new surface in pixels.
 + `height -> Num` Height of the new surface in pixels.

Creates of a new surface of specified dimensions. It is important that you clear
new surfaces if you don't intend to completely fill it as their contents are undefined
by default which will result in weird behaviour.

A good way to get a complete clear surface after creating it is

    var surf = Surface.new(100, 100)
    var original_blend_mode = Renderer.blend_mode
    var original_colour_mod = Renderer.colour_mod
    Renderer.blend_mode = Renderer.BLEND_MODE_NONE
    Renderer.colour_mod = [0, 0, 0, 0]
    Renderer.target = surf
    Renderer.clear()
    Renderer.target = Renderer.RENDER_TARGET_DEFAULT
    Renderer.colour_mod = original_colour_mod
    Renderer.blend_mode = original_blend_mode

### width
`width`

Returns the width of the surface in pixels.

### height
`height`

Returns the height of the surface in pixels.

### free
`free()`

Forces the texture to be freed without waiting for the garbage collector.

