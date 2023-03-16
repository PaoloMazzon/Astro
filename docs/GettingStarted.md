# Getting Started
This documentation only covers Astro, for a guide on how to use Astro's language of
choice, [Wren](https://github.com/wren-lang/wren), visit the [Wren website](https://wren.io/getting-started.html).

To launch a game in Astro you need 3 things:
 1. The runtime (`Astro.exe` and related files)
 2. Astro standard library
 3. A `prog/init.wren` file
 
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
 starting level. `renderer_config` must be a map containing specific keys. Additionally, you may provide
 a third variable, `window_icon`, which will be a path to an image Astro will set as the window's icon.

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
    
    var window_icon = "assets/logo.png"
    
    var start_level = Game.new()

As you can see, the `Renderer` class provides values to use for msaa, screen_mode, and filter_type.
Check [set_config](classes/Renderer.md#set_config) for details on each of the map's keys and expected
values.

> 📝 When the `init.wren` file is loaded into Astro, the rest of the engine is yet to initialize.
> In practice this means you may **not** call most engine-related methods.  
  
After Astro loads this file and extracts the data it wants from it, it will then
initialize the rest of the engine and call the level's `create()` method. From
there your game starts. See the [API Reference](API.md) for help and check the
[Asset Compiler](AssetCompiler.md) for info on how to automate asset management.
Feel free to play around with the demo game.

-----------

[Getting Started](GettingStarted.md) | [API Reference](API.md)