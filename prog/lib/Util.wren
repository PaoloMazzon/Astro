// Util.wren
// Author: Paolo Mazzon
// Utilities for various aspects of a game
import "lib/Renderer" for Renderer

// Various math related methods
class Math {
    // Rounds a number to another number
    static round_to(x, y) {
        return (x / y).round * y
    }

    // Floors a number to another number
    static floor_to(x, y) {
        return (x / y).floor * y
    }

    // Ceils a number to another number
    static ceil_to(x, y) {
        return (x / y).ceil * y
    }

    // Calculates the angle between two points
    static point_angle(x1, y1, x2, y2) {
        return (x2 - x1).atan(y2 - y1)
    }

    // Calculates the distance between two points
    static point_distance(x1, y1, x2, y2) {
        return ((x2 - x1).pow(2) + (y2 - y1).pow(2)).sqrt
    }

    // Returns true if px/py is in the rectangle
    static point_in_rectangle(x, y, w, h, px, py) {
        return (px >= (x) && px <= (x + w) && py >= y  && py <= (y + h))
    }

    // Checks if a point is in a circle
    static point_in_circle(x, y, r, px, py) {
        return (point_distance(x, y, px, py) <= r)
    }

    // Linearly interpolates a value from start to stop inclusive
    static lerp(percent, start, stop) {
        return start + (percent * (stop - start))
    }

    // Same as lerp but uses a sin graph to smooth it out instead of being linear
    static serp(percent, start, stop) {
        return lerp((((Num.pi * percent) - (Num.pi / 2)).sin / 2) + 0.5, start, stop)
    }

    // Casts a coordinate by an angle
    static cast_x(length, angle) {
        return length * angle.cos
    }

    // Casts a coordinate by an angle
    static cast_y(length, angle) {
        return length * angle.sin
    }

    // Keeps a number between two numbers and returns it
    static clamp(x, min, max) {
        var val = x
        if (x > max) val = max
        if (x < min) val = min
        return val
    }
}

// Collision utilities
class Hitbox {
    static TYPE_CIRCLE { 0 }
    static TYPE_RECTANGLE { 1 }
    static NO_HIT { Hitbox.new_circle(0) }

    type { _type }
    r { _r }
    w { _w }
    h { _h }

    // Creates a new circle hitbox
    construct new_circle(r) {
        _type = Hitbox.TYPE_CIRCLE
        _r = r
    }

    // Creates a new rectangle hitbox
    construct new_rectangle(w, h) {
        _type = Hitbox.TYPE_RECTANGLE
        _w = w
        _h = h
    }

    // Returns true if there is a collision between this and another hitbox
    collision(x1, y1, x2, y2, hitbox2) {
        if (hitbox2.type == Hitbox.TYPE_RECTANGLE && _type == Hitbox.TYPE_RECTANGLE) {
            return (y1 + _h > y2 && y1 < y2 + hitbox2.h && x1 + _w > x2 && x1 < x2 + hitbox2.w)
        } else if (hitbox2.type == Hitbox.TYPE_CIRCLE && _type == Hitbox.TYPE_RECTANGLE) {
            return false
        } else if (hitbox2.type == Hitbox.TYPE_RECTANGLE && _type == Hitbox.TYPE_CIRCLE) {
            return false
        } else if (hitbox2.type == Hitbox.TYPE_CIRCLE && _type == Hitbox.TYPE_CIRCLE) {
            return Math.point_distance(x1, y1, x2, y2) < _r + hitbox2.r
        }
    }

    // Returns this hitbox's bounding box as a list of [x1, y1, x2, y2]
    bounding_box(x, y) {
        if (_type == Hitbox.TYPE_CIRCLE) {
            return [x - _r, y - _r, x + _r, y + _r]
        } else if (_type == Hitbox.TYPE_RECTANGLE) {
            return [x, y, x + _w, y + _h]
        }
    }
}

// Tileset for drawing and colliding with many things at once
class Tileset {
    // Creates a new tileset from a list, should be a 2D list like
    // [[x1y1, x2y1], [x1y2, x2y2]]. Each cell in the grid should be
    // an index in the sprite sheet + 1 because 0 is treated as an
    // empty space for collisions
    construct new(tileset, sprite, x, y) {
        _tileset = tileset
        if (sprite == null) {
            _sprites = []
            _gids = []
        } else {
            _sprites = [sprite]
            _gids = [1]
        }
        _x = x
        _y = y
    }

    // Adds a new sprite to the tileset, index starting at gid
    add_tileset(sprite, gid) {
        _sprites.add(sprite)
        _gids.add(gid)
    }

    // Returns total width of the tileset
    width { _tileset[0].count * (_sprites[0].width) }

    // Returns total height of the tileset
    height { _tileset.count * (_sprites[0].height) }

    // Returns the width of the tileset in tiles
    tile_width { _tileset[0].count }

    // Returns the height of the tileset in tiles
    tile_height { _tileset.count }

    // Returns an element in the tileset
    [x, y] {
        var cell = 0
        if (y >= 0 && y < _tileset.count) {
            if (x >= 0 && x < _tileset[y].count) {
                cell = _tileset[y][x]
            }
        }
        return cell
    }

    // Sets an element in the tileset
    [x, y]=(cell) {
        if (y >= 0 && y < _tileset.count) {
            if (x >= 0 && x < _tileset[y].count) {
                _tileset[y][x] = cell
            }
        }
    }

    // Returns true if one of the bounding box corners of the hitbox are in a cell that
    // isn't 0. Only works when the hitbox's bounding box w/h is equal or smaller to the
    // w/h of cells in the tileset (usually don't worry about that).
    collision(hitbox, x, y) {
        var hb = hitbox.bounding_box(x, y)
        var bb = [((hb[0] - _x) / _sprites[0].width).floor, ((hb[1] - _y) / _sprites[0].height).floor, ((hb[2] - _x) / _sprites[0].width).floor, ((hb[3] - _y) / _sprites[0].height).floor]
        var hit = false
        if (this[bb[0], bb[1]] != 0) {hit = true}
        if (this[bb[0], bb[3]] != 0) {hit = true}
        if (this[bb[2], bb[1]] != 0) {hit = true}
        if (this[bb[2], bb[3]] != 0) {hit = true}
        return hit
    }

    // Internal use
    internal_draw_sprite(gid, x, y) {
        var selected_gid = 1
        var sprite = 0

        // Find the right tileset
        for (i in 0..(_gids.count - 1)) {
            if (_gids[i] <= gid && _gids[i] > selected_gid) {
                sprite = i
                selected_gid = _gids[i]
            }
        }

        Renderer.draw_sprite(_sprites[sprite], gid - selected_gid, x, y)
    }

    // Draws the tileset
    draw() {
        var y_offset = _y

        // Loop through each cell in the 2D grid
        for (i in _tileset) {
            var x_offset = _x
            for (cell in i) {
                if (cell != 0) {
                    internal_draw_sprite(cell, x_offset, y_offset)
                }
                x_offset = x_offset + _sprites[0].width
            }
            y_offset = y_offset + _sprites[0].height
        }
    }

    // Draws a tiling background with specified parallax (movement)
    // 0 for parallax would be static regardless of camera, 1 would
    // be moves with the camera. This doesn't work with rotated cameras.
    static draw_tiling_background(texture, parallax, camera) {
        var cx = 0
        var cy = 0
        var cwidth = 0
        var cheight = 0
        if (camera != null) {
            cx = camera.x
            cy = camera.y
            cwidth = camera.width
            cheight = camera.height
        } else {
            var conf = Renderer.get_config()
            cwidth = conf["window_width"]
            cheight = conf["window_height"]
        }

        var tile_start_x = cx * parallax
        var tile_start_y = cy * parallax
        tile_start_x = tile_start_x + (((cx - tile_start_x) / texture.width).floor * texture.width)
        tile_start_y = tile_start_y + (((cy - tile_start_y) / texture.height).floor * texture.height)
        var horizontal = (cwidth / texture.width).ceil
        var vertical = (cheight / texture.height).ceil
        for (y in 0..vertical) {
            for (x in 0..horizontal) {
                Renderer.draw_texture(texture, tile_start_x + (x * texture.width), tile_start_y + (y * texture.height))
            }
        }
    }
}

// Binary buffers
foreign class Buffer {
    // Creates a buffer
    construct new(size) {}

    // Creates a buffer from a file
    foreign static open(filename)

    // Resizes a buffer, doesn't move the pointer
    foreign resize(size)

    // Returns size of the buffer
    foreign size

    // Sets the pointer
    foreign pointer=(val)

    // Gets the pointer position
    foreign pointer

    // Reads a double from the buffer
    foreign read_double()

    // Write a double to the buffer
    foreign write_double(double)

    // Reads a float from the buffer    
    foreign read_float()

    // Write a float to the buffer
    foreign write_float(float)

    // Reads a uint64 from the buffer    
    foreign read_uint64()

    // Write a uint64 to the buffer
    foreign write_uint64(uint64)

    // Reads a uint32 from the buffer    
    foreign read_uint32()

    // Write a uint32 to the buffer
    foreign write_uint32(uint32)

    // Reads a uint16 from the buffer    
    foreign read_uint16()

    // Write a uint16 to the buffer
    foreign write_uint16(uint16)

    // Reads a uint8 from the buffer    
    foreign read_uint8()

    // Write a uint8 to the buffer
    foreign write_uint8(uint8)

    // Reads a int64 from the buffer    
    foreign read_int64()

    // Write a int64 to the buffer
    foreign write_int64(int64)

    // Reads a int32 from the buffer    
    foreign read_int32()

    // Write a int32 to the buffer
    foreign write_int32(int32)

    // Reads a int16 from the buffer    
    foreign read_int16()

    // Write a int16 to the buffer
    foreign write_int16(int16)

    // Reads a int8 from the buffer    
    foreign read_int8()

    // Write a int8 to the buffer
    foreign write_int8(int8)

    // Reads a string from the buffer    
    foreign read_string(size)

    // Write a string to the buffer
    foreign write_string(string)

    // Reads a bool from the buffer    
    foreign read_bool()

    // Write a bool to the buffer
    foreign write_bool(bool)

}