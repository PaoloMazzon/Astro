// Input.wren
// Author: Paolo Mazzon
// Input for your games

// Binds all JamUtil input functions
class Input {
    static KEY_A { 4 }
    static KEY_B { 5 }
    static KEY_C { 6 }
    static KEY_D { 7 }
    static KEY_E { 8 }
    static KEY_F { 9 }
    static KEY_G { 10 }
    static KEY_H { 11 }
    static KEY_I { 12 }
    static KEY_J { 13 }
    static KEY_K { 14 }
    static KEY_L { 15 }
    static KEY_M { 16 }
    static KEY_N { 17 }
    static KEY_O { 18 }
    static KEY_P { 19 }
    static KEY_Q { 20 }
    static KEY_R { 21 }
    static KEY_S { 22 }
    static KEY_T { 23 }
    static KEY_U { 24 }
    static KEY_V { 25 }
    static KEY_W { 26 }
    static KEY_X { 27 }
    static KEY_Y { 28 }
    static KEY_Z { 29 }
    static KEY_1 { 30 }
    static KEY_2 { 31 }
    static KEY_3 { 32 }
    static KEY_4 { 33 }
    static KEY_5 { 34 }
    static KEY_6 { 35 }
    static KEY_7 { 36 }
    static KEY_8 { 37 }
    static KEY_9 { 38 }
    static KEY_0 { 39 }
    static KEY_RETURN { 40 }
    static KEY_ESCAPE { 41 }
    static KEY_BACKSPACE { 42 }
    static KEY_TAB { 43 }
    static KEY_SPACE { 44 }
    static KEY_MINUS { 45 }
    static KEY_EQUALS { 46 }
    static KEY_LEFTBRACKET { 47 }
    static KEY_RIGHTBRACKET { 48 }
    static KEY_BACKSLASH { 49 }
    static KEY_NONUSHASH { 50 }
    static KEY_SEMICOLON { 51 }
    static KEY_APOSTROPHE { 52 }
    static KEY_GRAVE { 53 }
    static KEY_COMMA { 54 }
    static KEY_PERIOD { 55 }
    static KEY_SLASH { 56 }
    static KEY_CAPSLOCK { 57 }
    static KEY_F1 { 58 }
    static KEY_F2 { 59 }
    static KEY_F3 { 60 }
    static KEY_F4 { 61 }
    static KEY_F5 { 62 }
    static KEY_F6 { 63 }
    static KEY_F7 { 64 }
    static KEY_F8 { 65 }
    static KEY_F9 { 66 }
    static KEY_F10 { 67 }
    static KEY_F11 { 68 }
    static KEY_F12 { 69 }
    static KEY_PRINTSCREEN { 70 }
    static KEY_SCROLLLOCK { 71 }
    static KEY_PAUSE { 72 }
    static KEY_INSERT { 73 }
    static KEY_HOME { 74 }
    static KEY_PAGEUP { 75 }
    static KEY_DELETE { 76 }
    static KEY_END { 77 }
    static KEY_PAGEDOWN { 78 }
    static KEY_RIGHT { 79 }
    static KEY_LEFT { 80 }
    static KEY_DOWN { 81 }
    static KEY_UP { 82 }
    static KEY_NUMLOCKCLEAR { 83 }
    static KEY_KP_DIVIDE { 84 }
    static KEY_KP_MULTIPLY { 85 }
    static KEY_KP_MINUS { 86 }
    static KEY_KP_PLUS { 87 }
    static KEY_KP_ENTER { 88 }
    static KEY_KP_1 { 89 }
    static KEY_KP_2 { 90 }
    static KEY_KP_3 { 91 }
    static KEY_KP_4 { 92 }
    static KEY_KP_5 { 93 }
    static KEY_KP_6 { 94 }
    static KEY_KP_7 { 95 }
    static KEY_KP_8 { 96 }
    static KEY_KP_9 { 97 }
    static KEY_KP_0 { 98 }
    static KEY_KP_PERIOD { 99 }
    static KEY_NONUSBACKSLASH { 100 }
    static KEY_APPLICATION { 101 }
    static KEY_POWER { 102 }
    static KEY_KP_EQUALS { 103 }
    static KEY_F13 { 104 }
    static KEY_F14 { 105 }
    static KEY_F15 { 106 }
    static KEY_F16 { 107 }
    static KEY_F17 { 108 }
    static KEY_F18 { 109 }
    static KEY_F19 { 110 }
    static KEY_F20 { 111 }
    static KEY_F21 { 112 }
    static KEY_F22 { 113 }
    static KEY_F23 { 114 }
    static KEY_F24 { 115 }
    static KEY_EXECUTE { 116 }
    static KEY_HELP { 117 }
    static KEY_MENU { 118 }
    static KEY_SELECT { 119 }
    static KEY_STOP { 120 }
    static KEY_AGAIN { 121 }
    static KEY_UNDO { 122 }
    static KEY_CUT { 123 }
    static KEY_COPY { 124 }
    static KEY_PASTE { 125 }
    static KEY_FIND { 126 }
    static KEY_MUTE { 127 }
    static KEY_VOLUMEUP { 128 }
    static KEY_VOLUMEDOWN { 129 }
    static KEY_KP_COMMA { 133 }
    static KEY_KP_EQUALSAS400 { 134 }
    static KEY_INTERNATIONAL1 { 135 }
    static KEY_INTERNATIONAL2 { 136 }
    static KEY_INTERNATIONAL3 { 137 }
    static KEY_INTERNATIONAL4 { 138 }
    static KEY_INTERNATIONAL5 { 139 }
    static KEY_INTERNATIONAL6 { 140 }
    static KEY_INTERNATIONAL7 { 141 }
    static KEY_INTERNATIONAL8 { 142 }
    static KEY_INTERNATIONAL9 { 143 }
    static KEY_LANG1 { 144 }
    static KEY_LANG2 { 145 }
    static KEY_LANG3 { 146 }
    static KEY_LANG4 { 147 }
    static KEY_LANG5 { 148 }
    static KEY_LANG6 { 149 }
    static KEY_LANG7 { 150 }
    static KEY_LANG8 { 151 }
    static KEY_LANG9 { 152 }
    static KEY_ALTERASE { 153 }
    static KEY_SYSREQ { 154 }
    static KEY_CANCEL { 155 }
    static KEY_CLEAR { 156 }
    static KEY_PRIOR { 157 }
    static KEY_RETURN2 { 158 }
    static KEY_SEPARATOR { 159 }
    static KEY_OUT { 160 }
    static KEY_OPER { 161 }
    static KEY_CLEARAGAIN { 162 }
    static KEY_CRSEL { 163 }
    static KEY_EXSEL { 164 }
    static KEY_KP_00 { 176 }
    static KEY_KP_000 { 177 }
    static KEY_THOUSANDSSEPARATOR { 178 }
    static KEY_DECIMALSEPARATOR { 179 }
    static KEY_CURRENCYUNIT { 180 }
    static KEY_CURRENCYSUBUNIT { 181 }
    static KEY_KP_LEFTPAREN { 182 }
    static KEY_KP_RIGHTPAREN { 183 }
    static KEY_KP_LEFTBRACE { 184 }
    static KEY_KP_RIGHTBRACE { 185 }
    static KEY_KP_TAB { 186 }
    static KEY_KP_BACKSPACE { 187 }
    static KEY_KP_A { 188 }
    static KEY_KP_B { 189 }
    static KEY_KP_C { 190 }
    static KEY_KP_D { 191 }
    static KEY_KP_E { 192 }
    static KEY_KP_F { 193 }
    static KEY_KP_XOR { 194 }
    static KEY_KP_POWER { 195 }
    static KEY_KP_PERCENT { 196 }
    static KEY_KP_LESS { 197 }
    static KEY_KP_GREATER { 198 }
    static KEY_KP_AMPERSAND { 199 }
    static KEY_KP_DBLAMPERSAND { 200 }
    static KEY_KP_VERTICALBAR { 201 }
    static KEY_KP_DBLVERTICALBAR { 202 }
    static KEY_KP_COLON { 203 }
    static KEY_KP_HASH { 204 }
    static KEY_KP_SPACE { 205 }
    static KEY_KP_AT { 206 }
    static KEY_KP_EXCLAM { 207 }
    static KEY_KP_MEMSTORE { 208 }
    static KEY_KP_MEMRECALL { 209 }
    static KEY_KP_MEMCLEAR { 210 }
    static KEY_KP_MEMADD { 211 }
    static KEY_KP_MEMSUBTRACT { 212 }
    static KEY_KP_MEMMULTIPLY { 213 }
    static KEY_KP_MEMDIVIDE { 214 }
    static KEY_KP_PLUSMINUS { 215 }
    static KEY_KP_CLEAR { 216 }
    static KEY_KP_CLEARENTRY { 217 }
    static KEY_KP_BINARY { 218 }
    static KEY_KP_OCTAL { 219 }
    static KEY_KP_DECIMAL { 220 }
    static KEY_KP_HEXADECIMAL { 221 }
    static KEY_LCTRL { 224 }
    static KEY_LSHIFT { 225 }
    static KEY_LALT { 226 }
    static KEY_LGUI { 227 }
    static KEY_RCTRL { 228 }
    static KEY_RSHIFT { 229 }
    static KEY_RALT { 230 }
    static KEY_RGUI { 231 }
    static KEY_MODE { 257 }
    static KEY_AUDIONEXT { 258 }
    static KEY_AUDIOPREV { 259 }
    static KEY_AUDIOSTOP { 260 }
    static KEY_AUDIOPLAY { 261 }
    static KEY_AUDIOMUTE { 262 }
    static KEY_MEDIASELECT { 263 }
    static KEY_WWW { 264 }
    static KEY_MAIL { 265 }
    static KEY_CALCULATOR { 266 }
    static KEY_COMPUTER { 267 }
    static KEY_AC_SEARCH { 268 }
    static KEY_AC_HOME { 269 }
    static KEY_AC_BACK { 270 }
    static KEY_AC_FORWARD { 271 }
    static KEY_AC_STOP { 272 }
    static KEY_AC_REFRESH { 273 }
    static KEY_AC_BOOKMARKS { 274 }
    static KEY_BRIGHTNESSDOWN { 275 }
    static KEY_BRIGHTNESSUP { 276 }
    static KEY_DISPLAYSWITCH { 277 }
    static KEY_KBDILLUMTOGGLE { 278 }
    static KEY_KBDILLUMDOWN { 279 }
    static KEY_KBDILLUMUP { 280 }
    static KEY_EJECT { 281 }
    static KEY_SLEEP { 282 }
    static KEY_APP1 { 283 }
    static KEY_APP2 { 284 }
    static KEY_AUDIOREWIND { 285 }
    static KEY_AUDIOFASTFORWARD { 286 }

    // Returns true if a key is currently press (use `Input.KEY_*`)
    foreign static keyboard_get_key(key)

    // Returns true if a key was just pressed this frame
    foreign static keyboard_get_key_pressed(key)

    // Returns true if a key was just released this frame
    foreign static keyboard_get_key_released(key)

    // Gets the position of the mouse as a list of [x, y] in the game world 
    // relative to a camera (use `null` to get the mouse relative to the window)
    foreign static get_mouse_position(camera)
    
    // Returns true if the left mouse button is currently held
    foreign static get_mouse_left()
    
    // Returns true if the left mouse button is pressed
    foreign static get_mouse_left_pressed()
    
    // Returns true if the left mouse button is released
    foreign static get_mouse_left_released()
    
    // Returns true if the right mouse button is currently held
    foreign static get_mouse_right()
    
    // Returns true if the right mouse button was just pressed
    foreign static get_mouse_right_pressed()
    
    // Returns true if the right mouse button was just released
    foreign static get_mouse_right_released()
    
    // Returns true if the middle mouse button is currently held
    foreign static get_mouse_middle()
    
    // Returns true if the middle mouse button was just pressed
    foreign static get_mouse_middle_pressed()
    
    // Returns true if the middle mouse button was just released
    foreign static get_mouse_middle_released()
}