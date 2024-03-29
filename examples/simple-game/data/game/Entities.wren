// Helper entities not deserving of their own file

// This entity will move the player back to his spawn when
// the player touches it. This entity's hitbox is automatically
// created when the Tiled map is loaded.
class ResetPlayer is Entity {
    construct new() { super() }

    update(level) {
        if (colliding(level.player)) {
            level.player.x = 160
            level.player.y = 152
        }
    }
}

// This entity animates water from the tilesheet
class Water is Entity {
    construct new() { super() }

    create(level, tiled_data) {
        super.create(level, tiled_data)
        _tiles_across = tiled_data["width"] / 8
        _timer = 0
        _frame = 0
    }

    draw(level) {
        // tl;dr move one frame forward every 0.5 seconds
        _timer = _timer + Engine.delta
        if (_timer >= 0.5) {
            _timer = 0
            _frame = (_frame + 1) % 3
        }

        // Render tiles for each tile across, this is kinda
        // fancy but not hugely important.
        for (i in 0..(_tiles_across - 1)) {
            Renderer.draw_texture_part(
                Assets.tex_cavesofgallet_tiles,
                x + (i * 8),
                y,
                ((i + _frame) % 3) * 8,
                80,
                8,
                8
            )
        }
    }
}