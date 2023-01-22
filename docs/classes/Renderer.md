## Renderer
The `Renderer` class contains methods that are used in drawing and managing the display.

> 📝 The internal renderer for Astro is very picky and has rules on when you are allowed
> to call methods that draw to the screen. You are allowed to draw to the screen ***only***
> during the `update()` method of levels (and inside methods called from `update()`).
>
-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)