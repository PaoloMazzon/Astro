---
layout: default
title: Engine
parent: API Documentation
---

## Engine
The Engine class represents top-level static methods that assist in many
aspects of game development.

 + [switch_level](#switch_level)
 + [quit](#quit)
 + [fps_limit](#fps_limit)
 + [delta](#delta)
 + [time](#time)
 + [fps](#fps)
 + [info](#info)
 + [get_class](#get_class)
 + [timestep=](#timestep)
 + [timestep_percent](#timestep_percent)
 + [process_frame](#process_frame)
 + [argv](#argv)
 + [using_pak](#using_pak)
 
### switch_level
`static switch_level(level)`

Parameters
 + `level -> Level` level to switch to

Switches the current level to `level`. `level` must be an instance of a class 
that inherits from `Level` and implements all its methods.

{: .note }
After this method is called the current frame will finish execution, then
the current level's destruction method will be called before the new level's
creation method.

### quit
`static quit()`

Forces the game to exit. The game will finish executing the current frame, then
the level's destruction method will be called before all resources are cleaned up
and program closes. This method is also implicitly called when the user closes
the game window.

### fps_limit
Write Only: `static fps_limit=(fps)`

Variable Type: `Num` - target framerate

Forces the engine to process no more than `fps` frames a second. If `fps` is `0`,
there will be no limit on framerate.

{: .note }
If you set the screen mode to `SCREEN_MODE_VSYNC`, the framerate will be
capped at the monitor's refresh rate (typically 60) regardless of what you set
with this function.

### timestep=
Write Only: `timestep=(timestep)`

Variable Type: `Num` - How many times `process_frame` should be set to true each second.
 
For fixed-timestep logic. This controls how many times each second that `process_frame` will be
set to true to let the user handle their own fixed-timestep code. Level's [update](Level#update)
method will only update entities when `process_frame` is true automatically. Set this to 0 (the default)
to disable fixed-timestep.

{: .note }
Do not set this value before the first level is initialized; ie, in `init.wren`.

### timestep_percent
Read Only: `timestep_percent`

Variable Type: `Num`

How far (hypothetically) we are through this current second for fixed timestep interpolation.

### process_frame
Read Only: `process_frame`

Variable Type: `Bool`

Returns true if this frame is meant to be processed for fixed timestep games, always returns true
if `timestep` is 0.

### delta
Read Only: `static delta`

Variable Type: `Num`

Returns the time in seconds the last frame took to process. This is typically useful
when making game logic not bound to framerate.

### time
Read Only: `static time`

Variable Type: `Num`

Returns the time seconds since the engine initialized.

### fps
Read Only: `static fps`

Variable Type: `Num`

Returns the average framerate over the last second.

### info
Read Only: `static info`

Variable Type: `Map`

Returns a map containing information about the engine, the map will contain the
following keys:
 + `name` - A string containing the name of the engine.
 + `wren-version` - Number representing the version of Wren the engine was built with.
 + `astro-version` - A list representing the engine's version in the form `[major, minor, patch]`.
 + `build-date` - Date the engine was built on as a string.

### get_class
`static get_class(class_name)`

Parameters
 + `class_name -> String` Class name in the form `module::class`.
 
This method finds a variable in a module and returns it, or returns `null` if the variable/module
does not exist.

### argv
Read Only: `static argv`

Variable Type: `List` - List of command line parameters given to the program on startup.

### using_pak
Read Only: `static using_pak`

Variable Type: `Bool` - True if Astro is using a `game.pak` file.

