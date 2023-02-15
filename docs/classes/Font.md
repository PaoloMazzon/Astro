## Font
You may render text with a call to `lib/Renderer::draw_font`.

 + [open](#open)
 + [size](#size)
 + [free](#free)

### open
`construct open(filename, size, aa, unicode_start, unicode_end)`

Parameters
 + `filename -> String` Filename of the .ttf to use.
 + `size -> Num` Height in pixels the font should be.
 + `aa -> Bool` Whether or not to use an anti-aliasing filter.
 + `unicode_start -> Num` Starting character of the unicode range the image includes (inclusive).
 + `unicode_end -> Num` Ending character of the unicode range the image includes (inclusive).
 
Creates a new font from a .ttf file.

### size
`size(string)`

Parameters
 + `string -> String` String to check the size of.

Returns a list of `[w, h]` representing the width and height of the string if it were to be
rendered.

### free
`free()`

Forces a font to be freed from memory without waiting for the garbage collector.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)