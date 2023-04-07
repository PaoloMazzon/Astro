## Model
Astro is a 2D-first engine but there is still very primitive 3D support through models.
You may load 3D models from `.obj` files or create them from a list of vertices. You may
then use a 3D camera to draw them. To have an effect like that of the save points in
*Castlevania: Symphony of the Night* you would take the following steps:

 1. Load your model's texture and vertex data separately
 2. Create a camera with type `CAMERA_TYPE_PERSPECTIVE` or `CAMERA_TYPE_ORTHOGONAL`
 3. Create a small surface to render that model to, since 3D cameras don't directly work with 2D cameras
 4. Each frame, draw the model to the surface then the surface wherever you want in the game world

You may also attempt to sync up the 2D and 3D cameras, there are no restrictions that way.

 + [create](#create)
 + [free](#free)
 + [load](#load)
 + [vertex](#vertex)

### create
`construct create(vertices, indices, texture)`

Parameters
 + `vertices -> List` List of vertices, where each vertex is the return value of [vertex](#vertex).
 + `indices -> List` List of indices for the index buffer, should just be numbers.
 + `texture -> Texture or Surface` Texture to UV map the model to.

Creates a new model from given vertices and indices. The index list MUST be triangulated.

> 📝 The index buffer is internally indexed using 16-bit unsigned integers, meaning there is a
> maximum of 65536/FFFF/2^16 indices allowed in any given model.

### free
`free()`

Forces Astro to free a model.

### load
`load(obj_file, texture)`

Parameters
 + `obj_file -> String` Filename of the `.obj` file.
 + `texture -> Texture or Surface` Texture to UV map the model to.

Loads a model from a `.obj` file, mapping `texture` to it.

> 📝 The index buffer is internally indexed using 16-bit unsigned integers, meaning there is a
> maximum of 65536/FFFF/2^16 indices allowed in any given model.

### vertex
`static vertex(x, y, z, u, v)`

Parameters
 + `x -> Num` x component of the vertex.
 + `y -> Num` y component of the vertex.
 + `z -> Num` z component of the vertex.
 + `u -> Num` u component of the vertex.
 + `v -> Num` v component of the vertex.

Returns a new vertex with given components.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)