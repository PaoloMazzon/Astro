---
layout: default
title: Getting Started
nav_order: 2
has_children: false
---


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
The Astro standard library is automatically imported in every file Astro loads. Check the
[API Reference](./API.md) for a complete list of all classes and methods in said classes.

## `prog/init.wren`
You `init.wren` file is the "entry point" of your game. It must contain the two variables `renderer_config`
and `start_level`. `start_level` is an instance of `Level` that the engine will use as the
starting level. `renderer_config` must be a map containing specific keys. Additionally, you may provide
a third variable, `window_icon`, which will be a path to an image Astro will set as the window's icon.

```python
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
```

As you can see, the `Renderer` class provides values to use for msaa, screen_mode, and filter_type.
Check [set_config](classes/Renderer.md#set_config) for details on each of the map's keys and expected
values.

{: .note }
When the `init.wren` file is loaded into Astro, the rest of the engine is yet to initialize.
In practice this means you may **not** call most engine-related methods.  
  
After Astro loads this file and extracts the data it wants from it, it will then
initialize the rest of the engine and call the level's `create()` method. From
there your game starts. See the [API Reference](API.md) for help and check the
[Asset Compiler](AssetCompiler.md) for info on how to automate asset management.
Feel free to play around with the demo game.

## Packaging Your Game

Once you wish to ship your game, you may either just ship the code and assets as they
are, or alternatively you may use `Packer.exe` to package your game into a single file
and executable. See [Pak](Pak.md) for more details.

