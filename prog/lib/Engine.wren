// Engine.wren
// Author: Paolo Mazzon
// Top-level meta functions for the framework
import "lib/Util" for Hitbox

// Framework control functions
class Engine {
    // Switches levels to a given level
    foreign static switch_level(level)

    // Ends the game, calling the current level's destruction method first
    foreign static quit()

    // Caps the FPS, or use 0 for no cap
    foreign static cap_fps(fps)

    // Returns the time in seconds since the last frame
    foreign static delta

    // Returns the time in seconds since the program started
    foreign static time

    // Returns the average FPS over the last second
    foreign static fps

    // Returns a map containing info on the engine
    foreign static info

    // Returns a Wren class with a given class name in the format `module::class`
    foreign static get_class(class_name)
}

// Entity in the game world, child classes must implement their own
// getters for hitbox, x, and y since all entities are expected to
// have at least that. They must also have a constructor named `new`.
class Entity {
    // Returns the x position
    x { 0 }

    // Returns the y position
    y { 0 }

    // Returns the entity's hitbox
    hitbox { Hitbox.NO_HIT }

    // Called by the level when its added to the level list
    create(level) {}

    // Called each frame of the level
    update(level) {}

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
        new.create(this)
        return new
    }

    // Removes an entity from the level and calls its destroy function
    remove_entity(entity) {
        var ent = _entity_list.remove(entity)
        if (ent != null) {
            ent.destroy(this)
        }
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

    // Called when the level is loaded, call the super to create the entity list
    create() {
        _entity_list = []
    }

    // Called each frame while the level is loaded, call the super to process
    // the entities in the level
    update() {
        for (entity in _entity_list) {
            entity.update(this)
        }
    }

    // Called whenever the level is unloaded (when the game stops or level change)
    destroy() {
        for (entity in _entity_list) {
            entity.destroy(this)
        }
    }
}