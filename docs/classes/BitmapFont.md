## BitmapFont
Bitmap fonts are a way to render images as text with complete control. With bitmap fonts,
you load an image as a character set in a specified range - often 33 to 127 or `$` - `~`.
With that character set you may render text with a call to `lib/Renderer::draw_font`.

 + [new](#new)
 + [size](#size)
 + [free](#free)

### new
`construct new(filename, unicode_start, unicode_end, w, h)`

Parameters
 + `filename -> String` Filename of the image to use.
 + `unicode_start -> Num` Starting character of the unicode range the image includes (inclusive).
 + `unicode_end -> Num` Ending character of the unicode range the image includes (inclusive).
 + `w -> Num` Width of each character in pixels.
 + `h -> Num` Height of each character in pixels.
 
Creates a new bitmap font from a given image filename. The characters do not need to be in a line,
you may wrap characters to have a more square image.

> 📝 The image provided must be at least w * h * (unicode_end - unicode_start) pixels large or this
> method will fail.

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