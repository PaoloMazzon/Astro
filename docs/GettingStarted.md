# Getting Started
To launch a game in Astro you need 3 things:
 1. The runtime (`Astro.exe` and related files)
 2. Astro standard library
 3. An `prog/init.wren` file
 
 We'll cover these three things first then describe how to use the engine.
 
 ## Runtime
 You may either build the C runtime yourself or download a binary if their available. From there
 you only need to run the program and it will take care of the majority of things, bar the next
 two points.
 
 ## Astro Standard Library
 The standard library, located at `prog/lib/*` of this repo has all of the bindings for the engine
 as well as some other things you'll need to use the engine. The runtime always expect it to be
 located at `prog/lib/*`.
 
 ## `prog/init.wren`
 You `init.wren` file is the "entry point" of your game. It must contain the two variables `renderer_config`
 and `start_level`. `start_level` is an instance of `lib/Engine::Level` that the engine will use as the
 starting level. `renderer_config` must be a map containing specific keys.
 
    import "lib/Renderer" for Renderer
    import "Game" for Game
    
    var renderer_config = {
        "window_title": "Game",
        "window_width": 800,
        "window_height": 600,
        "fullscreen": false,
        "msaa": Renderer.MSAA_8X,
        "screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
        "filter_type": Renderer.FILTER_TYPE_NEAREST
    }
    
    var start_level = Game.new()

As you can see, the `Renderer` class provides values to use for msaa, screen_mode, and filter_type.

> 📝 When the `init.wren` file is loaded into Astro, the rest of the engine is yet to initialize.
> In practice this means you may **not** call most engine-related methods (anything from
> `lib/Engine`, anything drawing related, anything input related).    
  
After Astro loads this file and extracts the data it wants from it, it will then
initialize the rest of the engine and call the level's `create()` function. From
there your game starts. See the [API Reference](API.md) for help and check the
[Asset Compiler](AssetCompiler.md) for info on how to automate asset management.

`prog/init.wren` Variables
--------------------------
`start_level` must be a new instance of a class that implements all methods of `Level`. The runtime
will use that class to start the game from.

`renderer_config` must be a map of details on how to initialize the renderer. All of the following
keys must be provided:

 + `window_title` - A string which will be the window's title
 + `window_width` - Window width in pixels
 + `window_height` - Window height in pixels
 + `fullscreen` - Use fullscreen, width/height ignored in this case
 + `msaa` - Use `Renderer.MSAA_*` to select your preferred MSAA (if the selected MSAA isn't available
 the next highest available MSAA will be used instead)
 + `screen_mode` - Use `Renderer.SCREEN_MODE_*` to select your preferred screen mode (if that screen
 mode is unavailable `Renderer.SCREEN_MODE_IMMEDIATE` will be used instead)
 + `filter_type` - Use `Renderer.FILTER_TYPE_*` to select your preferred texture filter option

-----------

[Getting Started](GettingStarted.md) | [API Reference](API.md)