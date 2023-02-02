## Entity
Entities are a simple way to handle objects in the game world. You write your own classes
that inherit from this class, and then you may pass the child classes to [add_entity](Level.md#add_entity).
This class on its own does nothing.

 + [x](#x-getter)
 + [y](#y-getter)
 + [x=](#x-setter)
 + [y=](#y-setter)
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
`create(level)`

Parameters
 + `level -> lib/Engine::Level` Level that this instance was created for.

Called by the level when the entity is added to the level.

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