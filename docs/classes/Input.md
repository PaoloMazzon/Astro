## Input
Easy access for the keyboard and mouse.

 + [keyboard_get_key](#keyboard_get_key)
 + [keyboard_get_key_pressed](#keyboard_get_key_pressed)
 + [keyboard_get_key_released](#keyboard_get_key_released)
 + [get_mouse_position](#get_mouse_position)
 + [get_mouse_left](#get_mouse_left)
 + [get_mouse_left_pressed](#get_mouse_left_pressed)
 + [get_mouse_left_released](#get_mouse_left_released)
 + [get_mouse_right](#get_mouse_right)
 + [get_mouse_right_pressed](#get_mouse_right_pressed)
 + [get_mouse_right_released](#get_mouse_right_released)
 + [get_mouse_middle](#get_mouse_middle)
 + [get_mouse_middle_pressed](#get_mouse_middle_pressed)
 + [get_mouse_middle_released](#get_mouse_middle_released)

The following values are also provided:

 + `KEY_A`
 + `KEY_B`
 + `KEY_C`
 + `KEY_D`
 + `KEY_E`
 + `KEY_F`
 + `KEY_G`
 + `KEY_H`
 + `KEY_I`
 + `KEY_J`
 + `KEY_K`
 + `KEY_L`
 + `KEY_M`
 + `KEY_N`
 + `KEY_O`
 + `KEY_P`
 + `KEY_Q`
 + `KEY_R`
 + `KEY_S`
 + `KEY_T`
 + `KEY_U`
 + `KEY_V`
 + `KEY_W`
 + `KEY_X`
 + `KEY_Y`
 + `KEY_Z`
 + `KEY_1`
 + `KEY_2`
 + `KEY_3`
 + `KEY_4`
 + `KEY_5`
 + `KEY_6`
 + `KEY_7`
 + `KEY_8`
 + `KEY_9`
 + `KEY_0`
 + `KEY_RETURN`
 + `KEY_ESCAPE`
 + `KEY_BACKSPACE`
 + `KEY_TAB`
 + `KEY_SPACE`
 + `KEY_MINUS`
 + `KEY_EQUALS`
 + `KEY_LEFTBRACKET`
 + `KEY_RIGHTBRACKET`
 + `KEY_BACKSLASH`
 + `KEY_NONUSHASH`
 + `KEY_SEMICOLON`
 + `KEY_APOSTROPHE`
 + `KEY_GRAVE`
 + `KEY_COMMA`
 + `KEY_PERIOD`
 + `KEY_SLASH`
 + `KEY_CAPSLOCK`
 + `KEY_F1`
 + `KEY_F2`
 + `KEY_F3`
 + `KEY_F4`
 + `KEY_F5`
 + `KEY_F6`
 + `KEY_F7`
 + `KEY_F8`
 + `KEY_F9`
 + `KEY_F10`
 + `KEY_F11`
 + `KEY_F12`
 + `KEY_PRINTSCREEN`
 + `KEY_SCROLLLOCK`
 + `KEY_PAUSE`
 + `KEY_INSERT`
 + `KEY_HOME`
 + `KEY_PAGEUP`
 + `KEY_DELETE`
 + `KEY_END`
 + `KEY_PAGEDOWN`
 + `KEY_RIGHT`
 + `KEY_LEFT`
 + `KEY_DOWN`
 + `KEY_UP`
 + `KEY_NUMLOCKCLEAR`
 + `KEY_KP_DIVIDE`
 + `KEY_KP_MULTIPLY`
 + `KEY_KP_MINUS`
 + `KEY_KP_PLUS`
 + `KEY_KP_ENTER`
 + `KEY_KP_1`
 + `KEY_KP_2`
 + `KEY_KP_3`
 + `KEY_KP_4`
 + `KEY_KP_5`
 + `KEY_KP_6`
 + `KEY_KP_7`
 + `KEY_KP_8`
 + `KEY_KP_9`
 + `KEY_KP_0`
 + `KEY_KP_PERIOD`
 + `KEY_NONUSBACKSLASH`
 + `KEY_APPLICATION`
 + `KEY_POWER`
 + `KEY_KP_EQUALS`
 + `KEY_F13`
 + `KEY_F14`
 + `KEY_F15`
 + `KEY_F16`
 + `KEY_F17`
 + `KEY_F18`
 + `KEY_F19`
 + `KEY_F20`
 + `KEY_F21`
 + `KEY_F22`
 + `KEY_F23`
 + `KEY_F24`
 + `KEY_EXECUTE`
 + `KEY_HELP`
 + `KEY_MENU`
 + `KEY_SELECT`
 + `KEY_STOP`
 + `KEY_AGAIN`
 + `KEY_UNDO`
 + `KEY_CUT`
 + `KEY_COPY`
 + `KEY_PASTE`
 + `KEY_FIND`
 + `KEY_MUTE`
 + `KEY_VOLUMEUP`
 + `KEY_VOLUMEDOWN`
 + `KEY_KP_COMMA`
 + `KEY_KP_EQUALSAS400`
 + `KEY_INTERNATIONAL1`
 + `KEY_INTERNATIONAL2`
 + `KEY_INTERNATIONAL3`
 + `KEY_INTERNATIONAL4`
 + `KEY_INTERNATIONAL5`
 + `KEY_INTERNATIONAL6`
 + `KEY_INTERNATIONAL7`
 + `KEY_INTERNATIONAL8`
 + `KEY_INTERNATIONAL9`
 + `KEY_LANG1`
 + `KEY_LANG2`
 + `KEY_LANG3`
 + `KEY_LANG4`
 + `KEY_LANG5`
 + `KEY_LANG6`
 + `KEY_LANG7`
 + `KEY_LANG8`
 + `KEY_LANG9`
 + `KEY_ALTERASE`
 + `KEY_SYSREQ`
 + `KEY_CANCEL`
 + `KEY_CLEAR`
 + `KEY_PRIOR`
 + `KEY_RETURN2`
 + `KEY_SEPARATOR`
 + `KEY_OUT`
 + `KEY_OPER`
 + `KEY_CLEARAGAIN`
 + `KEY_CRSEL`
 + `KEY_EXSEL`
 + `KEY_KP_00`
 + `KEY_KP_000`
 + `KEY_THOUSANDSSEPARATOR`
 + `KEY_DECIMALSEPARATOR`
 + `KEY_CURRENCYUNIT`
 + `KEY_CURRENCYSUBUNIT`
 + `KEY_KP_LEFTPAREN`
 + `KEY_KP_RIGHTPAREN`
 + `KEY_KP_LEFTBRACE`
 + `KEY_KP_RIGHTBRACE`
 + `KEY_KP_TAB`
 + `KEY_KP_BACKSPACE`
 + `KEY_KP_A`
 + `KEY_KP_B`
 + `KEY_KP_C`
 + `KEY_KP_D`
 + `KEY_KP_E`
 + `KEY_KP_F`
 + `KEY_KP_XOR`
 + `KEY_KP_POWER`
 + `KEY_KP_PERCENT`
 + `KEY_KP_LESS`
 + `KEY_KP_GREATER`
 + `KEY_KP_AMPERSAND`
 + `KEY_KP_DBLAMPERSAND`
 + `KEY_KP_VERTICALBAR`
 + `KEY_KP_DBLVERTICALBAR`
 + `KEY_KP_COLON`
 + `KEY_KP_HASH`
 + `KEY_KP_SPACE`
 + `KEY_KP_AT`
 + `KEY_KP_EXCLAM`
 + `KEY_KP_MEMSTORE`
 + `KEY_KP_MEMRECALL`
 + `KEY_KP_MEMCLEAR`
 + `KEY_KP_MEMADD`
 + `KEY_KP_MEMSUBTRACT`
 + `KEY_KP_MEMMULTIPLY`
 + `KEY_KP_MEMDIVIDE`
 + `KEY_KP_PLUSMINUS`
 + `KEY_KP_CLEAR`
 + `KEY_KP_CLEARENTRY`
 + `KEY_KP_BINARY`
 + `KEY_KP_OCTAL`
 + `KEY_KP_DECIMAL`
 + `KEY_KP_HEXADECIMAL`
 + `KEY_LCTRL`
 + `KEY_LSHIFT`
 + `KEY_LALT`
 + `KEY_LGUI`
 + `KEY_RCTRL`
 + `KEY_RSHIFT`
 + `KEY_RALT`
 + `KEY_RGUI`
 + `KEY_MODE`
 + `KEY_AUDIONEXT`
 + `KEY_AUDIOPREV`
 + `KEY_AUDIOSTOP`
 + `KEY_AUDIOPLAY`
 + `KEY_AUDIOMUTE`
 + `KEY_MEDIASELECT`
 + `KEY_WWW`
 + `KEY_MAIL`
 + `KEY_CALCULATOR`
 + `KEY_COMPUTER`
 + `KEY_AC_SEARCH`
 + `KEY_AC_HOME`
 + `KEY_AC_BACK`
 + `KEY_AC_FORWARD`
 + `KEY_AC_STOP`
 + `KEY_AC_REFRESH`
 + `KEY_AC_BOOKMARKS`
 + `KEY_BRIGHTNESSDOWN`
 + `KEY_BRIGHTNESSUP`
 + `KEY_DISPLAYSWITCH`
 + `KEY_KBDILLUMTOGGLE`
 + `KEY_KBDILLUMDOWN`
 + `KEY_KBDILLUMUP`
 + `KEY_EJECT`
 + `KEY_SLEEP`
 + `KEY_APP1`
 + `KEY_APP2`
 + `KEY_AUDIOREWIND`
 + `KEY_AUDIOFASTFORWARD`

### keyboard_get_key
`static keyboard_get_key(key)`

Parameters:
 + `key -> Num` One of the `KEY_*` values to check.

Returns true if `key` is currently held.

### keyboard_get_key_pressed
`static keyboard_get_key_pressed(key)`

Parameters:
 + `key -> Num` One of the `KEY_*` values to check.

Returns true if `key` was pressed this frame.

### keyboard_get_key_released
`static keyboard_get_key_released(key)`

Parameters:
 + `key -> Num` One of the `KEY_*` values to check.

Returns true if `key` was released this frame.

### get_mouse_position
`static get_mouse_position(camera)`

Parameters
 + `camera -> lib/Renderer::Camera` or `null` Camera to check mouse relative to.
 
Returns the mouse position relative to a given camera, or `null` for the mouse position
relative to the window. The mouse position is returned as a list with the values `[x, y]`.

> 📝 This method adjusts the mouse position to the camera to make the position relative to
> the game world if you want, but it does not work properly with rotated cameras or cameras
> that use `zoom` instead of `width/height`.

### get_mouse_left
`static get_mouse_left()`

Returns true if the left mouse buttons is currently held down.

### get_mouse_left_pressed
`static get_mouse_left_pressed()`

Returns true if the left mouse button was just pressed this frame.

### get_mouse_left_released
`static get_mouse_left_released()`

Returns true if the left mouse button was just released this frame.

### get_mouse_right
`static get_mouse_right()`

Returns true if the right mouse buttons is currently held down.

### get_mouse_right_pressed
`static get_mouse_right_pressed()`

Returns true if the right mouse button was just pressed this frame.

### get_mouse_right_released
`static get_mouse_right_released()`

Returns true if the right mouse button was just released this frame.

### get_mouse_middle
`static get_mouse_middle()`

Returns true if the middle mouse buttons is currently held down.

### get_mouse_middle_pressed
`static get_mouse_middle_pressed()`

Returns true if the middle mouse button was just pressed this frame.

### get_mouse_middle_released
`static get_mouse_middle_released()`

Returns true if the middle mouse button was just released this frame.


-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)