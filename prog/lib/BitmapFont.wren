// BitmapFont.wren
// Author: Paolo Mazzon
// Tools for bitmap fonts

// Represents a bitmap font
foreign class BitmapFont {
    // Loads a bitmap font from a given image filename, it expects at
    // least unicode_end-unicode_start characters of w*h size
    construct new(filename, unicode_start, unicode_end, w, h) {}

    // Forces the font to be freed
    foreign free()
}
