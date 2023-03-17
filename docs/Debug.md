## Debug Utilities

Astro has a few different tools you can enable to make debugging a bit easier. These
options are enabled by creating an .ini file named `Astro.ini` with the layout

    [engine]
    enableTypeChecking=false
    enableDebugOverlay=false
    enableAssetsPrint=false
    disableGamePak=false
    gcBetweenLevels=true

(all of the specified values are the default values if no ini is provided)

For a brief explanation of them,

 + `enableTypeChecking` will force the engine to make sure every Wren value passed to
 foreign methods are the right type of value.
 + `enableDebugOverlay` displays a small overlay on top of the game with basic information
 like entity count, framerate, and information on fixed-timestep if thats enabled.
 + `enableAssetsPrint` prints the generated `Assets.wren` file to `stdout`.
 + `disableGamePak` stops the engine from loading things from `game.pak` regardless of
 whether or not it finds one.
 + `gcBetweenLevels` tells the engine to call the garbage collector in between each level
 switch.

-----------

[Getting Started](GettingStarted.md) | [API Reference](API.md)