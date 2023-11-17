def bin_to_4hex(binary):
	out = hex(binary)
	if len(out) == 3:
		return out[0] + out[1] + "0" + out[2]
	else:
		return out

def add_file_string(fname, var_type, var_suffix):
    string = ""
    with open(fname, "rb") as f:
        var_name = fname.split(".")[0].split("/")[-1].upper()
        code = ""
        count = 0
        for i in f.read():
            code += bin_to_4hex(i) + ", "
            count += 1
            if (count == 13):
                  count = 0
                  code += "\n    "
        code = code[:-2]
        string = "\n" + var_type + " " + var_name + var_suffix + "[] = {\n    " + code + "\n};\n"
    return string

if (__name__ == "__main__"):
    string = """/// \\file WrenHeaders.h
/// \\author Paolo Mazzon
/// \\brief Automatically generated file from generate_wren_blobs.py
#pragma once
"""
    string += add_file_string("prog/lib/Audio.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/Drawing.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/Engine.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/File.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/Input.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/Renderer.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/Util.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("prog/lib/Tiled.wren", "const char", "_WREN_SOURCE")
    string += add_file_string("loading.png", "const unsigned char", "_SCREEN_PNG")
    with open("src/WrenHeaders.h", "w") as f:
        f.write(string)

    string = """/// \\file Blobs.h
/// \\author Paolo Mazzon
/// \\brief Automatically generated file from generate_wren_blobs.py
#pragma once
#include <stdint.h>
"""
    string += add_file_string("distro.png", "const uint8_t", "_PNG")
    string += add_file_string("font.png", "const uint8_t", "_PNG")
    with open("src/Blobs.h", "w") as f:
        f.write(string)
    