// Surface.wren
// Author: Paolo Mazzon
// Bindings for VK2D texture targets

// Wren representation of a VK2D texture as a render target
foreign class Surface {
    construct new(width, height) {}

    // Returns the width of the texture
    foreign width()

    // Returns the height of the texture
    foreign height()
}