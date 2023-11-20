import "Game" for Game

var renderer_config = {
	"window_title": "Game",
	"window_width": 1280,
	"window_height": 720,
	"fullscreen": false,
	"msaa": Renderer.MSAA_32X,
	"screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
	"filter_type": Renderer.FILTER_TYPE_NEAREST
}

System.print(Engine.info)
Engine.fps_limit = 0

var minimum_load_time = 0
var start_level = Game.new()