import "lib/Renderer" for Renderer
import "Game" for Game

var config = {
	"window_title": "Game",
	"window_width": 800,
	"window_height": 600,
	"msaa": Renderer.MSAA_32X,
	"screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
	"filter_type": Renderer.FILTER_TYPE_NEAREST
}

var start = Game