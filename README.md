![Astro Engine](https://astroengine.ca/banner.png)

Astro Engine is a game development framework that provides all the things you need to
easily develop games. It uses the scripting language [Wren](https://github.com/wren-lang/wren)
for game logic, taking full advantage of the features that come with an embedded
scripting language.

![Example gif](examples/fixed-timestep/data/example.gif)

![example](https://github.com/PaoloMazzon/Astro/assets/17896827/686ab0a6-b37d-4407-8e26-1b98d0bd00ab)

Features
========

 + Intuitive API that utilizes [Wren](https://github.com/wren-lang/wren) as its language
 + Manual and automatic resource management
 + Fast and very flexible cameras
 + Simple level and entity management
 + Complete [API documentation](https://astroengine.ca/classes/)
 + Load [Tiled](https://www.mapeditor.org/) maps automatically

Getting Started
===============
See [Getting Started](https://astroengine.ca/GettingStarted.html) for a detailed breakdown, and check out the
[API Reference](https://astroengine.ca/classes/) for everything else. In short, you need a Wren file the engine
will use as an entry point and at least one level.
    
    class Game is Level {
        construct new() { }
        
        create() {
            _x = 100
            _y = 100
        }
        
        pre_frame() { }
    
        update() {
            var speed = Engine.delta * 100
            _x = _x + Keyboard.keys_as_axis(Keyboard.KEY_A, Keyboard.KEY_D) * speed
            _y = _y + Keyboard.keys_as_axis(Keyboard.KEY_W, Keyboard.KEY_S) * speed
    
            Renderer.draw_sprite(Assets.spr_sprite, _x, _y)
            Renderer.draw_font(Assets.fnt_font, "The quick brown fox jumps over the lazy dog.", 0, 0)
        }
    
        destroy() { }
    }

Detailed Feature List
=====================
Before a was brief highlight of features, this is more complete list (but not complete)
of things you can expect Astro to be capable of.

 + Scripting is done in the [Wren](https://github.com/wren-lang/wren) language
 + All Astro library files/classes are automatically imported in every file
 + Detailed error logs are printed to file and `stdout`
 + Automatic resource loading through the [Asset Compiler](https://astroengine.ca/AssetCompiler.html)
 + Automatically load sprite data from Aseprite
 + Optionally store game files in a `game.pak` file so you don't have to distribute multiple files/folders
 + Detailed 2D rendering including [textures](https://astroengine.ca/classes/Texture.html), [sprites](https://astroengine.ca/classes/Sprite.html), [fonts](https://astroengine.ca/classes/Font.html), basic shapes, and [polygon](https://astroengine.ca/classes/Polygon.html) loading
 + [Create 2D textures](https://astroengine.ca/classes/Surface.html) and draw to them
 + Load and use [SPIR-V shaders](https://astroengine.ca/classes/Shader.html) on 2D textures
 + [Simple 3D rendering](https://astroengine.ca/classes/Model.html), can load and render 3D models
 + Powerful [camera support](https://astroengine.ca/classes/Camera.html) for 2D and 3D cameras, supports up to 10 cameras at once
 + Detailed [renderer and window](https://astroengine.ca/classes/Renderer.html) support, allows control over
   - Blend mode
   - Render colour modifier
   - Window size/fullscreen
   - Rendering features like MSAA/filtering
   - Camera control
 + [Detailed text rendering](https://astroengine.ca/classes/Renderer.md#draw_font_ext) allowing the user to change colour/size/shakiness/etc of text in strings
 + Easily control engine-level things like framerate/delta/timestep through the [engine interface](https://astroengine.ca/classes/Engine.html)
 + [Load](https://astroengine.ca/classes/AudioData.html)/[play](https://astroengine.ca/classes/Audio.html) audio
 + Simple and very powerful [keyboard](https://astroengine.ca/classes/Keyboard.html), [mouse](https://astroengine.ca/classes/Mouse.html), and [gamepad](https://astroengine.ca/classes/Gamepad.html) support
 + [Entity](https://astroengine.ca/classes/Entity.html)/[level](https://astroengine.ca/classes/Level.html) management
 + Automatically [load Tiled maps](https://astroengine.ca/TiledIntegration.html) into levels
 + Use [tilesets](https://astroengine.ca/classes/Tileset.html) for fast and simplified tileset rendering and collisions
 + Basic filesystem support through [buffers](https://astroengine.ca/classes/Buffer.html), [file](https://astroengine.ca/classes/File.html), and [inis](https://astroengine.ca/classes/INI.html)
 
Platforms
=========
Astro is currently tested exclusively on Windows. Previous versions have been
tested successfully on Ubuntu with virtually no issue, but OSX support is untested.
Astro is built on top of Vulkan2D and Wren, Wren should be fine on basically
any platform including game consoles because its bytecode-interpreted and not
JIT, but Vulkan2D is not as permissive. It requires a platform that supports
Vulkan 1.2, meaning on OSX it has to be built with MoltenVK and other platforms
are more complicated. If anybody wishes to contribute on these fronts, please
create a pull request.

TODO
====

 + Better Tiled loader
 + Networking support
 + Spatial hash maps