import "lib/Levels" for Level
import "lib/Renderer" for Renderer

class Game is Level {
    construct new() {

    }
    
    create() {
        System.print("create")
    }

    update() {
        Renderer.draw_circle(200, 200, 10)
    }

    destroy() {
        System.print("destroy")
    }
}