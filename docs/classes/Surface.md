## Surface
Surfaces are textures that you may also render to. This means that you can use all
the `lib/Renderer::draw_texture` methods on surfaces, as well as set the render target
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

Creates of a new surface of specified dimensions.

### width
`width`

Returns the width of the surface in pixels.

### height
`height`

Returns the height of the surface in pixels.

### free
`free()`

Forces the texture to be freed without waiting for the garbage collector.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)