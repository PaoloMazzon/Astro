---
layout: default
title: File
parent: API Documentation
---

## File
`File` provides some basic tools for manipulating text files. This is a core class
that you are free to use before the engine is initialized.

+ [read()](#read)
+ [read_from_pak()](#read_from_pak)
+ [write()](#write)
+ [exists()](#exists)
+ [dir_exists()](#dir_exists)
+ [exists_in_pak()](#exists_in_pak)
+ [remove()](#remove)
+ [rename()](#rename)
+ [get_directory()](#get_directory)

### read
`static read(filename)`

Parameters
 + `filename -> String` filename to read from.
 
Reads an entire file into a string and returns it. The file is not kept open.

### read_from_pak
`static read_from_pak(filename)`

Parameters
 + `filename -> String` filename to read from.
 
Reads an entire file into a string and returns it from `game.pak`. See [pak](../Pak) for
more information.

{: .note }
This method expects the file to be in `game.pak` and not the file system, if you want
to know if the game is running from a pak see [using_pak](Engine#using_pak).

### write
`static write(filename, contents)`

Parameters
 + `filename -> String` filename to write to.
 + `contents -> String` or `Buffer` contents to write to the file.

Opens a file and writes `contents` to it, overwriting the file if it already exists. If contents
are a [Buffer](Buffer), the whole contents are dumped as binary to the file.

### exists
`static exists(filename)`

Parameters
 + `filename -> String` filename to check.
 
Returns true if the given `filename` exists and false otherwise.

### exists_in_pak
`static exists_in_pak(filename)`

Parameters
 + `filename -> String` filename to check.
 
Returns true if the given `filename` exists in `game.pak` and false otherwise. See [pak](../Pak) for
more information.

### remove
`static remove(filename)`

Parameters
 + `filename -> String` filename to delete.
 
Attempts to delete a file.

### rename
`static rename(old_filename, new_filename)`

Parameters
 + `old_filename -> String` original filename.
 + `new_filename -> String` name to rename the file to.
 
Attempts to rename a file.

### dir_exists
`static dir_exists(directory)`

Parameters
 + `directory -> String` The directory you want to check.

Returns true if a given directory was located.

### get_directory
`static get_directory(directory)`

Parameters
 + `directory -> String` The directory you want to get the contents of.

Returns a list of files and directories in a directory. The list does not differentiate
between the two so you will need to use `dir_exists` and `exists` to discern between the
two.

Assuming you have a directory of

    file.txt
    image.png
    data/
    docs/

```javascript
var directory = File.get_directory("./")
for (filename in directory) {
    if (File.exists(filename)) {
        System.print(filename + " is a file!")
    } else {
        System.print(filename + " is a directory!")
    }
}
```

Would produce

    ./ is a directory!
    ../ is a directory!
    file.txt is a file!
    image.png is a file!
    data/ is a directory!
    docs/ is a directory!