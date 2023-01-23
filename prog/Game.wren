import "lib/Engine" for Engine, Level
import "lib/Input" for Input
import "lib/Renderer" for Renderer, Camera
import "lib/Drawing" for BitmapFont, Sprite
import "lib/File" for File
import "lib/Audio" for Audio
import "Assets" for Assets

class Game is Level {
    construct new() { }
    
    create() {
        System.print(Engine.info())
        System.print("game create")
        
        // Create the game camera and set fps
        _game_cam = Camera.new()
        _game_cam.set_w(400)
        _game_cam.set_h(300)
        _game_cam.set_zoom(1)
        _game_cam.set_w_on_screen(800)
        _game_cam.set_h_on_screen(600)
        _game_cam.update()
        //Engine.cap_fps(60)

        _x = 100
        _y = 100
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

        // Render game world
        Renderer.lock_cameras(_game_cam)
        Renderer.draw_sprite(Assets.sprite, _x, _y)
        Renderer.draw_texture(Assets.logo, 100, 100)

        // Render UI
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        Renderer.draw_font(Assets.font, "FPS: " + Engine.fps().toString, 0, 0)
        Renderer.unlock_cameras()
    }

    destroy() { }
}