![Astro Engine](./examples/fixed-timestep/assets/banner.png)

Astro Engine is a game development framework that provides all the things you need to
easily develop games. It uses the scripting language [Wren](https://github.com/wren-lang/wren)
for game logic, taking full advantage of the features that come with an embedded
scripting language.

![Example gif](./examples/fixed-timestep/assets/example.gif)

Features
========

 + Intuitive API that utilizes [Wren](https://github.com/wren-lang/wren) as its language
 + Manual and automatic resource management
 + Fast and very flexible cameras
 + Simple level and entity management
 + Complete [API documentation](docs/API.md)
 + Load [Tiled](https://www.mapeditor.org/) maps automatically

Getting Started
===============
See [Getting Started](docs/GettingStarted.md) for a detailed breakdown, and check out the
[API Reference](docs/API.md) for everything else. In short, you need a Wren file the engine
will use as an entry point and at least one level.

A basic game
------------
    
    class Game is Level {
        construct new() { }
        
        create() {
            _x = 100
            _y = 100
        }
        
        pre_frame() { }
    
        update() {
            var speed = Engine.delta * 100
            if (Keyboard.key(Keyboard.KEY_A)) {
                _x = _x - speed
            }
            if (Keyboard.key(Keyboard.KEY_D)) {
                _x = _x + speed
            }
            if (Keyboard.key(Keyboard.KEY_W)) {
                _y = _y - speed
            }
            if (Keyboard.key(Keyboard.KEY_S)) {
                _y = _y + speed
            }
    
            Renderer.draw_sprite(Assets.spr_sprite, _x, _y)
            Renderer.draw_font(Assets.fnt_font, "The quick brown fox jumps over the lazy dog.", 0, 0)
        }
    
        destroy() { }
    }

See [Getting Started](docs/GettingStarted.md) for more information.

Detailed Feature List
=====================
Before a was brief highlight of features, this is more complete list (but not complete)
of things you can expect Astro to be capable of.

 + Scripting is done in the [Wren](https://github.com/wren-lang/wren) language
 + All Astro library files/classes are automatically imported in every file
 + Detailed error logs are printed to file and `stdout`
 + Automatic resource loading through the [Asset Compiler](./docs/AssetCompiler.md)
 + Optionally store game files in a `game.pak` file so you don't have to distribute multiple files/folders
 + Detailed 2D rendering including [textures](./docs/classes/Texture.md), [sprites](./docs/classes/Sprite.md), [fonts](./docs/classes/Font.md), basic shapes, and [polygon](./docs/classes/Polygon.md) loading
 + [Create 2D textures](./docs/classes/Surface.md) and draw to them
 + Load and use [SPIR-V shaders](./docs/classes/Shader.md) on 2D textures
 + [Simple 3D rendering](./docs/classes/Model.md), can load and render 3D models
 + Powerful [camera support](./docs/classes/Camera.md) for 2D and 3D cameras, supports up to 10 cameras at once
 + Detailed [renderer and window](./docs/classes/Renderer.md) support, allows control over
   - Blend mode
   - Render colour modifier
   - Window size/fullscreen
   - Rendering features like MSAA/filtering
   - Camera control
 + [Detailed text rendering](./docs/classes/Renderer.md#draw_font_ext) allowing the user to change colour/size/shakiness/etc of text in strings
 + Easily control engine-level things like framerate/delta/timestep through the [engine interface](./docs/classes/Engine.md)
 + [Load](./docs/classes/AudioData.md)/[play](./docs/classes/Audio.md) audio
 + Simple and very powerful [keyboard](./docs/classes/Keyboard.md), [mouse](./docs/classes/Mouse.md), and [gamepad](./docs/classes/Gamepad.md) support
 + [Entity](./docs/classes/Entity.md)/[level](./docs/classes/Level.md) management
 + Automatically [load Tiled maps](./docs/TiledIntegration.md) into levels
 + Use [tilesets](./docs/classes/Tileset.md) for fast and simplified tileset rendering and collisions
 + Basic filesystem support through [buffers](./docs/classes/Buffer.md), [file](./docs/classes/File.md), and [inis](./docs/classes/INI.md)
 

TODO
====

 + Networking support
 + Built-in mod support
 + Spatial hash maps
 + More example projects