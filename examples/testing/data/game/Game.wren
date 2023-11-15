class Game is Level {
    construct new() { }

    create() {
        super.create()
        _x1 = 300
        _y1 = 250
        _hit1 = Hitbox.new_circle(200)
        /*Hitbox.new_polygon([[10, 17], [125, 29], [135, 135], [83, 139], [37, 96]])
        _vertices1 = Polygon.create([
            [10, 17],
            [125, 29],
            [135, 135],
            [83, 139],
            [37, 96],
            [10, 17]
        ])*/
        _x2 = 0
        _y2 = 0
        _hit2 = Hitbox.new_polygon([[6, 117], [36, 19], [106, 11], [143, 58], [121, 136], [41, 142]])
        _vertices2 = Polygon.create([
            [6, 117],
            [36, 19],
            [106, 11],
            [143, 58],
            [121, 136],
            [41, 142]
        ])
    }

    pre_frame() {
        super.pre_frame()
    }

    update() {
        super.update()
        
        var pos = Mouse.position(null)
        _x2 = pos[0]
        _y2 = pos[1]
        if (_hit1.collision(_x1, _y1, _x2, _y2, _hit2)) {
            Renderer.colour_mod = [0.9, 0.1, 0.1, 1]
        }
        Renderer.draw_circle_outline(_x1, _y1, 200, 1)
        Renderer.draw_circle(_x1, _y1, 2)
        //Renderer.draw_polygon(_vertices1, _x1, _y1)
        Renderer.draw_polygon(_vertices2, _x2, _y2)
        Renderer.colour_mod = [1, 1, 1, 1]
        var bb = _hit1.bounding_box(_x1, _y1)
        Renderer.draw_rectangle_outline(bb[0], bb[1], bb[2] - bb[0], bb[3] - bb[1], 0, 0, 0, 1)
        bb = _hit2.bounding_box(_x2, _y2)
        Renderer.draw_rectangle_outline(bb[0], bb[1], bb[2] - bb[0], bb[3] - bb[1], 0, 0, 0, 1)
    }

    destroy() {
        super.destroy()
    }
}