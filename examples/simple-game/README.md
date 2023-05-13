# Simple Game
![example](https://github.com/PaoloMazzon/Astro/assets/17896827/686ab0a6-b37d-4407-8e26-1b98d0bd00ab)

![Astro_2023-05-12_23-50-35](https://github.com/PaoloMazzon/PaoloMazzon/assets/17896827/a58bd927-7c6f-48b6-851c-2909d4d690dc)

This demo features a small room where the player can jump around and climb ladders.
The game is rendered to a surface for proper pixel scaling at the low resolution, 
and as such that surface is rendered to the center of the screen no matter the window
size so the user may toggle fullscreen. There are 3 different entities: the player,
a hitbox that detects when the player falls into the water, and an entity that just
animates water for a specified distance. This game is great for demonstrating how to
use Tiled in Astro.