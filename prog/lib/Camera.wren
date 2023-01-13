// Camera.wren
// Author: Paolo Mazzon
// Functions for managing and creating cameras in Astro Engine

// This is equivalent to VK2D's camera spec and the camera index combined
foreign class Camera {
    static CAMERA_TYPE_DEFAULT { 0 }
    static CAMERA_TYPE_PERSPECTIVE { 1 }
    static CAMERA_TYPE_ORTHOGRAPHIC { 2 }

    construct new() {}

    // Gets this camera's type
    foreign get_type()

    // Sets this camera's type (should be one of `Camera.CAMERA_TYPE_*`)
    foreign set_type(type)

    // Gets this camera's x
	foreign get_x()

    // Sets this camera's x
    foreign set_x(x)

    // Gets this camera's y
	foreign get_y()

    // Sets this camera's y
    foreign set_y(y)

    // Gets this camera's w
	foreign get_w()

    // Sets this camera's w
    foreign set_w(w)

    // Gets this camera's h
	foreign get_h()

    // Sets this camera's h
    foreign set_h(h)

    // Gets this camera's zoom
	foreign get_zoom()

    // Sets this camera's zoom
    foreign set_zoom(zoom)

    // Gets this camera's rotation
	foreign get_rotation()

    // Sets this camera's rotation
    foreign set_rotation(rotation)

    // Gets this camera's x_on_screen
	foreign get_x_on_screen()

    // Sets this camera's x_on_screen
    foreign set_x_on_screen(x_on_screen)

    // Gets this camera's y_on_screen
	foreign get_y_on_screen()

    // Sets this camera's y_on_screen
    foreign set_y_on_screen(y_on_screen)

    // Gets this camera's w_on_screen
	foreign get_w_on_screen()

    // Sets this camera's w_on_screen
    foreign set_w_on_screen(w_on_screen)

    // Gets this camera's h_on_screen
	foreign get_h_on_screen()

    // Sets this camera's h_on_screen
    foreign set_h_on_screen(h_on_screen)

    // Gets this camera's eyes
	foreign get_eyes()

    // Sets this camera's eyes -- must be a 3-element list
    foreign set_eyes(eyes)

    // Gets this camera's centre
	foreign get_centre()

    // Sets this camera's centre -- must be a 3-element list
    foreign set_centre(centre)

    // Gets this camera's up
	foreign get_up()

    // Sets this camera's up -- must be a 3-element list
    foreign set_up(up)

    // Gets this camera's fov
	foreign get_fov()

    // Sets this camera's fov
    foreign set_fov(fov)

    // Applies current settings for the camera
    foreign update()
}