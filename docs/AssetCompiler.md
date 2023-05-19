---
layout: default
title: Asset Compiler
nav_order: 7
has_children: false
---

# Asset Compiler

 1. [Intro](#intro)
 2. [Recognized File Types](#recognized-file-types)
 3. [Loading Complex Assets](#loading-complex-assets)
 4. [Excluding Files and Directories](#excluding-files-and-directories)

## Intro
Astro automatically creates a Wren module when it starts called `Assets` with one class:
`Assets`. In this class you will find all of the resources your game needs from the folder
`assets/` already loaded. Each loaded asset is accessible via their path relative to `assets/`,
but with their extension removed and a small prefix depending on what they were loaded as.
For example, `assets/player.png` would be accessed via `Assets.tex_player`, and 
`assets/sounds/music.wav` would be `Assets.sounds.aud_music`.

For example, if your `assets/` folder looks like

    | player.png
    | grass.jpg
    | music.wav

You would be able to access the files with

    Renderer.draw_texture(Assets.tex_player, 0, 0)
    Renderer.draw_texture(Assets.tex_grass, 100, 100)
    Audio.play(Assets.aud_music, true, 1, 1)
    
The `Assets` class is automatically imported in every file.

Based on the extension, the asset compiler will automatically load it as the proper class.
The `Assets` class also provides a way to access assets via a string name of the asset by the
`Assets[]` operator. With the `Assets[]` operator you may put in a path to your loaded file
relative to `assets/` once again. So `Assets.tex_player` is the same as `Assets["player.png"]`.

Prefixes

 + Sprites - `spr_*`
 + Textures - `tex_*`
 + Audio - `aud_*`
 + Bitmap fonts - `fnt_*`
 + TrueType fonts - `fnt_*`
 + Buffers - `buf_*`
 + Strings - `txt_*`
 
The asset compiler is recursive for sub-directories and all sub-directories are accessible
from the `Assets` class by replacing any `/`'s in the name with `.`'s instead. For example,
given the following as the structure of the `assets/` folder:

    | dir/
    |    | sprites/
    |    |        | player.png
    |    |        | player_run.png
    |    | jump.ogg
    | banner.jpg

You could access all of them in order through

    Assets.dir.sprites.tex_player
    Assets.dir.sprites.tex_player_run
    Assets.dir.aud_jump
    Assets.tex_banner

## Recognized File Types
In brief,

 + `.png`, `.bmp`, `.jpg`, and `.jpeg` are all recognized as textures.
 + `.wav` and `.ogg` are audio.
 + `.txt` is a string.

To load sprites, bitmap fonts, truetype fonts, buffers, or non-.txt strings automatically
you have to make the asset compiler aware of them through a `assets.json` file. It is outlined
in more detail [here](#loading-complex-assets). Sprites may also be loaded through a corresponding
*texture-file*.json file in the same directory as *texture-file*.

## Loading Complex Assets
Using an `assets.json` file in any folder in the `assets/` folder you may specify sprites/fonts/buffers/strings
inside that folder (sub-directories of `assets/` need their own `assets.json` file too).
TODO: This

## Excluding Files and Directories
TODO: This