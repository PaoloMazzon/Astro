// Engine.wren
// Author: Paolo Mazzon
// Top-level meta functions for the framework
import "lib/Util" for Hitbox, Tileset, Math
import "lib/Renderer" for Renderer
import "lib/Tiled" for TiledMap
import "Assets" for Assets

// Framework control functions
class Engine {
    // Switches levels to a given level
    foreign static switch_level(level)

    // Ends the game, calling the current level's destruction method first
    foreign static quit()

    // Caps the FPS, or use 0 for no cap
    foreign static fps_limit=(fps)

    // Returns the time in seconds since the last frame
    foreign static delta

    // Returns the time in seconds since the program started
    foreign static time

    // Returns the average FPS over the last second
    foreign static fps

    // Returns a map containing info on the engine
    foreign static info

    // Sets Astro's timestep (how many times `process_frame` will return true in a second)
    foreign static timestep=(timestep)

    // Returns the percent through the current timestep (so if there is one frame processed every 50ms (timestep=20) and its been 25 ms since the last process frame this returns 0.5)
    foreign static timestep_percent

    // Returns true if this is a frame that should be processed
    foreign static process_frame

    // Returns a Wren class with a given class name in the format `module::class`
    foreign static get_class(class_name)

    // Returns a list of command line parameters
    foreign static argv

    // Returns whether or not the engine is running out of a game pak
    foreign static using_pak

    // For internal use
    foreign static report_debug(entity_count)
}

// Entity in the game world, child classes must make their own constructor
// named `new` and call the superclass constructor.
class Entity {
    construct new() {
        _x = 0
        _y = 0
        _prev_x = 0
        _prev_y = 0
        _sprite = null
        _hitbox = Hitbox.NO_HIT
        _update_enabled = true
    }

    int_x { Math.lerp(Engine.timestep_percent, _prev_x, _x) }
    int_y { Math.lerp(Engine.timestep_percent, _prev_y, _y) }

    // Returns the previous x
    prev_x { _prev_x }

    // Sets the previous x
    prev_x=(new_x) { _prev_x = new_x }

    // Returns the previous y
    prev_y { _prev_y }

    // Sets the previous y
    prev_y=(new_y) { _prev_y = new_y }

    // Returns the x position
    x { _x }

    // Sets the x position
    x=(new_x) { _x = new_x }

    // Returns the y position
    y { _y }

    // Sets the y position
    y=(new_y) { _y = new_y }

    // Sets the position without leaving a prev_x/y trace
    set_pos(x, y) {
        _x = x
        _y = y
        _prev_x = x
        _prev_y = y
    }

    update_enabled { _update_enabled }
    update_enabled=(s) { _update_enabled = s }

    // Returns the entity's hitbox
    hitbox { _hitbox }

    // Sets the entity's hitbox
    hitbox=(new_hitbox) { _hitbox = new_hitbox }

    // Returns the entity's sprite
    sprite { _sprite }

    // Sets the entity's sprite
    sprite=(new_sprite) { _sprite = new_sprite }

    // Checks for a collision between this entity's hitbox and another's or a tileset
    colliding(coll) {
        if (coll is Entity) {
            return hitbox.collision(x, y, coll.x, coll.y, coll.hitbox)
        } else if (coll is Tileset) {
            return coll.collision(hitbox, x, y)
        }
        return false
    }
    
    // Same as above but with a virtual x/y
    colliding(coll, new_x, new_y) {
        if (coll is Entity) {
            return hitbox.collision(new_x, new_y, coll.x, coll.y, coll.hitbox)
        } else if (coll is Tileset) {
            return coll.collision(hitbox, new_x, new_y)
        }
        return false
    }

    // Called by the level when its added to the level list
    create(level, tiled_data) {}

    // Called each frame of the level
    update(level) {
        prev_x = x
        prev_y = y
    }

    // Draws the entity each frame
    draw(level) {
        if (sprite != null) {
            Renderer.draw_sprite(sprite, x, y)
        }
    }

    // Called when the level ends or the entity is destroyed
    destroy(level) {}
}

// All levels must inherit this class and implement a constructor, additionally
// they must implement `create`, `update`, and `destroy` and call the super version
// within their own implementation.
class Level {
    // Adds an entity to the level, only pass the class and not an instance
    // of the class (ie, `add_entity(Player)`, not `add_entity(Player.new())`)
    add_entity(entity) {
        var new = entity.new()
        _entity_list.add(new)
        new.create(this, null)
        return new
    }

    update_enabled { _update_enabled }
    update_enabled=(s) { _update_enabled = s }

    // Same as add_entity but provides tiled_data as well
    add_entity(entity, tiled_data) {
        var new = entity.new()
        _entity_list.add(new)
        new.x = tiled_data["x"]
        new.y = tiled_data["y"]

        // Automatically create a new hitbox if a width and height is specified
        if (tiled_data["width"] != 0 && tiled_data["height"] != 0) {
            new.hitbox = Hitbox.new_rectangle(tiled_data["width"], tiled_data["height"])
        }

        new.create(this, tiled_data)

        return new
    }

    // Removes an entity from the level and calls its destroy function
    remove_entity(entity) {
        var ent = _entity_list.remove(entity)
        if (ent != null) {
            ent.destroy(this)
        }
    }

    // Removes all entities
    remove_all_entities() {
        for (entity in _entity_list) {
            ent.destroy(this)
        }
        _entity_list = []
    }

    // Returns the entity count
    entity_count { _entity_list.count }

    // Gets the first entity the level finds of a given type
    get_entity(base_class) {
        var out = null
        for (entity in _entity_list) {
            if (entity is base_class) {
                out = entity
                break
            }
        }
        return out
    }

    // Checks for a collision between an entity and any given entity of a specified base class
    // Returns null if no collision and the other entity if there is a collision
    entity_collision(entity, base_class) {
        var out = null
        for (ent in _entity_list) {
            if (ent is base_class) {
                if (entity.hitbox.collision(entity.x, entity.y, ent.x, ent.y, ent.hitbox)) {
                    out = ent
                    break
                }
            }
        }
        return out
    }

    // Same as above but allows the user to specify a specific x/y
    entity_collision(entity, x, y, base_class) {
        var out = null
        for (ent in _entity_list) {
            if (ent is base_class) {
                if (entity.hitbox.collision(x, y, ent.x, ent.y, ent.hitbox)) {
                    out = ent
                    break
                }
            }
        }
        return out
    }

    // Returns a list of entities that are all instances of the same class
    get_entities(base_class) {
        var type_list = []
        for (entity in _entity_list) {
            if (entity is base_class) {
                type_list.add(entity)
            }
        }
        return type_list
    }

    // Loads a Tiled map for this level
    load(filename) {
        var tilesets = {}
        var map = TiledMap.open(filename)
        var layer = map.next_layer()
        var gids = map.get_tilesets()

        while (layer != TiledMap.NO_MORE_LAYERS) {
            if (layer == TiledMap.OBJECT_LAYER) {
                // Create a bunch of entities
                var entities = map.get_objects()
                for (entity in entities) {
                    var internal = Engine.get_class(entity["class"])
                    if (internal != null) {
                        var e = add_entity(internal, entity)
                    } else {
                        System.print("Unable to find entity \"" + entity["class"] + "\" in map " + filename)
                    }
                }
            } else if (layer == TiledMap.TILE_LAYER) {
                // Create a new tileset
                var ts = Tileset.new(map.get_tiles(), null, map.cell_width, map.cell_height)
                for (slot in gids) {
                    ts.add_tileset(Assets["tex_" + slot["filename"].split(".")[0]], slot["gid"])
                }
                tilesets[map.layer_name] = ts
            }

            layer = map.next_layer()
        }

        return tilesets
    }

    // Called when the level is loaded, call the super to create the entity list
    create() {
        _entity_list = []
        _update_enabled = true
    }

    pre_frame() {

    }

    // Called each frame while the level is loaded, call the super to process
    // the entities in the level and handle debug stuff
    update() {
        for (entity in _entity_list) {
            if (Engine.process_frame && entity.update_enabled && _update_enabled) {
                entity.update(this)
            }
            entity.draw(this)
        }
        Engine.report_debug(_entity_list.count)
    }

    // Called whenever the level is unloaded (when the game stops or level change)
    destroy() {
        for (entity in _entity_list) {
            entity.destroy(this)
        }
    }
}