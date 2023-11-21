import "random" for Random
import "lib/Util" for CHitbox

class Collider is Entity {
    construct new() { super() }
    static TYPE_POLYGON { 0 }
    static TYPE_CIRCLE { 1 }
    static TYPE_RECTANGLE { 2 }

    create(level, tiled_data) {
        super.create(level, tiled_data)
        _type = level.rng.int(3)
        if (_type == Collider.TYPE_POLYGON) {
            hitbox = CHitbox.new_polygon([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
            _polygon = Polygon.create([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
        } else if (_type == Collider.TYPE_CIRCLE) {
            hitbox = CHitbox.new_circle(level.rng.int(15, 30))
        } else if (_type == Collider.TYPE_RECTANGLE) {
            hitbox = CHitbox.new_rectangle(level.rng.int(20, 40), level.rng.int(20, 40))
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

    profile_begin(title, trials) {
        _title_length = title.count
        _trials = trials
        System.print("--------------------%(title)--------------------")
        _start_time = Engine.time
    }

    profile_end() {
        var total_time = Engine.time - _start_time
        var average = (total_time * 1000000) / _trials
        System.print("Trial Count: %(_trials)")
        System.print("Total Time: %(total_time)")
        System.print("Average Time/Trial: %(average)µs")
        var s = "----------------------------------------"
        for (i in 0..(_title_length - 1)) {
            s = s + "-"
        }
        System.print(s)
    }

    create() {
        super.create()
        _rng = Random.new()

        for (i in 0..100) {
            add_entity(Collider)
        }
        
        _hit1 = CHitbox.new_polygon([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
        _polygon = Polygon.create([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
        //_hit2 = CHitbox.new_circle(30)
        _hit2 = CHitbox.new_polygon([[0, 0], [30, 0], [30, 30], [0, 30]])
        _polygon2 = Polygon.create([[0, 0], [30, 0], [30, 30], [0, 30]])
        //_hit2 = CHitbox.new_rectangle(30, 30)
        /*if (_type == Collider.TYPE_POLYGON) {
            hitbox = CHitbox.new_polygon([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
            _polygon = Polygon.create([[10, 4], [4, 18], [3, 34], [17, 43], [42, 45], [47, 25], [41, 10]])
        } else if (_type == Collider.TYPE_CIRCLE) {
            hitbox = CHitbox.new_circle(level.rng.int(15, 30))
        } else if (_type == Collider.TYPE_RECTANGLE) {
            hitbox = CHitbox.new_rectangle(level.rng.int(20, 40), level.rng.int(20, 40))
        }*/
    }

    pre_frame() {
        super.pre_frame()
    }

    update() {
        //super.update()
        
        Renderer.draw_font(null, Engine.fps.toString, 0, 0)

        var mouse_pos = Mouse.position(null)
        var dir = Math.point_angle(250, 250, mouse_pos[0], mouse_pos[1])
        var dist = Math.point_distance(250, 250, mouse_pos[0], mouse_pos[1])
        Renderer.draw_line(250, 250, 250 + Math.cast_x(dist, dir), 250 + Math.cast_y(dist, dir))
        Renderer.draw_circle_outline(250, 250, 5, 1)
        Renderer.draw_circle_outline(mouse_pos[0], mouse_pos[1], 5, 1)
        Renderer.draw_rectangle_outline(250 - 50, 250 - 50, 100, 100, dir, 50, 50, 1)
        Renderer.draw_circle_outline(250 + Math.cast_x(dist * Math.serp(Engine.time / 2, 0, 1), dir), 250 + Math.cast_y(dist * Math.serp(Engine.time / 2, 0, 1), dir), 5, 1)

        if (_hit1.collision(800, 400, mouse_pos[0], mouse_pos[1], _hit2)) {
            Renderer.colour_mod = [0.8, 0.1, 0.1, 1]
        }
        Renderer.draw_polygon(_polygon, 800, 400)
        Renderer.draw_polygon(_polygon2, mouse_pos[0], mouse_pos[1])
        //Renderer.draw_circle(mouse_pos[0], mouse_pos[1], _hit2.r)
        //Renderer.draw_rectangle(mouse_pos[0], mouse_pos[1], _hit2.w, _hit2.h, 0, 0, 0)
        Renderer.colour_mod = [1, 1, 1, 1]
    }

    destroy() {
        super.destroy()
    }
}