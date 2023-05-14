# Asset Compiler
Astro automatically creates a Wren module when it starts called `Assets` with one class:
`Assets`. In this class you will find all of the resources your game needs from the folder
`assets/` already loaded. The asset compiler will search through said folder and any
files with extensions it recognizes will be loaded at startup. The `Assets` class will
name the getter function the same thing as the files it pulled from without the extension and
with a prefix based on the asset type.

For example, if your `assets/` folder looks like

    player.png
    grass.jpg
    music.wav

You would be able to access the files with

    Renderer.draw_texture(Assets.tex_player, 0, 0)
    Renderer.draw_texture(Assets.tex_grass, 100, 100)
    Audio.play(Assets.aud_music, true, 1, 1)
    
The `Assets` class is automatically imported in every file.

Based on the extension, the asset compiler will automatically load it as the proper class.
The `Assets` class also provides a way to access assets via a string name of the asset by the
`Assets[]` operator. Simply, `Assets.fnt_font` is equal to `Assets["fnt_font"]`.

Prefixes

 + Sprites - `spr_*` and additionally a texture of the same name (`tex_*`)
 + Textures - `tex_*`
 + Audio - `aud_*`
 + Bitmap fonts - `fnt_*`
 + TrueType fonts - `fnt_*`

## Loading Sprites and Bitmap Fonts
Because bitmap fonts and sprites both just come from images, you have to tell the asset compiler
if you want it to load an image as a sprite or bitmap font with a file called `assets/assets.ini`.
Any images not named in `assets/assets.ini` (or if the file doesn't exist) will be loaded as a
`Texture`.

An example of an `assets/asset.ini` file

    [sprite.png]
    type=sprite
    x=0
    y=0
    w=16
    h=16
    delay=0.2
    origin_x=0
    origin_y=0
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

## TrueType Fonts
You must also outline `.ttf` files in the `assets.ini` file as well, its quite simple.

    [my_font.ttf]
    size=50
    ustart=32
    uend=128
    aa=true

`size` is the height of the font in pixels, and `ustart` to `uend` is the unicode range to
preload. `aa` is whether or not to apply anti-aliasing to the font.

--------

[Getting Started](GettingStarted) | [API Reference](API)