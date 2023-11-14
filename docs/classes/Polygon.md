---
layout: default
title: Polygon
parent: API Documentation
---

## Polygon
Polygons are a way of rendering arbitrary polygons, see [draw_polygon()](Renderer#draw_polygon)
for info on how they may be drawn. They are all drawn as the current colour modifier.

 + [create()](#create)
 + [free()](#free)

## create
`construct create(vertices)`

Parameters
 + `vertices -> List` List of vertices, where each element is in the form `[x, y]`

Creates a new polygon from given vertices. The vertex list MUST be triangulated.

For example, you may do something like

    var poly = Polygon.create([
        [0, 0],
        [-1, 1],
        [1, 1]
    ])

to make a triangle.

## free
`free()`

Forces a polygon to be freed from memory.

