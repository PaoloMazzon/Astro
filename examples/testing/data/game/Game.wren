import "random" for Random

class Game is Level {
    construct new() { }
    rng { _rng }

    create() {
        super.create()
        _rng = Random.new()
    }

    pre_frame() {
        super.pre_frame()
    }

    update() {
        super.update()
        
        Renderer.draw_font(null, Engine.fps.toString, 0, 0)

        var mouse_pos = Mouse.position(null)
        var dir = Math.point_angle(250, 250, mouse_pos[0], mouse_pos[1])
        var dist = Math.point_distance(250, 250, mouse_pos[0], mouse_pos[1])
        Renderer.draw_line(250, 250, 250 + Math.cast_x(dist, dir), 250 + Math.cast_y(dist, dir))
        Renderer.draw_circle_outline(250, 250, 5, 1)
        Renderer.draw_circle_outline(mouse_pos[0], mouse_pos[1], 5, 1)
        Renderer.draw_rectangle_outline(250 - 50, 250 - 50, 100, 100, dir, 50, 50, 1)
        Renderer.draw_circle_outline(250 + Math.cast_x(dist * Math.serp(Engine.time / 2, 0, 1), dir), 250 + Math.cast_y(dist * Math.serp(Engine.time / 2, 0, 1), dir), 5, 1)
    }

    destroy() {
        super.destroy()
    }
}