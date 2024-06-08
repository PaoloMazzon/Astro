// Drawing.wren
// Author: Paolo Mazzon
// Drawing related classes

// Bitmap fonts
foreign class BitmapFont {
    // Loads a bitmap font from a given image filename, it expects at
    // least unicode_end-unicode_start characters of w*h size
    construct new(filename, unicode_start, unicode_end, w, h) {}

    // Returns the size in pixels of a given string as [w, h]
    foreign size(string)

    // Returns the size of a string as if it were wrapped
    foreign size_wrapped(string, width)

    // Same as above but ignores tokens
    foreign size_ext(string)

    // Same as above but wrapped
    foreign size_ext_wrapped(string, width)

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

// For internal use with lighting
foreign class Shadow {
    foreign position=(pos)
    foreign update(x, y, scale_x, scale_y, rotation, origin_x, origin_y)
    foreign enabled=(enabled)
    foreign enabled
}

// Internal use for shadows
foreign class LightSource {
    foreign position=(pos)
    foreign position
    foreign rotation=(rotation)
    foreign rotation
    foreign origin_x=(origin_x)
    foreign origin_x
    foreign origin_y=(origin_y)
    foreign origin_y
    foreign texture=(texture)
    foreign delete()
}

// Shadows
foreign class Lighting {
    foreign static add_shadow(edges) // returns a shadow object
    foreign static add_light(x, y, rotation, origin_x, origin_y, texture) // returns a new light source
    foreign static reset() // destroys all shadow objects
    foreign static flush_vbo() // call once done adding shadow objects
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

    // Returns current frame in the animation
    foreign frame
    
    // Sets the current frame
    foreign frame=(frame)

    // Returns the number of frames in the animations
    foreign frame_count

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

// TrueType fonts
foreign class Font {
    // Opens a ttf and caches it so it can be drawn faster
    construct open(filename, size, aa, unicode_start, unicode_end) {}

    // Returns the size in pixels of a given string as [w, h]
    foreign size(string)

    // Returns the size of a string as if it were wrapped
    foreign size_wrapped(string, width)

    // Same as above but ignores tokens
    foreign size_ext(string)

    // Same as above but wrapped
    foreign size_ext_wrapped(string, width)

    // Frees without waiting for the gc
    foreign free()
}

// For 3D models
foreign class Model {
    vertex(x, y, z, u, v) {
        return [x, y, z, u, v]
    }

    // Creates a 3D model from a list of vertices - expects triangles
    construct create(vertices, indices, texture) {}

    // Loads a 3D model from a .obj file
    foreign static load(obj_file, texture)

    // Forces the engine to free a model
    foreign free()
}

// A 2D polygon
foreign class Polygon {
    construct new() {}

    // Creates a polygon from a list of vertices - triangulates automatically
    foreign static create(vertices)

    // Forces the engine to free a polygon
    foreign free()
}