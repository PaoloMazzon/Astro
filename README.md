![Astro Engine](./assets/banner.png)

Astro Engine is a game development framework that provides all the things you need to
easily develop games. In no small part, Astro Engine provides bindings for the libraries
[Vulkan2D](https://github.com/PaoloMazzon/Vulkan2D) and [JamUtil](https://github.com/PaoloMazzon/JamUtil)
to a [Wren](https://github.com/wren-lang/wren) virtual machine, with various additional features
provided in the bindings. It will also provide a slew of other features to make development
faster and easier.

Getting Started
===============
See [Getting Started](docs/GettingStarted.md) for a detailed breakdown, and check out the
[API Reference](docs/API.md) for everything else. In short, you need a Wren file the engine
will use as an entry point and at least one level.

A basic game
------------

    import "lib/Engine" for Engine, Level
    import "lib/Input" for Input
    import "lib/Renderer" for Renderer
    import "lib/Drawing" for BitmapFont, Sprite
    import "Assets" for Assets
    
    class Game is Level {
        construct new() { }
        
        create() {
            _x = 100
            _y = 100
        }
    
        update() {
            var speed = Engine.delta() * 100
            if (Input.keyboard_get_key(Input.KEY_A)) {
                _x = _x - speed
            }
            if (Input.keyboard_get_key(Input.KEY_D)) {
                _x = _x + speed
            }
            if (Input.keyboard_get_key(Input.KEY_W)) {
                _y = _y - speed
            }
            if (Input.keyboard_get_key(Input.KEY_S)) {
                _y = _y + speed
            }
    
            Renderer.draw_sprite(Assets.sprite, _x, _y)
            Renderer.draw_font(Assets.font, "The quick brown fox jumps over the lazy dog.", 0, 0)
        }
    
        destroy() { }
    }

See [Getting Started](docs/GettingStarted.md) for more information.

Features
========
Astro is still quite bare-bones at the moment while everything is still being ported over.
Technically, you can use it to make games right now but the biggest issue is the lack of an
audio subsystem as well as lack of any sort of file I/O. See the TODO section for an overview
on what is yet to come.

 + Intuitive API that utilizes [Wren](https://github.com/wren-lang/wren) as its language
 + Manual and automatic resource management
 + Fast and flexible cameras
 + Simple level management
 + Easily load and use sprites, sounds, and fonts

TODO
====

Meta/Framework Things
---------------------

 + Debug mode
 + Asset load directories
 + OGG loading support
 + Profiling tools
 + Entity management system
 + Detailed documentation
 + TrueType fonts
 + Built-in mod support