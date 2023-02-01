# Asset Compiler
Astro automatically creates a Wren module when it starts called `Assets` with one class:
`Assets`. In this class you will find all of the resources your game needs from the folder
`assets/` already loaded. The asset compiler will search through said folder and any
files with extensions it recognizes will be loaded at startup. The `Assets` class will
name the getter function the same thing as the files it pulled from without the extension.

For example, if your `assets/` folder looks like

    player.png
    grass.jpg
    music.wav

You would be able to access the files with

    import "Assets" for Assets
    ...
    Renderer.draw_texture(Assets.player, 0, 0)
    Renderer.draw_texture(Assets.grass, 100, 100)
    Audio.play(Assets.music, true, 1, 1)

Based on the extension, the asset compiler will automatically load it as the proper class.
The `Assets` class also provides a way to access assets via a string name of the asset by the
`Assets[]` operator. Simply, `Assets.font` is equal to `Assets["font"]`.

## Loading Sprites and Bitmap Fonts
Because bitmap fonts and sprites both just come from images, you have to tell the asset compiler
if you want it to load an image as a sprite or bitmap font with a file called `assets/assets.ini`.
Any images not named in `assets/assets.ini` (or if the file doesn't exist) will be loaded as a
`lib/Drawin::Texture`.

An example of an `assets/asset.ini` file

    [sprite.png]
    type=sprite
    x=0
    y=0
    w=16
    h=16
    delay=0.2
    frames=4
    
    [font.png]
    type=font
    ustart=32
    uend=128
    w=40
    h=70

For each file you want to load as something other than a texture, you need a corresponding
header in the INI file with the filename as the header, and a few other variables you can
see in the example above.

--------

[Getting Started](GettingStarted.md) | [API Reference](API.md)