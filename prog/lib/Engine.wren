// Engine.wren
// Author: Paolo Mazzon
// Top-level meta functions for the framework

// Framework control functions
class Engine {
    // Switches levels to a given level
    foreign static switch_level(level)

    // Ends the game, calling the current level's destruction method first
    foreign static quit()

    // Caps the FPS, or use 0 for no cap
    foreign static cap_fps(fps)

    // Returns the time in seconds since the last frame
    foreign static delta()

    // Returns the time in seconds since the program started
    foreign static time()
}

// All levels must inherit this class and implement all functions AND a constructor
class Level {
    // Called when the level is loaded
    init() {}

    // Called each frame while the level is loaded
    update() {}

    // Called whenever the level is unloaded (when the game stops or level change)
    destroy() {}
}