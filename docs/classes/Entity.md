## Entity
Entities are a simple way to handle objects in the game world. You write your own classes
that inherit from this class, and then you may pass the child classes to [add_entity](Level.md#add_entity).
This class on its own does nothing.

 + [x](#x)
 + [y](#y)
 + [hitbox](#hitbox)
 + [create](#create)
 + [update](#update)
 + [destroy](#destroy)

### x
`x`

One of the fields that must be in all entities, by default this just returns 0 meaning
child classes must implement their own x field and their version of this method.

### y
`y`

One of the fields that must be in all entities, by default this just returns 0 meaning
child classes must implement their own y field and their version of this method.

### hitbox
`hitbox`

One of the fields that must be in all entities, by default this just returns a blank hitbox meaning
child classes must implement their own hitbox field and their version of this method.

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

### destroy
`destroy(level)`

Parameters
 + `level -> lib/Engine::Level` Level that this instance was created for.

Called by the level at the end of the level or when the entity is destroyed.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)