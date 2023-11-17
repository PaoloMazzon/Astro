import "random" for Random

class Collider is Entity {
    construct new() { super() }

    create(level, tiled_data) {
        super.create(level, tiled_data)
        hitbox = Hitbox.new_polygon([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
        _polygon = Polygon.create([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
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
        Renderer.draw_polygon(_polygon, x, y)
        Renderer.colour_mod = [1, 1, 1, 1]
    }
}

class Game is Level {
    construct new() { }
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
        super.update()
        
        //Renderer.draw_font(null, Engine.fps.toString, 0, 0)
    }

    destroy() {
        super.destroy()
    }
}