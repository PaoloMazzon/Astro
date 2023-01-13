// Sprite.wren
// Author: Paolo Mazzon
// Tools for creating and managing sprites

// Animated sprites
foreign class Sprite {
    // Loads a sprite from a spritesheet, x/y is where
    // the frames are in the spritesheet, w/h is the width
    // and height of each frame and frames is the number
    // of frames to pull from the spritesheet 
    construct new(filename, x, y, w, h, delay, frames) {}

    // Forces the sprite to be freed
    foreign free()

    // Returns a copy of this sprite that just references the internal
    // texture and doesn't copy it
    foreign copy()

    // Gets this sprite's delay
    foreign get_delay()

    // Sets this sprite's delay
    foreign set_delay(delay)
    
    // Gets this sprite's origin_x
	foreign get_origin_x()

    // Sets this sprite's origin_x
    foreign set_origin_x(origin_x)
    
    // Gets this sprite's origin_y
	foreign get_origin_y()

    // Sets this sprite's origin_y
    foreign set_origin_y(origin_y)
    
    // Gets this sprite's scale_x
	foreign get_scale_x()

    // Sets this sprite's scale_x
    foreign set_scale_x(scale_x)
    
    // Gets this sprite's scale_y
	foreign get_scale_y()

    // Sets this sprite's scale_y
    foreign set_scale_y(scale_y)
    
    // Gets this sprite's rotation
	foreign get_rotation()

    // Sets this sprite's rotation
    foreign set_rotation(rotation)
}