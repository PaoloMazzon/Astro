Vulkan2D/JamUtil Environment for Wren
=====================================
This project provides a runtime for Wren to use [Vulkan2D](https://github.com/PaoloMazzon/Vulkan2D) 
and [JamUtil](https://github.com/PaoloMazzon/JamUtil) from the scripting language 
[Wren](https://github.com/wren-lang/wren).

Usage
=====

 + Your Wren program must have a module called `main`, a class called `Main` inside it,
 and a static function called `main` inside the class. The runtime will use this as
 the entry point for your program.
 + The SDL side of Vulkan2D is handled by the runtime, all you need is `lib/Renderer` on the
 Wren side of the program
 + Use `Renderer.init()` to create the renderer and `Renderer.update()` every frame to update
 the renderer and window. Freeing resources will be handled by the runtime.