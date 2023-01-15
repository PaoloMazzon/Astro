// Save.wren
// Author: Paolo Mazzon
// An incredibly simple (and restrictive) API to save and load from a file

foreign class Save {
    // Loads a save from a file, if it doesn't exist it creates a new one
    construct new(filename) {}

    // Stores the save's contents to a file
    foreign flush(filename)

    // Returns true if the key exists in the save
    foreign key_exists(key)

    // Gets a string from the save
    foreign get_string(key)

    // Sets a string in the save
    foreign set_string(key, val)

    // Gets a num from the save
    foreign get_num(key)

    // Sets a num in the save
    foreign set_num(key, val)
}