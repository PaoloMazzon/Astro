import "Game" for Game

var renderer_config = {
	"window_title": "Game",
	"window_width": 832,
	"window_height": 640,
	"fullscreen": false,
	"msaa": Renderer.MSAA_32X,
	"screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
	"filter_type": Renderer.FILTER_TYPE_NEAREST
}

var window_icon = "assets/logo.png"

Engine.fps_limit = 60
System.print(Engine.info)

var start_level = Game.new()