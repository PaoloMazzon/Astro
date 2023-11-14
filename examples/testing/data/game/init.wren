import "Game" for Game

var renderer_config = {
	"window_title": "Game",
	"window_width": 800,
	"window_height": 600,
	"fullscreen": false,
	"msaa": Renderer.MSAA_1X,
	"screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
	"filter_type": Renderer.FILTER_TYPE_NEAREST
}

System.print(Engine.info)
Engine.fps_limit = 60

var start_level = Game.new()