class Player is Entity {
    construct new() { super() }

    create(level, tiled_data) {
        super.create(level, tiled_data)
    }

    update(level) {
        super.update(level)

        var speed = 1
        level.camera.x = Math.clamp(level.camera.x + (speed * Keyboard.keys_as_axis(Keyboard.KEY_LEFT, Keyboard.KEY_RIGHT)), 0, level.tileset.width - (16 * 8))
        level.camera.y = Math.clamp(level.camera.y + (speed * Keyboard.keys_as_axis(Keyboard.KEY_UP, Keyboard.KEY_DOWN)), 0, level.tileset.height - (15 * 8))
        level.camera.update()
    }
}