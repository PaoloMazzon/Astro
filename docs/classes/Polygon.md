---
layout: default
title: Polygon
parent: API Documentation
---

## Polygon
Polygons are a way of rendering arbitrary polygons that are highly customizable as well.
You may specify polygons of any amount of vertices with any colours and draw them scaled/
rotated/translated/whatever.

 + [create](#create)
 + [free](#free)
 + [vertex](#vertex)

## create
`construct create(vertices)`

Parameters
 + `vertices -> List` List of vertices, where each element is a value returned from [vertex](#vertex)

Creates a new polygon from given vertices. The vertex list MUST be triangulated.

For example, you may do something like

    var poly = Polygon.create([
        Polygon.vertex(0, 0, 1, 0, 0, 1),
        Polygon.vertex(-1, 1, 1, 0, 0, 1),
        Polygon.vertex(1, 1, 1, 0, 0, 1))

to make a red triangle.

## free
`free()`

Forces a polygon to be freed from memory.

## vertex
`static vertex(x, y, r, g, b, a)`

Parameters
 + `x -> Num` x component of the vertex.
 + `y -> Num` y component of the vertex.
 + `r -> Num` r component of the vertex.
 + `g -> Num` g component of the vertex.
 + `b -> Num` b component of the vertex.
 + `a -> Num` a component of the vertex.

Returns a vertex with given components.

