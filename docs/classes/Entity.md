## Entity
Entities are a simple way to handle objects in the game world. You write your own classes
that inherit from this class, and then you may pass the child classes to [add_entity](Level.md#add_entity).
This class on its own does nothing.

 + [x](#x-getter)
 + [y](#y-getter)
 + [x=](#x-setter)
 + [y=](#y-setter)
 + [prev_x](#prev_x-getter)
 + [prev_x=](#prev_x-setter)
 + [prev_y](#prev_y-getter)
 + [prev_y=](#prev_y-setter)
 + [hitbox](#hitbox-getter)
 + [hitbox=](#hitbox-setter)
 + [sprite](#sprite-getter)
 + [sprite=](#sprite-setter)
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

### prev_x
`prev_x`

Returns the previous x value.

### prev_x=(new_x)
`prev_x=(new_x)`

Parameters
 + `new_ -> Num` New value.
 
Sets the previous x value. This is mostly for internal use.

### prev_y
`prev_y`

Returns the previous y value.

### prev_y=(new_y)
`prev_y=(new_y)`

Parameters
 + `new_ -> Num` New value.
 
Sets the previous y value. This is mostly for internal use.


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

### create
`create(level, tiled_data)`

Parameters
 + `level -> lib/Engine::Level` Level that this instance was created for.
 + `tiled_data -> Map` Map containing data from Tiled if this entity was loaded from a Tiled map.

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
 + `level -> lib/Engine::Level` Level that this instance was created for.

Called by the level each frame.

### draw
`draw(level)`

Parameters
 + `level -> lib/Engine::Level` Level that this instance was created for.

Called by the level each frame, if this isn't overloaded and `sprite` is non-null this will
render this entity's sprite.

### destroy
`destroy(level)`

Parameters
 + `level -> lib/Engine::Level` Level that this instance was created for.

Called by the level at the end of the level or when the entity is destroyed.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)