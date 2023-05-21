---
layout: default
title: Getting Started
nav_order: 2
has_children: false
---


# Getting Started
This documentation mainly covers Astro, for a guide on how to use Astro's language of
choice, [Wren](https://github.com/wren-lang/wren), visit the [Wren website](https://wren.io/getting-started.html)
and the [Wren section](Wren).
To launch an Astro project, you need the runtime (Astro.exe) and at least one Wren file named
`data/game/init.wren` that tells the engine where and how to start execution.

In brief, you want

 + All of your resources inside `data/`.
 + All of your .wren source files in `data/game`.
 + A file named `data/game/init.wren` that tells Astro how to start.

## Runtime
You may either build the C runtime yourself or download a binary if their available. The runtime
is responsible for the Wren virtual machine, asset compiling, and error bindings to the engine
functionality. 

[Downloads](https://github.com/PaoloMazzon/Astro/releases)

## Astro Standard Library
The Astro standard library is automatically imported in every file Astro loads. This means
that anywhere in your game you don't need to import anything Astro defines, simply use the method.
For example, you don't need to import the keyboard, just use it via something like
`Keyboard.key(Keyboard.KEY_LEFT)`. Check the [API Reference](classes/index) for a complete list 
of all classes and methods in said classes.

## `data/game/init.wren`
The `init.wren` file is the "entry point" of your game. It must contain the two variables `renderer_config`
and `start_level`. `start_level` is an instance of `Level` that the engine will use as the
entry point. `renderer_config` must be a map containing specific keys. 

```javascript
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

var window_icon = "data/logo.png"
var minimum_load_time = 3

var start_level = Game.new()
```

There are two other (optional) variables you may set in this file: `window_icon` and `minimum_load_time`.
`window_icon` should specify an image file which Astro will set as the window icon, and 
`minimum_load_time` will be the minimum amount of time the loading screen (see: [Loading Screen](#loading-screen))
will be displayed.

As you can see, the `Renderer` class provides values to use for msaa, screen_mode, and filter_type.
Check [config](classes/Renderer.md#config) for details on each of the map's keys and expected
values, but the above example has all required map keys. The level instance you pass will eventually
have its [create](classes/Level#create) method called once the engine has fully initialized.

{: .note }
When the `init.wren` file is loaded into Astro most of the engine is not yet initialized - in particular
anything that handles user input, the window and renderer, and all the user assets. You may use things like
[INI](classes/INI) or [File](classes/File) to read saved settings, but not much past that.

## Loading Screen
Astro will display a small (800x450) image while the game compiles and loads the asset file/initializes
the renderer. By default this will show an Astro Engine banner, but you may specify your own loading
screen image by placing an image at `data/_loading.png`. By default that image is displayed for at least
3 seconds (adjustable in the `init.wren` file as seen above) so this is a good place to put promo materials.
