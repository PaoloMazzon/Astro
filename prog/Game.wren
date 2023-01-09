import "lib/Levels" for Level
import "lib/Renderer" for Renderer

class Game is Level {
    update() {
        draw_circle(200, 200, 10)
    }
}