import "Player" for Player
import "Entities" for ResetPlayer, Water

class Game is Level {
    // Random getters
    tileset { _collision_tileset }
    ladder_tileset { _ladder_tileset }
    camera { _camera }
    ui_camera { _ui_camera }
    game_surface { _game_surface }
    game_width { 16 * 8 }
    game_height { 15 * 8 }
    player { _player_entity }

    construct new() { }

    create() {
        super.create()
        
        // Load the level from tiled
        var tilesets = load("assets/level0.tmj")
        _collision_tileset = tilesets["collisions"] // for player collisions
        _ladder_tileset = tilesets["ladder"] // so the player can climb ladders
        _tilset_surfaces = [ // for drawing the whole level more efficiently
            tilesets["background"].draw_to_surface(),
            tilesets["collisions"].draw_to_surface(),
            tilesets["ladder"].draw_to_surface(),
        ]

        // Create the game surface for proper pixel scaling
        _game_surface = Surface.new(game_width, game_height)

        // Locate the player in the level
        _player_entity = get_entity(Player)

        // Create the game cameras, one for the ui and another to follow
        // the player around
        _camera = Camera.new()
        _ui_camera = Camera.new()
        for (cam in [_camera, _ui_camera]) {
            cam.width = game_width
            cam.height = game_height
            cam.update()
        }
        Renderer.use_cameras_on_surfaces = true

        // Center the game camera on the player initially
        camera.x = player.x - (game_width / 2)
        camera.y = player.y - (game_height / 2)
    }

    pre_frame() {
        super.pre_frame()
    }

    update() {
        // Prepare the game surface
        Renderer.target = game_surface
        Renderer.clear_blank()

        // Draw the tilesets
        Renderer.lock_cameras(camera)
        for (tileset in _tilset_surfaces) {
            Renderer.draw_texture(tileset, 0, 0)
        }

        // Update all entities
        super.update()

        // Draw UI
        Renderer.lock_cameras(ui_camera)
        Renderer.draw_font_ext(Assets.fnt_FutilePro, "[#000000][~2]Alt+Enter", 1, 2)
        Renderer.draw_font_ext(Assets.fnt_FutilePro, "[*][~2]Alt+Enter", 2, 0)

        // Draw the game surface to the middle of the screen
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        Renderer.target = Renderer.RENDER_TARGET_DEFAULT
        var scale = (Renderer.window_width / game_width).floor
        if ((Renderer.window_height / game_height).floor < scale) {
            scale = (Renderer.window_height / game_height).floor
        }
        Renderer.draw_texture(game_surface, (Renderer.window_width - (game_width * scale)) / 2, (Renderer.window_height - (game_height * scale)) / 2, scale, scale, 0, 0, 0)

        // Allow the user to toggle fullscreen with Alt+Enter
        if (Keyboard.key(Keyboard.KEY_LALT) && Keyboard.key_pressed(Keyboard.KEY_RETURN)) {
            Renderer.fullscreen = !Renderer.fullscreen
        }
    }

    destroy() {
        super.destroy()
    }
}