# API Reference by Class

### `Engine.wren`
 + [Engine](#engine)
 + [Level](#level)

### `File.wren`
 + [File](#file)
 + [INI](#ini)
 
### `Drawing.wren`
 + [BitmapFont](#bitmapfont)
 + [Texture](#texture)
 + [Sprite](#sprite)
 + [Surface](#surface)

### `Input.wren`
 + [Input](#input)

### `Renderer.wren`
 + [Renderer](#renderer)
 + [Camera](#camera)

### `Audio.wren`
 + [AudioData](#audiodata)
 + [Audio](#audio)

### `Util.wren`
 + [Math](#math)
 + [Hitbox](#hitbox)


## Engine
The Engine class represents top-level static methods that assist in many
aspects of game development.

 + [switch_level](#switch_level)
 + [quit](#quit)
 + [cap_fps](#cap_fps)
 + [delta](#delta)
 + [time](#time)
 + [fps](#fps)
 
### switch_level
`static switch_level(level)`

Parameters
 + `level -> lib/Engine::Level` level to switch to

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

Parameters
 + `fps -> Num` target framerate

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

### fps
`static fps()`

Returns the average framerate over the last second.

## Level
This class is only provided for the user to inherit their level classes from. See [switch_level](#switch_level).

## File
`File` provides some basic tools for manipulating text files.

+ [read](#read)
+ [write](#write)
+ [exists](#exists)

### read
`static read(filename)`

Parameters
 + `filename -> String` filename to read from.
 
Reads an entire file into a string and returns it. The file is not kept open.

### write
`static write(filename, contents)`

Parameters
 + `filename -> String` filename to write to.
 + `contents -> String` contents to write to the file.

Opens a file and writes `contents` to it, overwriting the file if it already exists.

### exists
`static exists(filename)`

Parameters
 + `filename -> String` filename to check.
 
Returns true if the given `filename` exists and false otherwise.

## INI 
You can easily access and save data to INI configuration files with this class.
If you just need some simple data saved to a file this class will often be easier
to use than manually manipulating files. 

INI files are human-readable file format that organizes data into key/header pairs.
Files look something like the following:

    [header]
    key=value
    other_value=5.6
    
    [my data]
    player=False

+ [open](#open)
+ [flush](#flush)
+ [key_exists](#key_exists)
+ [get_string](#get_string)
+ [set_string](#set_string)
+ [get_num](#get_num)
+ [set_num](#set_num)
+ [get_bool](#get_bool)
+ [set_bool](#set_bool)

### open
`construct open(filename) {}`

Parameters
 + `filename -> String` file to read config from.
 
Creates a new INI class from an existing INI file, if the file does not exist it
will return an empty INI class.

### flush
`flush(filename)`

Parameters
 + `filename -> String` file to export the INI to.

Saves the contents of the class to a file in the form of a human-readable INI file.

### key_exists
`key_exists(header, key)`

Parameters
 + `header -> String` header to check from.
 + `key -> String` key to check for.

Returns true if the selected `key`/`header` pair exists in the INI, false otherwise.

### get_string
`get_string(header, key, def)`

Parameters
 + `header -> String` header to get the value in.
 + `key -> String` key to get the value for.
 + `def -> String` value that will be returned if they key doesn't exist.
 
Returns a value from the INI if it exists, otherwise it returns `def`.

### set_string
`set_string(header, key, val)`

Parameters
 + `header -> String` header to set value in.
 + `key -> String` key to set the value for.
 + `val -> String` value to set.
 
Sets a value in the INI.

### get_num
`get_num(header, key, def)`

Parameters
 + `header -> String` header to get the value in.
 + `key -> String` key to get the value for.
 + `def -> Num` value that will be returned if they key doesn't exist.
 
Returns a value from the INI if it exists, otherwise it returns `def`.

### set_num
`set_num(header, key, val)`

Parameters
 + `header -> String` header to set value in.
 + `key -> String` key to set the value for.
 + `val -> Num` value to set.
 
Sets a value in the INI.

### get_bool
`get_bool(header, key, def)`

Parameters
 + `header -> String` header to get the value in.
 + `key -> String` key to get the value for.
 + `def -> Bool` value that will be returned if they key doesn't exist.
 
Returns a value from the INI if it exists, otherwise it returns `def`.

### set_bool
`set_bool(header, key, val)`

Parameters
 + `header -> String` header to set value in.
 + `key -> String` key to set the value for.
 + `val -> Bool` value to set.
 
Sets a value in the INI.

## BitmapFont


## Texture


## Sprite


## Surface


## Input


## Renderer
The `Renderer` class contains methods that are used in drawing and managing the display.

> 📝 The internal renderer for Astro is very picky and has rules on when you are allowed
> to call methods that draw to the screen. You are allowed to draw to the screen ***only***
> during the `update()` method of levels (and inside methods called from `update()`).


## Camera


## AudioData


## Audio


## Math


## Hitbox

