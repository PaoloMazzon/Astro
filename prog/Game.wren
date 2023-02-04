import "lib/Engine" for Engine, Level, Entity
import "lib/Input" for Input
import "lib/Renderer" for Renderer, Camera
import "lib/Drawing" for BitmapFont, Sprite, Surface
import "lib/Audio" for Audio
import "lib/Util" for Tileset, Hitbox, Math
import "Assets" for Assets
import "Tileset" for LEVEL_TILES

// Player
class Player is Entity {
    construct new() { super() }

    create(level, tiled_data) {
        sprite = Assets.player_idle
        hitbox = Hitbox.new_rectangle(sprite.width, sprite.height)
        _hspeed = 0
        _vspeed = 0
        _gravity = 0
        _jumps = 2 // resets when you touch the ground
        _scalex = 1
    }

    update(level) {
        // Speeds
        var speed = Engine.delta * 200
        _hspeed = 0

        // Left/right
        if (Input.keyboard_get_key(Input.KEY_A)) {
            _hspeed = _hspeed - speed
            _scalex = -1
        }
        if (Input.keyboard_get_key(Input.KEY_D)) {
            _hspeed = _hspeed + speed
            _scalex = 1
        }

        // Jumping
        if (level.tileset.collision(hitbox, x, y + 1)) {
            _jumps = 2
        }
        if (Input.keyboard_get_key_pressed(Input.KEY_SPACE) && _jumps > 0) {
            _jumps = _jumps - 1
            _gravity = -250
            y = y - 1
        }
        if (!level.tileset.collision(hitbox, x, y + 1)) {
            _gravity = _gravity + (800 * Engine.delta)
        } else {
            _gravity = 0
        }
        _vspeed = _gravity * Engine.delta

        // Handle collisions
        if (level.tileset.collision(hitbox, x + _hspeed, y)) {
            if (_hspeed.sign == 1) { x = Math.ceil_to(x, 16) - 0.001 }
            if (_hspeed.sign == -1) { x = Math.floor_to(x, 16) }
            _hspeed = 0
        }
        if (level.tileset.collision(hitbox, x, y + _vspeed)) {
            if (_vspeed.sign == 1) { y = Math.ceil_to(y, 16) - 0.001 }
            if (_vspeed.sign == -1) { y = Math.floor_to(y, 16) }
            _vspeed = 0
        }

        // Handle animations
        if (_hspeed != 0) {
            sprite = Assets.player_run
        } else {
            sprite = Assets.player_idle
        }

        if (_gravity < 0) {
            sprite = Assets.player_jump
        } else if (_gravity > 0 && !level.tileset.collision(hitbox, x, y + 1)) {
            sprite = Assets.player_fall
        }

        // Actually move the player
        x = Math.clamp(x + _hspeed, 0, level.tileset.width - sprite.width)
        y = Math.clamp(y + _vspeed, 0, level.tileset.height - sprite.height)
    }

    draw(level) {
        // Draw the player
        sprite.scale_x = _scalex
        if (sprite.scale_x == -1) {
            Renderer.draw_sprite(sprite, x + sprite.width, y)
        } else {
            Renderer.draw_sprite(sprite, x, y)
        }
    }
}

class Game is Level {
    construct new() { }

    tileset { _tileset }
    
    create() {
        super.create()
        System.print(Engine.info)
        System.print("game create")
        
        // Create the game camera
        _game_cam = Camera.new()
        _game_cam.width = 208
        _game_cam.height = 160
        _game_cam.zoom = 1
        _game_cam.w_on_screen = 832
        _game_cam.h_on_screen = 640
        _game_cam.update()

        // Load the tileset and draw it to a surface
        _tileset = load("assets/level0.tmj")[0]
        _tileset_surface = Surface.new(_tileset.width, _tileset.height)
        Renderer.set_target(_tileset_surface)
        _tileset.draw()
        Renderer.set_target(Renderer.RENDER_TARGET_DEFAULT)

        // Find the player entity
        _player = get_entities(Player)[0]
    }

    update() {
        // Center the camera on the player
        _game_cam.x = (_game_cam.x + (((_player.x + 16 - (_game_cam.width / 2)) - _game_cam.x) * 5 * Engine.delta))
        _game_cam.y = (_game_cam.y + (((_player.y + 16 - (_game_cam.height / 2)) - _game_cam.y) * 5 * Engine.delta))
        _game_cam.update()

        // Render game world
        Renderer.lock_cameras(_game_cam)
        Tileset.draw_tiling_background(Assets.bg_blue, 0.7, _game_cam)
        Renderer.draw_texture(_tileset_surface, 0, 0)
        Renderer.draw_sprite(Assets.cherries, 256, 208)
        Renderer.draw_sprite(Assets.kiwi, 180, 152)
        Renderer.draw_sprite(Assets.bananas, 64, 208)
        super.update() // update all entities

        // Render UI
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        //Renderer.draw_font(Assets.font, "FPS: " + Engine.fps.toString, 2, 0)
        Renderer.unlock_cameras()
    }

    destroy() {
        super.destroy()
    }
}