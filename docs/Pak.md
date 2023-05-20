---
layout: default
title: Game Paks
nav_order: 6
has_children: false
---

## game.pak

You may choose to compile all of your game assets and code into a single `game.pak`
file. `Packer.exe` is automatically built with the top-level CMake project and when
ran will put each file from `data/` and `data/game/` into `game.pak`.

To build your game.pak file, copy Packer to your projects directory (or call it from
your project's root directory) and exectute

    $ ./Packer

## Usage

When Astro is launched it checks for `game.pak` and if it finds one it will load
most assets from there with the exception of some various tools like [File](classes/File)
and [Buffer](classes/Buffer) which need the user to specify if they wish to load
from `game.pak`. [using_pak](classes/Engine#using_pak) can tell your program if
a `game.pak` is currently in use at runtime.

For most intents and purposes you may just work on your projects as normal and package
your game up with `Packer.exe` at the end.

### Notes

`game.pak` is not encrypted or compressed in any way, and the filetype is documented
in Astro's source, it is merely a way to not expose your entire game to most users.
`.pak` files are read-only, which is why you may load files/buffers from it but you can't
save them to it after.

