import "lib/Levels" for Level
import "lib/Renderer" for Renderer
import "lib/Texture" for Texture

class Game is Level {
    construct new() {

    }
    
    create() {
        System.print("game create")
        var tex = Texture.new("assets/sprite.png")
        System.print(tex.width())
    }

    update() {
        Renderer.draw_circle(200, 200, 10)
    }

    destroy() {
        System.print("game destroy")
    }
}