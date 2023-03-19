// Player
class Player is Entity {
    construct new() { super() }

    // For interpolating between two frames for smooth animation between updates due to the fixed-timestep
    int_x { Math.lerp(Engine.timestep_percent, prev_x, x) }
    int_y { Math.lerp(Engine.timestep_percent, prev_y, y) }

    create(level, tiled_data) {
        sprite = Assets.spr_player_idle
        hitbox = Hitbox.new_rectangle(sprite.width - 4, sprite.height - 1)
        hitbox.x_offset = -2
        hitbox.y_offset = -1
        _hspeed = 0
        _vspeed = 0
        _gravity = 0
        _jumps = 2 // resets when you touch the ground
        _scalex = 1
    }

    update(level) {
        // Speeds
        var speed = 8
        _hspeed = 0

        // Left/right
        if (Keyboard.key(Keyboard.KEY_A) || Gamepad.button(0, Gamepad.BUTTON_DPAD_LEFT)) {
            _hspeed = _hspeed - speed
            _scalex = -1
        }
        if (Keyboard.key(Keyboard.KEY_D) || Gamepad.button(0, Gamepad.BUTTON_DPAD_RIGHT)) {
            _hspeed = _hspeed + speed
            _scalex = 1
        }

        // Jumping
        if (level.tileset.collision(hitbox, x, y + 1) && _jumps < 2) {
            _jumps = 2
            Gamepad.rumble(0, 0.7, 150)
        }
        if ((Keyboard.key_pressed(Keyboard.KEY_SPACE) || Gamepad.button_pressed(0, Gamepad.BUTTON_A)) && _jumps > 0) {
            if (_jumps == 1) {
                sprite = Assets.spr_player_double_jump
            } else {
                sprite = Assets.spr_player_jump
            }
            _jumps = _jumps - 1
            _vspeed = -9
        }
        _vspeed = _vspeed + 0.8

        // Handle collisions
        if (level.tileset.collision(hitbox, x + _hspeed, y)) {
            while (!level.tileset.collision(hitbox, x + _hspeed.sign, y)) {
                x = x + _hspeed.sign
            }
            _hspeed = 0
        }
        if (level.tileset.collision(hitbox, x, y + _vspeed)) {
            while (!level.tileset.collision(hitbox, x, y + _vspeed.sign)) {
                y = y + _vspeed.sign
            }
            _vspeed = 0
        }

        // Handle animations
        if (_hspeed != 0 && _vspeed == 0) {
            sprite = Assets.spr_player_run
        } else if (_vspeed == 0) {
            sprite = Assets.spr_player_idle
        }

        if (_vspeed > 0) {
            sprite = Assets.spr_player_fall
        }

        // Actually move the player
        x = Math.clamp(x + _hspeed, 0, level.tileset.width - sprite.width)
        y = Math.clamp(y + _vspeed, 0, level.tileset.height - sprite.height)
    }

    draw(level) {
        // Draw the player
        sprite.scale_x = _scalex
        if (sprite.scale_x == -1) {
            Renderer.draw_sprite(sprite, int_x + sprite.width, int_y)
        } else {
            Renderer.draw_sprite(sprite, int_x, int_y)
        }
    }
}

class Game is Level {
    construct new() { }

    tileset { _tileset }
    
    create() {
        super.create()
        System.print("game create")
        
        // Create the game camera
        _game_cam = Camera.new()
        _game_cam.width = 208
        _game_cam.height = 160
        _game_cam.zoom = 1
        _game_cam.w_on_screen = 832
        _game_cam.h_on_screen = 640
        _game_cam.update()
        Engine.timestep = 30

        // Load the level and tilesets
        _tilesets = load("assets/level0.tmj")
        _tileset = _tilesets["collisions"]

        // Pre-load the image of the foreground tileset
        _foreground_surface = Surface.new(_tileset.width, _tileset.height)
        Renderer.target = _foreground_surface
        Renderer.blend_mode = Renderer.BLEND_MODE_NONE
        Renderer.colour_mod = [0, 0, 0, 0]
        Renderer.clear()
        Renderer.blend_mode = Renderer.BLEND_MODE_BLEND
        Renderer.colour_mod = [1, 1, 1, 1]
        _tileset.draw()
        Renderer.target = Renderer.RENDER_TARGET_DEFAULT

        // Pre-load the image of the backdrop tileset
        _backdrop_surface = Surface.new(_tilesets["background"].width, _tilesets["background"].height)
        Renderer.target = _backdrop_surface
        Renderer.blend_mode = Renderer.BLEND_MODE_NONE
        Renderer.colour_mod = [0, 0, 0, 0]
        Renderer.clear()
        Renderer.blend_mode = Renderer.BLEND_MODE_BLEND
        Renderer.colour_mod = [1, 1, 1, 1]
        _tilesets["background"].draw()
        Renderer.target = Renderer.RENDER_TARGET_DEFAULT

        // Pre-load the image of the background tileset
        _background_surface = Surface.new(_tilesets["distance"].width, _tilesets["distance"].height)
        Renderer.target = _background_surface
        Renderer.blend_mode = Renderer.BLEND_MODE_NONE
        Renderer.colour_mod = [0, 0, 0, 0]
        Renderer.clear()
        Renderer.blend_mode = Renderer.BLEND_MODE_BLEND
        Renderer.colour_mod = [1, 1, 1, 1]
        _tilesets["distance"].draw()
        Renderer.target = Renderer.RENDER_TARGET_DEFAULT

        // Find the player entity
        _player = get_entity(Player)
    }

    update() {
        // Center the camera on the player
        _game_cam.x = (_game_cam.x + (((_player.int_x + 16 - (_game_cam.width / 2)) - _game_cam.x) * 0.6 * Engine.delta))
        _game_cam.y = (_game_cam.y + (((_player.int_y + 16 - (_game_cam.height / 2)) - _game_cam.y) * 0.6 * Engine.delta))
        _game_cam.update()

        // Render game world
        Renderer.lock_cameras(_game_cam)
        Tileset.draw_tiling_background(Assets.tex_bg_blue, 0.7, _game_cam)
        Renderer.colour_mod = [0.4, 0.4, 0.4, 1]
        Renderer.draw_texture(_background_surface, _game_cam.x * 0.18, _game_cam.y * 0.18)
        Renderer.colour_mod = [0.5, 0.5, 0.5, 1]
        Renderer.draw_texture(_backdrop_surface, 0, 0)
        Renderer.colour_mod = Renderer.COLOUR_DEFAULT
        Renderer.draw_texture(_foreground_surface, 0, 0)

        super.update() // update all entities
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
    }

    destroy() {
        super.destroy()
    }
}