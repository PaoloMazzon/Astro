---
layout: default
title: Level
parent: API Documentation
---

## Level
All levels in Astro must inherit from this class. See [switch_level](Engine.md#switch_level).

 + [add_entity](#add_entity)
 + [remove_entity](#remove_entity)
 + [update_enabled](#update_enabled-getter)
 + [update_enabled=](#update_enabled-setter)
 + [remove_all_entities](#remove_all_entities)
 + [entity_count](#entity_count)
 + [get_entity](#get_entity)
 + [entity_collision](#entity_collision)
 + [get_entities](#get_entities)
 + [load](#load)
 + [create](#create)
 + [pre_frame](#pre_frame)
 + [update](#update)
 + [destroy](#destroy)


### add_entity
`add_entity(entity)`
`add_entity(entity, tiled_data)`

Parameters
 + `entity -> Entity` Entity class to add to the level.
 + `tiled_data -> Map` If provided, supplies Tiled data to the entity.

Adds an entity to a level, `entity` must be a class and not an instance of a class. That
class must inherit from `Entity`. This method returns the new instance of the
entity that was added to the level, and calls that entity's `create` method.

### remove_entity
`remove_entity(entity)`

Parameters
 + `entity -> Entity` Entity instance to remove.
 
Finds `entity` in the level, calls the entity's `destroy` method, then remove it from the level.

### remove_all_entities
`remove_all_entities()`

Removes all entities in the level, calling each `destroy` method.

### entity_count
`entity_count`

Returns the number of entities in the level.

### update_enabled (getter)
`update_enabled`

Returns whether or not entities' update methods will be called when you call `super.update(level)`.

### update_enabled= (setter)
`update_enabled=(enabled)`

Parameters
 + `enabled -> Bool` Whether or not to allow entities to update each frame.

Controls whether or not entities' update methods will be called when you call `super.update(level)`.
This is often useful when you want something like a pause menu.

### get_entity
`get_entity(base_class)`

Parameters
 + `base_class -> Entity` Base class of the entity to find, not an instance.

Returns the first instance of `base_class` the level finds, or `null` if it can't find any.

### entity_collision
`entity_collision(entity, base_class)`
`entity_collision(entity, x, y, base_class)`

Parameters
 + `entity -> Entity` Entity instance to check for collisions against.
 + `x -> Num` If specified, the virtual x position of `entity`.
 + `y -> Num` If specified, the virtual y position of `entity`.
 + `base_class -> Entity` Base class of the entity to find, not an instance.

Checks for a collision between an instance of an entity and every instance of `base_class`
in the level. Returns `null` if none are found, and returns the instance of the entity its
colliding with if a collision is occurring.

### get_entities
`get_entities(base_class)`

Parameters
 + `base_class -> Entity` Entity class to search for.
 
This method returns a list of all entities in the level that are an instance of `base_class`.

### load
`load(filename)`

Parameters
 + `filename -> String` Filename of the Tiled map.

Loads a [Tiled](https://www.mapeditor.org/) map as a level. See [Tiled Integration](../TiledIntegration)
for more information. Returns a map of `Tileset`s representing each tile layer by their name, for
instance if you had two tileset layers in Tiled, 'Foreground' and 'Background', you can access their
`Tileset` equivalent via `tilesets["Background"]` and `tilesets["Foreground"]` respectively. If `load`
cannot find the entities in the map, it will attempt to import the module that contains them itself.

### create
`create()`

Must be overridden in child classes, called when the level is loaded by Astro. Be sure
to call `super.create()` somewhere in the new method.

### pre_frame
`pre_frame()`

Must be overridden in child classes, called before the rendering frame begins each frame.
This is useful because once the rendering frame begins any camera updates and shader buffer
updates will not be applied until the next time rendering begins. Be sure to call 
`super.create()` somewhere in the new method.

### update
`update()`

Must be overridden in child classes, called each frame by Astro. Be sure
to call `super.update()` somewhere in the new method, as it calls the update/draw method
of each entity.

### destroy
`destroy()`

Must be overridden in child classes, called when the level is unloaded by Astro. Be sure
to call `super.destroy()` somewhere in the new method.


