import "lib/Levels" for Level
import "lib/Renderer" for Renderer
import "lib/Texture" for Texture
import "lib/Runtime" for Runtime
import "lib/Input" for Input

class Game is Level {
    construct new() {
        
    }
    
    create() {
        System.print("game create")
        Runtime.cap_fps(60)
        _tex = Texture.new("assets/sprite.png")
        _x = 100
        _y = 100
    }

    update() {
        if (Input.keyboard_get_key(Input.KEY_A)) {
            _x = _x - 0.5
        }
        if (Input.keyboard_get_key(Input.KEY_D)) {
            _x = _x + 0.5
        }
        if (Input.keyboard_get_key(Input.KEY_W)) {
            _y = _y - 0.5
        }
        if (Input.keyboard_get_key(Input.KEY_S)) {
            _y = _y + 0.5
        }

        Renderer.draw_circle(200, 200, 10)
        Renderer.draw_texture(_tex, _x, _y)
    }

    destroy() {
        _tex.free()
        System.print("game destroy")
    }
}