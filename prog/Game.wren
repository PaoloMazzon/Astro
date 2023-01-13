import "lib/Levels" for Level
import "lib/Renderer" for Renderer
import "lib/Texture" for Texture
import "lib/Camera" for Camera

class Game is Level {
    construct new() {
        
    }
    
    create() {
        System.print("game create")
        _tex = Texture.new("assets/sprite.png")
    }

    update() {
        Renderer.draw_circle(200, 200, 10)
        Renderer.draw_texture(_tex, 100, 100)
    }

    destroy() {
        _tex.free()
        System.print("game destroy")
    }
}