// File.wren
// Author: Paolo Mazzon
// Access and modify files on the system

// Tools for accessing files
class File {
    // Reads an entire file into a string and returns it or NULL if the file doesn't exist
    foreign static read(filename)

    // Writes a string to a file
    foreign static write(filename, contents)

    // Returns true if a file exists, false otherwise
    foreign static exists(filename)
}

// Easy access for configuration files
foreign class INI {
    // Opens an INI file, returns an empty one if the file doesn't exist
    construct open(filename) {}

    // Stores the INI's contents to a file
    foreign flush(filename)

    // Returns true if the key exists in the INI
    foreign key_exists(header, key)

    // Gets a string from the INI
    foreign get_string(header, key, def)

    // Sets a string in the INI
    foreign set_string(header, key, val)

    // Gets a num from the INI
    foreign get_num(header, key, def)

    // Sets a num in the INI
    foreign set_num(header, key, val)

    // Gets a bool in the INI
    foreign get_bool(header, key, def)
    
    // Sets a bool in the INI
    foreign set_bool(header, key, val)
}