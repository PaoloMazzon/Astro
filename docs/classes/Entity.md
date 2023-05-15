---
layout: default
title: Entity
parent: API Documentation
---

## Entity
Entities are a simple way to handle objects in the game world. You write your own classes
that inherit from this class, and then you may pass the child classes to [add_entity](Level#add_entity).
This class on its own does nothing.

 + [x](#x)
 + [y](#y)
 + [prev_x](#prev_x)
 + [prev_y](#prev_y)
 + [int_x](#int_x)
 + [int_y](#int_y)
 + [set_pos()](#set_pos)
 + [hitbox](#hitbox)
 + [sprite](#sprite)
 + [update_enabled](#update_enabled)
 + [colliding()](#colliding)
 + [create()](#create)
 + [update()](#update)
 + [draw()](#draw)
 + [destroy()](#destroy)

### x
`x=` `x`

Variable Type: `Num` - X coordinate in the game world.

### y
`y=` `y`

Variable Type: `Num` - Y coordinate in the game world.

### prev_x
`prev_x=(new_x)` `prev_x`

Variable Type: `Num` - The previous x value. This is mostly for internal use.

### prev_y
`prev_y=(new_y)` `prev_y`

Variable Type: `Num` - The previous y value. This is mostly for internal use.

### int_x
Read Only: `int_x`

Variable Type: `Num`

Returns the entity's x value interpolated between it current and previous x value by
`Engine.timstep_percent`.

### int_y
Read Only: `int_y`

Variable Type: `Num`

Returns the entity's y value interpolated between it current and previous y value by
`Engine.timstep_percent`.

### update_enabled
Write Only: `update_enabled=(enabled)`

Variable Type: `Bool` - Whether or not to allow the level to update this entity.
 
Controls whether or not this entity's `update(level)` method will be called by the level each frame.

### set_pos
`set_pos(x, y)`

Parameters
 + `x -> Num` New x position of the entity.
 + `y -> Num` New y position of the entity.

Sets the position of an entity without allowing for inter-frame interpolation.

### hitbox
`hitbox=` `hitbox`

Variable Type: `Hitbox` - Sets the hitbox.

### sprite
`sprite=` `sprite`

Variable Type: `Sprite` - Sets the sprite.

### colliding
`colliding(object)`
`colliding(object, x, y)`

Parameters
 + `object -> Entity or Tileset` Entity or [Tileset](Tileset) to check for a collision against.
 + `x -> Num` If specified, x position to use in place of the entity's actual x position.
 + `y -> Num` If specified, y position to use in place of the entity's actual y position.

Returns true if there is a collision between this entity and another entity or specified tileset. 

### create
`create(level, tiled_data)`

Parameters
 + `level -> Level` Level that this instance was created for.
 + `tiled_data -> Map or null` Map containing data from Tiled if this entity was loaded from a Tiled map, `null` otherwise.

Called by the level when the entity is added to the level.

`tiled_data` is a map with the following keys:

 + `x -> Num` X position in the editor - automatically set.
 + `y -> Num` Y position in the editor - automatically set.
 + `class -> String` Name of this entity.
 + `width -> Num` Width of the entity in the editor.
 + `height -> Num` Height of the entity in the editor.
 + `gid -> Num` GID of the entity in the editor.
 + `point -> Bool` Whether or not it is a single point in the editor.
 + `rotation -> Num` Rotation of the entity in the editor.
 + `id -> Num` ID in the editor.
 + `visible -> Bool` Whether or not its visible in the editor.
 + `properties -> Map` Map of all user defined properties in the editor.

If `tiled_data` is not null and a width and height were specified, [load()](Level#load)
will automatically create a hitbox of those dimensions before this method is called.

### update
`update(level)`

Parameters
 + `level -> Level` Level that this instance was created for.

Called by the level each frame.

### draw
`draw(level)`

Parameters
 + `level -> Level` Level that this instance was created for.

Called by the level each frame, if this isn't overloaded and `sprite` is non-null this will
render this entity's sprite.

### destroy
`destroy(level)`

Parameters
 + `level -> Level` Level that this instance was created for.

Called by the level at the end of the level or when the entity is destroyed.

