![Astro Engine](./assets/banner.png)

Astro Engine is a game development framework that provides all the things you need to
easily develop games. In no small part, Astro Engine provides bindings for the libraries
[Vulkan2D](https://github.com/PaoloMazzon/Vulkan2D) and [JamUtil](https://github.com/PaoloMazzon/JamUtil)
to a [Wren](https://github.com/wren-lang/wren) virtual machine, with various additional features
provided in the bindings. It will also provide a slew of other features to make development
faster and easier.

Getting Started
===============
To use the Astro Engine runtime you must first compile it from source. That won't be covered
here in detail at the moment, but in brief, clone this repository and all the submodules
and run the CMake file.

Here are the specifications for using the runtime

 + The runtime entry point is `prog/init.wren`, from there where you put your source files
 is irrelevant to the runtime as long as you import them all properly
 + Wren bindings must be located in `prog/lib/`
 + The runtime treats `prog/` as the import working directory
 + `prog/init.wren` must contain at least two top-level variables `start_level` and `renderer_config`
 that will be detailed below

`prog/init.wren` Variables
--------------------------
`start_level` must be a new instance of a class that implements all methods of `Level`. The runtime
will use that class to start the game from.

`renderer_config` must be a map of details on how to initialize the renderer. All of the following
keys must be provided:

 + `window_title` - A string which will be the window's title
 + `window_width` - Window width in pixels
 + `window_height` - Window height in pixels
 + `msaa` - Use `Renderer.MSAA_*` to select your preferred MSAA (if the selected MSAA isn't available
 the next highest available MSAA will be used instead)
 + `screen_mode` - Use `Renderer.SCREEN_MODE_*` to select your preferred screen mode (if that screen
 mode is unavailable `Renderer.SCREEN_MODE_IMMEDIATE` will be used instead)
 + `filter_type` - Use `Renderer.FILTER_TYPE_*` to select your preferred texture filter option

 
TODO
====

Meta/Framework Things
---------------------

 + Debug mode that checks types on binded functions
 + Basic file I/O functions that can be binded to Wren since Wren doesn't provide any
 + Better audio system that can be binded to the VM

Bindings
--------

 + All of `JamUtil`
 + Foreign class for VK2D objects `VK2DModel`, `VK2DShader`, `VK2DPolygon`
 + Bind functions to the above foreign classes