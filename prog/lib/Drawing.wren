// Drawing.wren
// Author: Paolo Mazzon
// Drawing related classes

// Bitmap fonts
foreign class BitmapFont {
    // Loads a bitmap font from a given image filename, it expects at
    // least unicode_end-unicode_start characters of w*h size
    construct new(filename, unicode_start, unicode_end, w, h) {}

    // Forces the font to be freed if you don't want to wait for the garbage collector
    foreign free()
}

// 2D textures
foreign class Texture {
    construct new(filename) {}

    // Returns the width of the texture
    foreign width

    // Returns the height of the texture
    foreign height

    // Forces the internal texture to be freed if you don't want to wait for the garbage collector
    foreign free()
}

// Textures you can draw to
foreign class Surface {
    // Creates a new surface you can draw on and draw to the screen
    construct new(width, height) {}

    // Returns the width of the texture
    foreign width

    // Returns the height of the texture
    foreign height

    // Forces the surface to be freed if you don't want to wait for the garbage collector
    foreign free()
}

// Animated sprites
foreign class Sprite {
    // Loads a sprite from a spritesheet, x/y is where
    // the frames are in the spritesheet, w/h is the width
    // and height of each frame and frames is the number
    // of frames to pull from the spritesheet 
    construct new(filename, x, y, w, h, delay, frames) {}

    // Same as new but uses an already loaded texture
    foreign static from(texture, x, y, w, h, delay, frames)

    // Forces the sprite to be freed if you don't want to wait for the garbage collector
    foreign free()

    // Returns a copy of this sprite that just references the internal
    // texture and doesn't copy it
    foreign copy()

    // Gets this sprite's delay
    foreign delay

    // Sets this sprite's delay
    foreign delay=(delay)
    
    // Gets this sprite's origin_x
	foreign origin_x

    // Sets this sprite's origin_x
    foreign origin_x=(origin_x)
    
    // Gets this sprite's origin_y
	foreign origin_y

    // Sets this sprite's origin_y
    foreign origin_y=(origin_y)
    
    // Gets this sprite's scale_x
	foreign scale_x

    // Sets this sprite's scale_x
    foreign scale_x=(scale_x)
    
    // Gets this sprite's scale_y
	foreign scale_y

    // Sets this sprite's scale_y
    foreign scale_y=(scale_y)
    
    // Gets this sprite's rotation
	foreign rotation

    // Sets this sprite's rotation
    foreign rotation=(rotation)

    // Returns the sprite's width
    foreign width

    // Returns the sprite's height
    foreign height
}
