// Levels.wren
// Author: Paolo Mazzon
// Level superclass for all other game levels to inherit from

// All levels must inherit this class and implement all functions AND a constructor
class Level {
    // Called when the level is loaded
    init() {}

    // Called each frame while the level is loaded
    update() {}

    // Called whenever the level is unloaded (when the game stops or level change)
    destroy() {}
}