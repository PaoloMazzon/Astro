import "random" for Random

class Collider is Entity {
    construct new() { super() }
    static TYPE_POLYGON { 0 }
    static TYPE_CIRCLE { 1 }
    static TYPE_RECTANGLE { 2 }

    create(level, tiled_data) {
        super.create(level, tiled_data)
        _type = level.rng.int(3)
        if (_type == Collider.TYPE_POLYGON) {
            hitbox = Hitbox.new_polygon([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
            _polygon = Polygon.create([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
        } else if (_type == Collider.TYPE_CIRCLE) {
            hitbox = Hitbox.new_circle(level.rng.int(15, 30))
        } else if (_type == Collider.TYPE_RECTANGLE) {
            hitbox = Hitbox.new_rectangle(level.rng.int(20, 40), level.rng.int(20, 40))
        }
        x = level.rng.int(1280 - hitbox.bb_right(0, 0))
        y = level.rng.int(720 - hitbox.bb_bottom(0, 0))
        var factor = level.rng.sample([-2, -1, 1, 2])
        _x_speed = 100 * factor
        _y_speed = 50 * factor
    }

    update(level) {
        super.update(level)
        if (hitbox.bb_right(x, y) + (_x_speed * Engine.delta) > 1280 || hitbox.bb_left(x, y) + (_x_speed * Engine.delta) < 0) {
            _x_speed = -_x_speed
        }
        if (hitbox.bb_bottom(x, y) + (_y_speed * Engine.delta) > 720 || hitbox.bb_top(x, y) + (_y_speed * Engine.delta) < 0) {
            _y_speed = -_y_speed
        }
        
        x = x + (_x_speed * Engine.delta)
        y = y + (_y_speed * Engine.delta)
    }

    draw(level) {
        super.draw(level)
        if (level.entity_collision(this, Collider)) {
            Renderer.colour_mod = [0.8, 0.1, 0.1, 1]
        }
        if (_type == Collider.TYPE_POLYGON) {
            Renderer.draw_polygon(_polygon, x, y)
        } else if (_type == Collider.TYPE_CIRCLE) {
            Renderer.draw_circle(x, y, hitbox.r)
        } else if (_type == Collider.TYPE_RECTANGLE) {
            Renderer.draw_rectangle(x, y, hitbox.w, hitbox.h, 0, 0, 0)
        }
        Renderer.colour_mod = [1, 1, 1, 1]
    }
}

class Game is Level {
    construct new() {
        _start_time = 0
        _title_length = 0
        _trials
    }
    rng { _rng }

    create() {
        super.create()
        _rng = Random.new()

        for (i in 0..100) {
            add_entity(Collider)
        }
    }

    pre_frame() {
        super.pre_frame()
    }

    update() {
        //super.update()

        var mouse_pos = Mouse.position(null)
        var dir = Math.point_angle(250, 250, mouse_pos[0], mouse_pos[1])
        var dist = Math.point_distance(250, 250, mouse_pos[0], mouse_pos[1])
        Renderer.draw_line(250, 250, 250 + Math.cast_x(dist, dir), 250 + Math.cast_y(dist, dir))
        Renderer.draw_circle_outline(250, 250, 5, 1)
        Renderer.draw_circle_outline(mouse_pos[0], mouse_pos[1], 5, 1)
        Renderer.draw_rectangle_outline(250 - 50, 250 - 50, 100, 100, dir, 50, 50, 1)
        Renderer.draw_circle_outline(250 + Math.cast_x(dist * Math.serp(Engine.time / 2, 0, 1), dir), 250 + Math.cast_y(dist * Math.serp(Engine.time / 2, 0, 1), dir), 5, 1)
        Renderer.draw_font(Assets.fnt_merriweather48, "The quick brown fox jumps over the lazy dog.", 0, 0)
        Renderer.draw_font(Assets.fnt_merriweather32, "The quick brown fox jumps over the lazy dog.", 0, 50)
        Renderer.draw_font(Assets.fnt_merriweather24, "The quick brown fox jumps over the lazy dog.", 0, 50 + 34)
        Renderer.draw_font(Assets.fnt_merriweather16, "The quick brown fox jumps over the lazy dog.", 0, 50 + 34 + 26)
    }

    destroy() {
        super.destroy()
    }
}