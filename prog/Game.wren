import "lib/Levels" for Level
import "lib/Renderer" for Renderer
import "lib/Sprite" for Sprite
import "lib/Engine" for Engine
import "lib/Input" for Input
import "lib/BitmapFont" for BitmapFont
import "lib/Camera" for Camera
import "Assets" for Assets
import "lib/Save" for Save

class Game is Level {
    construct new() { }
    
    create() {
        System.print("game create")
        
        // Create the game camera and set fps
        _game_cam = Camera.new()
        _game_cam.set_w(400)
        _game_cam.set_h(300)
        _game_cam.set_zoom(1)
        _game_cam.set_w_on_screen(800)
        _game_cam.set_h_on_screen(600)
        _game_cam.update()
        Engine.cap_fps(60)

        // Load assets
        _font = BitmapFont.new("assets/font.png", 32, 128, 40, 70)
        _spr = Sprite.new("assets/sprite.png", 0, 0, 16, 16, 0.3, 4)
        _x = 100
        _y = 100

        var save = Save.new("save.bin")
        System.print(save.get_num("number"))
        System.print(save.get_string("string"))
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

        Renderer.lock_cameras(_game_cam)
        Renderer.draw_sprite(_spr, _x, _y)
        Renderer.draw_texture(Assets.logo(), 100, 100)
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        Renderer.draw_font(_font, "Font test", 0, 0)
        Renderer.unlock_cameras()
    }

    destroy() { }
}