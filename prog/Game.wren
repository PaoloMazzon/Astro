import "lib/Engine" for Engine, Level, Entity
import "lib/Input" for Input
import "lib/Renderer" for Renderer, Camera
import "lib/Drawing" for BitmapFont, Sprite, Surface
import "lib/File" for File
import "lib/Audio" for Audio
import "lib/Util" for Tileset
import "Assets" for Assets

var LEVEL_TILES = [
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00],
    [01, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 03],
    [45, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 47]
]

// Player
class Player is Entity {
    construct new() {}

    x { _x }
    y { _y }

    create(level) {
        _sprite = Assets.player_idle
        _x = 208 / 2
        _y = 160 / 2
    }

    update(level) {
        var speed = Engine.delta * 100
        if (Input.keyboard_get_key(Input.KEY_A)) {
            _x = _x - speed
        }
        if (Input.keyboard_get_key(Input.KEY_D)) {
            _x = _x + speed
        }
        if (Input.keyboard_get_key(Input.KEY_W)) {
            _y = _y - speed
        }
        if (Input.keyboard_get_key(Input.KEY_S)) {
            _y = _y + speed
        }

        // Center the camera on the player
        //var game_cam = level.game_cam
        //game_cam.set_x(game_cam.get_x() + (((_x + 16 - (game_cam.get_w() / 2)) - game_cam.get_x()) * 5 * Engine.delta()))
        //game_cam.set_y(game_cam.get_y() + (((_y + 16 - (game_cam.get_h() / 2)) - game_cam.get_y()) * 5 * Engine.delta()))
        //game_cam.update()

        // Draw the player
        Renderer.draw_sprite(_sprite, _x, _y)
    }
}

class Game is Level {
    construct new() { }
    
    create() {
        super.create()
        System.print(Engine.info)
        System.print("game create")
        
        // Create the game camera and set fps
        _game_cam = Camera.new()
        _game_cam.width = 208
        _game_cam.height = 160
        _game_cam.zoom = 1
        _game_cam.w_on_screen = 832
        _game_cam.h_on_screen = 640
        _game_cam.update()
        //Engine.cap_fps(60)

        // Load the tileset and draw it to a surface
        _tileset = Tileset.new(LEVEL_TILES, Assets.terrain, 0, 0)
        _tileset_surface = Surface.new(_tileset.width, _tileset.height)
        Renderer.set_target(_tileset_surface)
        _tileset.draw()
        Renderer.set_target(Renderer.RENDER_TARGET_DEFAULT)

        // Create the player entity and save it
        _player = add_entity(Player)
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
        Renderer.draw_sprite(Assets.cherries, 200, 50)
        super.update() // update all entities

        // Render UI
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        Renderer.draw_font(Assets.font, "FPS: " + Engine.fps.toString, 2, 0)
        Renderer.unlock_cameras()
    }

    destroy() {
        super.destroy()
    }
}