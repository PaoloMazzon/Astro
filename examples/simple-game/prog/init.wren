import "Game" for Game

var renderer_config = {
	"window_title": "Game",
	"window_width": 16 * 8 * 6,
	"window_height": 15 * 8 * 6,
	"fullscreen": false,
	"msaa": Renderer.MSAA_1X,
	"screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
	"filter_type": Renderer.FILTER_TYPE_NEAREST
}

var window_icon = "assets/logo.png"

System.print(Engine.info)
Engine.fps_limit = 60

var start_level = Game.new()