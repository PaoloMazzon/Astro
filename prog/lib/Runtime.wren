// Runtime.wren
// Author: Paolo Mazzon
// Top-level meta functions for the framework

// Framework control functions
class Runtime {
    // Switches levels to a given level
    foreign static switch_level(level)

    // Ends the game, calling the current level's destruction method first
    foreign static quit()

    // Caps the FPS, or use 0 for no cap
    foreign static cap_fps(fps)
}