class Player is Entity {
    construct new() { super() }

    create(level, tiled_data) {
        super.create(level, tiled_data)
        sprite = Assets.spr_player_idle
        hitbox = Hitbox.new_rectangle(8, 8)
        _hspeed = 0
        _vspeed = 0
        _facing = 1
    }

    update(level) {
        super.update(level)
        var speed = 1.2
        var gravity = 0.18
        var jump_speed = 3
        var ladder_speed = 1
        var touching_ladder = colliding(level.ladder_tileset)

        // Move the player left and right
        _hspeed = Keyboard.keys_as_axis(Keyboard.KEY_LEFT, Keyboard.KEY_RIGHT) * speed

        // Adjust the facing direction for animation purposes
        if (_hspeed > 0) {
            _facing = 1
        } else if (_hspeed < 0) {
            _facing = -1
        }

        // Interact with ladders
        if (!touching_ladder) {
            // Player jump (if touching the ground)
            if (Keyboard.key_pressed(Keyboard.KEY_UP) && level.tileset.collision(hitbox, x, y + 1)) {
                _vspeed = -jump_speed
            }
            _vspeed = _vspeed + gravity
        } else {
            // If the user is touching a ladder they may simply move up or down
            _vspeed = Keyboard.keys_as_axis(Keyboard.KEY_UP, Keyboard.KEY_DOWN) * ladder_speed
        }

        // Handle player collisions by snapping to a wall if we're about to collide
        if (colliding(level.tileset, x + _hspeed, y)) {
            x = _hspeed > 0 ? level.tileset.snap_right(hitbox, x, y) : level.tileset.snap_left(hitbox, x, y)
            _hspeed = 0
        }
        x = Math.clamp(x + _hspeed, 0, level.tileset.width - 8)
        if (colliding(level.tileset, x, y + _vspeed)) {
            y = _vspeed > 0 ? level.tileset.snap_down(hitbox, x, y) : level.tileset.snap_up(hitbox, x, y)
            _vspeed = 0
        }
        y = Math.clamp(y + _vspeed, 0, level.tileset.height - 8)

        // Center the camera towards the player slowly
        var cameraSpeed = 0.15
        level.camera.x = Math.clamp(level.camera.x + (((x - (level.game_width / 2)) - level.camera.x) * cameraSpeed), 0, level.tileset.width - level.game_width)
        level.camera.y = Math.clamp(level.camera.y + (((y - (level.game_height / 2)) - level.camera.y) * cameraSpeed), 0, level.tileset.height - level.game_height)
        level.camera.update()

        // Change animation depending on what the player is doing
        if (!colliding(level.tileset, x, y + 1)) {
            sprite = Assets.spr_player_jump
        } else if (_hspeed != 0) {
            sprite = Assets.spr_player_walk
        } else {
            sprite = Assets.spr_player_idle
        }
        if (_facing == 1) {
            sprite.scale_x = 1
            sprite.origin_x = 0
        } else {
            sprite.scale_x = -1
            sprite.origin_x = 8
        }
    }
}