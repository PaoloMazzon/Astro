class Game is Level {
    construct new() { }

    create() {
        super.create()
        _x1 = 300
        _y1 = 300
        _hit1 = Hitbox.new_polygon([[10, 17], [125, 29], [135, 135], [83, 139], [37, 96]])
        _vertices1 = Polygon.create([
            [10, 17],
            [125, 29],
            [135, 135],
            [83, 139],
            [37, 96],
            [10, 17]
        ])
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
        Renderer.draw_polygon(_vertices1, _x1, _y1)
        Renderer.draw_polygon(_vertices2, _x2, _y2)
    }

    destroy() {
        super.destroy()
    }
}