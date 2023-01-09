// Texture.wren
// Author: Paolo Mazzon
// Bindings for VK2D textures

// Wren representation of a VK2D texture
foreign class Texture {
    construct new(filename) {}

    // Returns the width of the texture
    foreign width()

    // Returns the height of the texture
    foreign height()
}