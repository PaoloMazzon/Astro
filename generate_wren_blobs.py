def bin_to_4hex(binary):
	out = hex(binary)
	if len(out) == 3:
		return out[0] + out[1] + "0" + out[2]
	else:
		return out

def add_file_string(fname):
    string = ""
    with open(fname, "r") as f:
        var_name = fname.split(".")[0].split("/")[-1].upper()
        code = ""
        count = 0
        for i in f.read():
            code += bin_to_4hex(ord(i)) + ", "
            count += 1
            if (count == 13):
                  count = 0
                  code += "\n    "
        code = code[:-2]
        string = "\nconst char " + var_name + "_WREN_SOURCE[] = {\n    " + code + "\n};\n"
    return string

if (__name__ == "__main__"):
    string = """/// \\file WrenHeaders.h
/// \\author Paolo Mazzon
/// \\brief Automatically generated file from generate_wren_blobs.py
#pragma once
"""
    string += add_file_string("prog/lib/Audio.wren")
    string += add_file_string("prog/lib/Drawing.wren")
    string += add_file_string("prog/lib/Engine.wren")
    string += add_file_string("prog/lib/File.wren")
    string += add_file_string("prog/lib/Input.wren")
    string += add_file_string("prog/lib/Renderer.wren")
    string += add_file_string("prog/lib/Util.wren")
    string += add_file_string("prog/lib/Tiled.wren")
    with open("src/WrenHeaders.h", "w") as f:
        f.write(string)
    