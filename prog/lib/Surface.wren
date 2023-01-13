// Surface.wren
// Author: Paolo Mazzon
// Bindings for VK2D texture targets

// Wren representation of a VK2D texture as a render target
foreign class Surface {
    // Creates a new surface you can draw on and draw to the screen
    construct new(width, height) {}

    // Returns the width of the texture
    foreign width()

    // Returns the height of the texture
    foreign height()

    // Forces the surface to be freed if you don't want to wait for the garbage collector
    foreign free()
}