// Util.wren
// Author: Paolo Mazzon
// Utilities for various aspects of a game
import "lib/Renderer" for Renderer
import "lib/Drawing" for Surface

// Various math related methods
foreign class Math {
    // Rounds a number to another number
    foreign static round_to(x, y)

    // Floors a number to another number
    foreign static floor_to(x, y)

    // Ceils a number to another number
    foreign static ceil_to(x, y)

    // Calculates the angle between two points
    foreign static point_angle(x1, y1, x2, y2)

    // Calculates the distance between two points
    foreign static point_distance(x1, y1, x2, y2)

    // Returns true if px/py is in the rectangle
    foreign static point_in_rectangle(x, y, w, h, px, py)

    // Checks if a point is in a circle
    foreign static point_in_circle(x, y, r, px, py)

    // Linearly interpolates a value from start to stop inclusive
    foreign static lerp(percent, start, stop)

    // Same as lerp but uses a sin graph to smooth it out instead of being linear
    foreign static serp(percent, start, stop)

    // Casts a coordinate by an angle
    foreign static cast_x(length, angle)

    // Casts a coordinate by an angle
    foreign static cast_y(length, angle)

    // Keeps a number between two numbers and returns it
    foreign static clamp(x, min, max)
}

foreign class Hitbox {
    construct new() {}
    foreign static new_circle(radius)
    foreign static new_rectangle(w, h)
    foreign static new_polygon(vertices)
    foreign static NO_HIT
    foreign r
    foreign w
    foreign h
    foreign no_hit
    foreign x_offset=(offset)
    foreign y_offset=(offset)
    foreign x_offset
    foreign y_offset
    foreign collision(x1, y1, x2, y2, hitbox)
    foreign bounding_box(x, y)
    foreign bb_left(x, y)
    foreign bb_right(x, y)
    foreign bb_top(x, y)
    foreign bb_bottom(x, y)
}

// Tileset for drawing and colliding with many things at once
class Tileset {
    // Creates a new tileset from a list, should be a 2D list like
    // [[x1y1, x2y1], [x1y2, x2y2]]. Each cell in the grid should be
    // an index in the sprite sheet + 1 because 0 is treated as an
    // empty space for collisions
    construct new(tileset, texture, w, h) {
        _tileset = tileset
        if (texture == null) {
            _sprites = []
            _gids = []
        } else {
            _sprites = [texture]
            _gids = [1]
        }
        _w = w
        _h = h
    }

    // Adds a new sprite to the tileset, index starting at gid
    add_tileset(sprite, gid) {
        _sprites.add(sprite)
        _gids.add(gid)
    }

    // Returns total width of the tileset
    width { _tileset[0].count * (_w) }

    // Returns total height of the tileset
    height { _tileset.count * (_h) }

    // Returns the width of the tileset in tiles
    tiles_wide { _tileset[0].count }

    // Returns the height of the tileset in tiles
    tiles_tall { _tileset.count }

    // Width of each tile
    tile_width { _w }

    // Height of each tile
    tile_height { _h }

    // Snaps an x position to the nearest grid spot on the left
    snap_left(hitbox, x, y) {
        x = Math.floor_to(x, _w)
        while (!collision(hitbox, x - 1, y) && hitbox.bb_left(x - 1, y) > 0) {
            x = x - _w
        }
        return x + hitbox.x_offset
    }

    // Same but right
    snap_right(hitbox, x, y) {
        x = Math.ceil_to(x, _w)
        while (!collision(hitbox, x + 1, y) && hitbox.bb_right(x + 1, y) < width) {
            x = x + _w
        }
        return x - hitbox.y_offset
    }

    // Same but up
    snap_up(hitbox, x, y) {
        y = Math.floor_to(y, _h)
        while (!collision(hitbox, x, y - 1) && hitbox.bb_top(x, y - 1) > 0) {
            y = y - _h
        }
        return y + hitbox.y_offset
    }

    // Same but down
    snap_down(hitbox, x, y) {
        y = Math.ceil_to(y, _h)
        while (!collision(hitbox, x, y + 1) && hitbox.bb_bottom(x, y + 1) < height) {
            y = y + _h
        }
        return y - hitbox.y_offset
    }

    // Copies from source tileset at [sx,sy] to [sx+sw,sy+sh] into this
    // one at [dx,dy]
    copy(source, sx, sy, sw, sh, dx, dy) {
        var x_counter = dx
        var y_counter = dy
        for (y in sy..(sh - 1)) {
            x_counter = dx
            for (x in sx..(sw - 1)) {
                this[x_counter, y_counter] = source[x, y]
            }
            y_counter = y_counter + 1
        }
    }

    // Copies all of source tileset into this one at x/y
    copy(source, x, y) {
        copy(source, 0, 0, source.tiles_wide, source.tiles_tall, x, y)
    }

    // Returns true if a hitbox's bounding box is colliding with a non-zero space
    // on the grid.
    collision(hitbox, x, y) {
        if (hitbox.no_hit) {
            return false
        }
        var bb = hitbox.bounding_box(x, y)
        x = x - hitbox.x_offset
        y = y - hitbox.y_offset
        // this is to account for bounding boxes that end on a new spot and shouldn't
        bb[2] = bb[2] % _w == 0 ? bb[2] - 0.1 : bb[2]
        bb[3] = bb[3] % _h == 0 ? bb[3] - 0.1 : bb[3]
        var vertices_wide = 1 + ((bb[2] - bb[0]) / _w).ceil
        var vertices_tall = 1 + ((bb[3] - bb[1]) / _h).ceil
        var vertex_x = x
        var vertex_y = y

        for (y_index in 0..(vertices_tall - 1)) {
            vertex_x = x
            for (x_index in 0..(vertices_wide - 1)) {
                if (this[(vertex_x / _w).floor, (vertex_y / _h).floor] != 0) {
                    return true
                }
                vertex_x = x_index == vertices_wide - 2 ? bb[2] : vertex_x + _w
            }
            vertex_y = y_index == vertices_tall - 2 ? bb[3] : vertex_y + _h
        }

        return false
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

        var cell = gid - selected_gid

        var draw_x = ((cell * _w) % _sprites[sprite].width).round
        var draw_y = (_h * ((cell * _w) / (_sprites[sprite].width)).floor).round

        Renderer.draw_texture_part(_sprites[sprite], x, y, draw_x, draw_y, _w, _h)
    }

    // Draws a specific portion of the tileset
    draw(x, y, w, h) {
        var y_offset = 0

        // Loop through each cell in the 2D grid
        for (i in _tileset[y..(y + h - 1)]) {
            var x_offset = 0
            for (cell in i[x..(x + w - 1)]) {
                if (cell != 0) {
                    internal_draw_sprite(cell, x_offset, y_offset)
                }
                x_offset = x_offset + _w
            }
            y_offset = y_offset + _h
        }
    }

    // Draws the tileset
    draw() {
        draw(0, 0, tiles_wide, tiles_tall)
    }

    // Draws a specific portion to a surface
    draw_to_surface(x, y, w, h) {
        var surf = Surface.new(w * _w, h * _h)
        Renderer.target = surf
        Renderer.clear_blank()
        draw(x, y, w, h)
        Renderer.target = Renderer.RENDER_TARGET_DEFAULT
        return surf
    }

    // Draws the whole thing to a surface
    draw_to_surface() {
        return draw_to_surface(0, 0, tiles_wide, tiles_tall)
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
            cwidth = Renderer.window_width
            cheight = Renderer.window_height
        }

        var tile_start_x = cx * parallax
        var tile_start_y = cy * parallax
        tile_start_x = (tile_start_x + (((cx - tile_start_x) / texture.width).floor * texture.width)) - texture.width
        tile_start_y = (tile_start_y + (((cy - tile_start_y) / texture.height).floor * texture.height)) - texture.height
        var horizontal = (cwidth / texture.width).ceil + 1
        var vertical = (cheight / texture.height).ceil + 1
        for (y in 0..vertical) {
            for (x in 0..horizontal) {
                Renderer.draw_texture(texture, tile_start_x + (x * texture.width), tile_start_y + (y * texture.height))
            }
        }
    }

    // These are down here cuz vs code syntax highlighting is not good
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
}

// Binary buffers
foreign class Buffer {
    // Creates a buffer
    construct new(size) {}

    // Creates a buffer from a file
    foreign static open(filename)

    // Same as above but from game.pak
    foreign static open_from_pak(filename)

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