## game.pak

You may choose to compile all of your game assets and code into a single `game.pak`
file by running the tool `Packer.exe` in your root game directory. `Packer.exe` is
automatically built with the top-level CMake project and when ran will put each
file from `assets/` and `prog/` into `game.pak`.

When Astro is launched it checks for `game.pak` and if it finds one it will load
most assets from there with the exception of some various tools like [File](classes/File.md)
and [Buffer](classes/Buffer.md) which need the user to specify if they wish to load
from `game.pak`.

For most intents and purposes you may just work on your projects as normal and package
your game up with `Packer.exe` at the end.

### Notes

`game.pak` is not encrypted or compressed in any way, and the filetype is documented
in Astro's source, it is merely a way to not expose your entire game to most users.
`.pak` files are read-only, which is why you may load files/buffers from it but you can't
save them to it after.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)