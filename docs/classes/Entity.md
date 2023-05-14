---
layout: default
title: Entity
parent: API Documentation
---

## Entity
Entities are a simple way to handle objects in the game world. You write your own classes
that inherit from this class, and then you may pass the child classes to [add_entity](Level#add_entity).
This class on its own does nothing.

 + [x](#x-getter)
 + [y](#y-getter)
 + [x=](#x-setter)
 + [y=](#y-setter)
 + [prev_x](#prev_x-getter)
 + [prev_x=](#prev_x-setter)
 + [prev_y](#prev_y-getter)
 + [prev_y=](#prev_y-setter)
 + [int_x](#int_x)
 + [int_y](#int_y)
 + [set_pos](#set_pos)
 + [hitbox](#hitbox-getter)
 + [hitbox=](#hitbox-setter)
 + [sprite](#sprite-getter)
 + [sprite=](#sprite-setter)
 + [colliding](#colliding)
 + [create](#create)
 + [update](#update)
 + [draw](#draw)
 + [destroy](#destroy)

### x (Getter)
`x`

Returns the x.

### y (Getter)
`y`

Returns the y.

### x= (Setter)
`x=`

Sets the x.

### y= (Setter)
`y=`

Sets the y.

### prev_x (Getter)
`prev_x`

Returns the previous x value.

### prev_x= (Setter)
`prev_x=(new_x)`

Parameters
 + `new_ -> Num` New value.
 
Sets the previous x value. This is mostly for internal use.

### prev_y (Getter)
`prev_y`

Returns the previous y value.

### prev_y= (Setter)
`prev_y=(new_y)`

Parameters
 + `new_ -> Num` New value.
 
Sets the previous y value. This is mostly for internal use.

### int_x
`int_x`

Returns the entity's x value interpolated between it current and previous x value by
`Engine.timstep_percent`.

### int_y
`int_y`

Returns the entity's y value interpolated between it current and previous y value by
`Engine.timstep_percent`.

### update_enabled (getter)
`update_enabled`

Returns whether or not this entity's `update(level)` method will be called by the level each frame.

### update_enabled= (setter)
`update_enabled=(enabled)`

Parameters
 + `enabled -> Bool` Whether or not to allow the level to update this entity.
 
Controls whether or not this entity's `update(level)` method will be called by the level each frame.

### set_pos
`set_pos(x, y)`

Parameters
 + `x -> Num` New x position of the entity.
 + `y -> Num` New y position of the entity.

Sets the position of an entity without allowing for inter-frame interpolation.

### hitbox (Getter)
`hitbox`

Returns the hitbox.

### hitbox= (Setter)
`hitbox=`

Sets the hitbox.

### sprite (Getter)
`sprite`

Returns the sprite.

### sprite= (Setter)
`sprite=`

Sets the sprite.

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

 + `x -> Num` X position in the editor.
 + `y -> Num` Y position in the editor.
 + `class -> String` Name of this entity.
 + `width -> Num` Width of the entity in the editor.
 + `height -> Num` Height of the entity in the editor.
 + `gid -> Num` GID of the entity in the editor.
 + `point -> Bool` Whether or not it is a single point in the editor.
 + `rotation -> Num` Rotation of the entity in the editor.
 + `id -> Num` ID in the editor.
 + `visible -> Bool` Whether or not its visible in the editor.
 + `properties -> Map` Map of all user defined properties in the editor.

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

