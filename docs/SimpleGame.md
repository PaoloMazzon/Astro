---
layout: default
title: A Simple Game
nav_order: 8
has_children: false
---

# Simple Game Example Explained
You can find the simple-game example on the 
[Github](https://github.com/PaoloMazzon/Astro/tree/master/examples/simple-game).
Here is a brief explanation of how the small game works.

![example](https://user-images.githubusercontent.com/17896827/237554076-686ab0a6-b37d-4407-8e26-1b98d0bd00ab.gif)

 + [init.wren](#initwren)
 + [Game.wren](#Gamewren)
 + [Player Entity](#player-entity)
 + [Supporting Entities](#supporting-entities)
 + [Tiled Map](#tiled-map)

## init.wren

```javascript
import "Game" for Game

var renderer_config = {
	"window_title": "Game",
	"window_width": 16 * 8 * 6,
	"window_height": 15 * 8 * 6,
	"fullscreen": false,
	"msaa": Renderer.MSAA_1X,
	"screen_mode": Renderer.SCREEN_MODE_TRIPLE_BUFFER,
	"filter_type": Renderer.FILTER_TYPE_NEAREST
}

var window_icon = "assets/logo.png"

System.print(Engine.info)
Engine.fps_limit = 60

var start_level = Game.new()
```

This file is small enough that we can just look at it in whole. There are 3 notable
things we do in this file. First we set the screen width/height `16 * 8 * 6 x 15 * 8 * 6`,
which seems like an odd number, but we'll get there in a second. Second, we set the
window icon to logo found in `assets/logo.png`, and third we create the level instance
the game takes place in for the engine.

The game consists of really small 8x8 tiles like the NES, and our screen resolution
reflects this. We are basically saying we want to display `16x15` tiles on screen
up-scaled by a factor of 6 (we do that calculation in `Game.wren`).

The game running at 6x scale

![astro](astrobig.png)

The game running at 2x scale

![astro](astrosmall.png)

## Game.wren
The level `Game` is responsible for a few things:

 + Loading the Tiled map and tileset surfaces
 + Creating the cameras for the game
 + Updating the entities
 + Drawing the UI/up-scaling the game

{: .note }
The level also provides a bunch of getter methods for
random pieces of data other entities may need, like the camera or
tilesets. Those won't be covered here.

For the first bit, loading the tiled map, the code in question is

```javascript
var tilesets = load("assets/level0.tmj")
_collision_tileset = tilesets["collisions"] // for player collisions
_ladder_tileset = tilesets["ladder"] // so the player can climb ladders
tilset_surfaces = [ // for drawing the whole level more efficiently
    tilesets["background"].draw_to_surface(),
    tilesets["collisions"].draw_to_surface(),
    tilesets["ladder"].draw_to_surface(),
]
```

We store the ladder and collision tileset so the player can use them later, and
then we create a list of each tileset's surface. the [draw_to_surface()](classes/Tileset#draw_to_surface)
method will create a new surface (basically a texture here) with that tileset drawn
to it. This means instead of drawing each tile from each tileset each from, we need
only to draw the surface we drew the whole tileset to. Since we also put it in a list
we can just loop that list, in fact that is what we do later:

```javascript
for (tileset in _tilset_surfaces) {
    Renderer.draw_texture(tileset, 0, 0)
}
```

Using only those two pieces of code we loaded every entity and tileset we need, from the
map and also drew them all efficiently.

{: .note }
You don't need to store the tilesets in a surface, but it is just much more
efficient to do so. This example is so small that it wouldn't cause problems, but
in a map with hundreds or thousands of tiles it would completely hinder performace.

Cameras are less interesting, in this example we create only 2: one for the game world
that the player will control by moving it towards himself, and another that doesn't
move for the UI.

```javascript
_camera = Camera.new()
_ui_camera = Camera.new()
for (cam in [_camera, _ui_camera]) {
    cam.width = game_width
    cam.height = game_height
    cam.update()
}
Renderer.use_cameras_on_surfaces = true

camera.x = player.x - (game_width / 2)
camera.y = player.y - (game_height / 2)
```

Updating the entities is done with the single line `super.update()` and up-scaling
the game isn't terribly important to the game's functionality so it won't be covered
here.

## Player Entity
TODO: This

## Supporting Entities
TODO: This

## Tiled Map
TODO: This
