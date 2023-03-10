## Level
All levels in Astro must inherit from this class. See [switch_level](Engine.md#switch_level).

 + [add_entity](#add_entity)
 + [remove_entity](#remove_entity)
 + [remove_all_entities](#remove_all_entities)
 + [entity_count](#entity_count)
 + [get_entity](#get_entity)
 + [entity_collision](#entity_collision)
 + [get_entities](#get_entities)
 + [load](#load)
 + [create](#create)
 + [update](#update)
 + [destroy](#destroy)


### add_entity
`add_entity(entity)`
`add_entity(entity, tiled_data)`

Parameters
 + `entity -> lib/Engine::Entity` Entity class to add to the level.
 + `tiled_data -> Map` If provided, supplies Tiled data to the entity.

Adds an entity to a level, `entity` must be a class and not an instance of a class. That
class must inherit from `lib/Engine::Entity`. This method returns the new instance of the
entity that was added to the level, and calls that entity's `create` method.

### remove_entity
`remove_entity(entity)`

Parameters
 + `entity -> lib/Engine::Entity` Entity instance to remove.
 
Finds `entity` in the level, calls the entity's `destroy` method, then remove it from the level.

### remove_all_entities
`remove_all_entities()`

Removes all entities in the level, calling each `destroy` method.

### entity_count
`entity_count`

Returns the number of entities in the level.

### get_entity
`get_entity(base_class)`

Parameters
 + `base_class -> lib/Engine::Entity` Base class of the entity to find, not an instance.

Returns the first instance of `base_class` the level finds, or `null` if it can't find any.

### entity_collision
`entity_collision(entity, base_class)`

Parameters
 + `entity -> lib/Engine::Entity` Entity instance to check for collisions against.
 + `base_class -> lib/Engine::Entity` Base class of the entity to find, not an instance.

Checks for a collision between an instance of an entity and every instance of `base_class`
in the level. Returns `null` if none are found, and returns the instance of the entity its
colliding with if a collision is occurring.

### get_entities
`get_entities(base_class)`

Parameters
 + `base_class -> lib/Engine::Entity` Entity class to search for.
 
This method returns a list of all entities in the level that are an instance of `base_class`.

### load
`load(filename)`

Parameters
 + `filename -> String` Filename of the Tiled map.

Loads a [Tiled](https://www.mapeditor.org/) map as a level. See [Tiled Integration](../TiledIntegration.md)
for more information. Returns a map of `lib/Util::Tileset`s representing each tile layer by their name, for
instance if you had two tileset layers in Tiled, 'Foreground' and 'Background', you can access their
`lib/Util::Tileset` equivalent via `tilesets["Background"]` and `tilesets["Foreground"]` respectively.

### create
`create()`

May be overridden in child classes, called when the level is loaded by Astro. If this method
is overridden, you must call `super.create()` somewhere in the new method.

### update
`update()`

May be overridden in child classes, called each frame by Astro. If this method
is overridden, you must call `super.update()` somewhere in the new method.

### destroy
`destroy()`

May be overridden in child classes, called when the level is unloaded by Astro. If this method
is overridden, you must call `super.destroy()` somewhere in the new method.


-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)