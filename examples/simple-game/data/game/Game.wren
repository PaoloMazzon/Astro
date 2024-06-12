import "Player" for Player
import "random" for Random

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
        _rng = Random.new()
        
        // Load the level from tiled
        Renderer.use_cameras_on_surfaces = false
        var tilesets = load("data/level0.tmj")
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

        // Setup shadows
        Renderer.setup_lighting(game_width, game_height, game_width, game_height)
        _lights = []
        _lights.add(Lighting.add_light(17 * 8 + 4, 18 * 8 + 4, 0, Assets.tex_light.width / 2, Assets.tex_light.height / 2, Assets.tex_light))
        _lights.add(Lighting.add_light(3 * 8 + 4, 19 * 8 + 4, 0, Assets.tex_light.width / 2, Assets.tex_light.height / 2, Assets.tex_light))
        _lights.add(Lighting.add_light(15 * 8 + 4, 5 * 8 + 4, 0, Assets.tex_light.width / 2, Assets.tex_light.height / 2, Assets.tex_light))
        for (light in _lights) {
            light.colour = [1, 1, 1, 0.7]
        }
        Lighting.add_shadow([
            [20 * 8, 13 * 8, 20 * 8, 15 * 8], 
            [20 * 8, 15 * 8, 30 * 8, 15 * 8], 
            [20 * 8, 13 * 8, 30 * 8, 13 * 8],
            [16 * 8, 21 * 8, 25 * 8, 21 * 8],
            [0 * 8, 22 * 8, 6 * 8, 22 * 8],
            [3 * 8, 8 * 8, 17 * 8, 8 * 8]
        ])
        
        // Player shadow
        _player_shadow = Lighting.add_shadow([[2, 2, 7, 2], [7, 2, 7, 7], [7, 7, 2, 7], [2, 7, 2, 2]])
        Lighting.flush_vbo()
        _flicker_timer = 0
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

        // Draw shadows
        _flicker_timer = _flicker_timer + 1
        if (_flicker_timer >= 8) {
            for (light in _lights) {
                var scale = _rng.float(0.8, 1.2)
                light.scale_x = scale
                light.scale_y = scale
                light.origin_x = (Assets.tex_light.width * scale) / 2
                light.origin_y = (Assets.tex_light.height * scale) / 2
            }
            _flicker_timer = 0
        }
        _player_shadow.position = [_player_entity.sprite.scale_x == 1 ? _player_entity.x : _player_entity.x - 1, _player_entity.y]
        Renderer.draw_lighting(game_surface, camera, ui_camera)

        // Update all entities
        Renderer.lock_cameras(camera)
        super.update()

        // Draw UI
        Renderer.lock_cameras(ui_camera)
        Renderer.draw_font_ext(Assets.fnt_font, "[#000000][~2]Alt+Enter", 1, 2)
        Renderer.draw_font_ext(Assets.fnt_font, "[*][~2]Alt+Enter", 2, 0)

        // Calculate the game surface's scale
        Renderer.lock_cameras(Renderer.DEFAULT_CAMERA)
        Renderer.target = Renderer.RENDER_TARGET_DEFAULT
        var scale = (Renderer.window_width / game_width).floor
        if ((Renderer.window_height / game_height).floor < scale) {
            scale = (Renderer.window_height / game_height).floor
        }

        // Draw a background, outline around the game, and the game surface
        Renderer.colour_mod = [0.05, 0.05, 0.05, 1]
        Renderer.draw_texture(Assets.tex_background, 0, 0, Renderer.window_width / Assets.tex_background.width, Renderer.window_height / Assets.tex_background.height, 0, 0, 0)
        Renderer.colour_mod = "#000000"
        Renderer.draw_rectangle((Renderer.window_width - (game_width * scale)) / 2 - 2, (Renderer.window_height - (game_height * scale)) / 2 - 2, (game_width * scale) + 4, (game_height * scale) + 4, 0, 0, 0)
        Renderer.colour_mod = Renderer.COLOUR_DEFAULT
        Renderer.draw_texture(game_surface, (Renderer.window_width - (game_width * scale)) / 2, (Renderer.window_height - (game_height * scale)) / 2, scale, scale, 0, 0, 0)
        

        // Allow the user to toggle fullscreen with Alt+Enter
        if (Keyboard.key(Keyboard.KEY_LALT) && Keyboard.key_pressed(Keyboard.KEY_RETURN)) {
            Renderer.fullscreen = !Renderer.fullscreen
        }
    }

    destroy() {
        super.destroy()
        Lighting.reset()
    }
}