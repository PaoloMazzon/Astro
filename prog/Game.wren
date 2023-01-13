import "lib/Levels" for Level
import "lib/Renderer" for Renderer
import "lib/Texture" for Texture
import "lib/Engine" for Engine
import "lib/Input" for Input
import "lib/BitmapFont" for BitmapFont

class Game is Level {
    construct new() {
        
    }
    
    create() {
        System.print("game create")
        Engine.cap_fps(60)
        _tex = Texture.new("assets/sprite.png")
        _x = 100
        _y = 100
        _font = BitmapFont.new("assets/font.png", 32, 128, 40, 70)
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

        Renderer.draw_texture(_tex, _x, _y)
        Renderer.draw_font(_font, "Font test", 0, 0)
    }

    destroy() {
        _tex.free()
        _font.free()
        System.print("game destroy")
    }
}