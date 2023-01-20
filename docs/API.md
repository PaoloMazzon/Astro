# API Reference by Class

### `Engine.wren`
 1. [Engine](#engine)
 2. [Level](#level)

### `File.wren`
 1. [File](#file)
 2. [INI](#ini)


## Engine
The Engine class represents top-level static functions that assist in many
aspects of game development.

 + [switch_level](#switch_level)
 + [quit](#quit)
 + [cap_fps](#cap_fps)
 + [delta](#delta)
 + [time](#time)

### switch_level
`static switch_level(level)`

Switches the current level to `level`. `level` must be an instance of a class 
that inherits from `lib/Engine::Level` and implements all its methods.

> 📝 After this method is called the current frame will finish execution, then
> the current level's destruction method will be called before the new level's
> creation method.

### quit
`static quit()`

Forces the game to exit. The game will finish executing the current frame, then
the level's destruction method will be called before all resources are cleaned up
and program closes. This method is also implicitly called when the user closes
the game window.

### cap_fps
`static cap_fps(fps)`

Forces the engine to process no more than `fps` frames a second. If `fps` is `0`,
there will be no limit on framerate.

> 📝 If you set the screen mode to `SCREEN_MODE_VSYNC`, the framerate will be
> capped at the monitor's refresh rate (typically 60) regardless of what you set
> with this function.

### delta
`static delta()`

Returns the time in seconds the last frame took to process. This is typically useful
when making game logic not bound to framerate.

### time
`static time()`

Returns the time seconds since the engine initialized.


## Level


## File


## INI 