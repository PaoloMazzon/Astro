---
layout: default
title: Texture
parent: API Documentation
---

## Texture
2D images in Astro are represented by `Texture`. They are quite simple but very flexible when it
comes to rendering them.

 + [new](#new)
 + [width](#width)
 + [height](#height)
 + [free](#free)

### new
`construct new(filename)`

Parameters
 + `filename -> String` Filename of the texture to load.

Loads an image as a texture.

### width
`width`

Returns the width of the texture in pixels.

### height
`height`

Returns the height of the texture in pixels.

### free
`free()`

Forces the texture to be freed without waiting for the garbage collector.

