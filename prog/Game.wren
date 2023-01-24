import "lib/Engine" for Engine, Level
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

class Game is Level {
    construct new() { }
    
    create() {
        System.print(Engine.info())
        System.print("game create")
        
        // Create the game camera and set fps
        _game_cam = Camera.new()
        _game_cam.set_w(208)
        _game_cam.set_h(160)
        _game_cam.set_zoom(1)
        _game_cam.set_w_on_screen(832)
        _game_cam.set_h_on_screen(640)
        _game_cam.update()
        //Engine.cap_fps(60)

        // Load the tileset
        _tileset = Tileset.new(LEVEL_TILES, Assets.terrain, 0, 0)

        _x = 208 / 2
        _y = 160 / 2
    }

    update() {
        var speed = Engine.delta() * 100
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
        if (Input.keyboard_get_key_pressed(Input.KEY_SPACE)) {
            Audio.play(Assets.sound, false, 1, 1)
        }

        // Center the camera on the player
        _game_cam.set_x(_game_cam.get_x() + (((_x + 16 - (_game_cam.get_w() / 2)) - _game_cam.get_x()) * 5 * Engine.delta()))
        _game_cam.set_y(_game_cam.get_y() + (((_y + 16 - (_game_cam.get_h() / 2)) - _game_cam.get_y()) * 5 * Engine.delta()))
        _game_cam.update()

        // Render game world
        Renderer.lock_cameras(_game_cam)
        Tileset.draw_tiling_background(Assets.bg_blue, 0.7, _game_cam)
        _tileset.draw()
        Renderer.draw_sprite(Assets.player_idle, _x, _y)

        // Render UI
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        Renderer.draw_font(Assets.font, "FPS: " + Engine.fps().toString, 2, 0)
        Renderer.unlock_cameras()
    }

    destroy() { }
}