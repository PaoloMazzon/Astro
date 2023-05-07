## File
`File` provides some basic tools for manipulating text files. This is a core class
that you are free to use before the engine is initialized.

+ [read](#read)
+ [read_from_pak](#read_from_pak)
+ [write](#write)
+ [exists](#exists)
+ [exists_in_pak](#exists_in_pak)
+ [remove](#remove)
+ [rename](#rename)

### read
`static read(filename)`

Parameters
 + `filename -> String` filename to read from.
 
Reads an entire file into a string and returns it. The file is not kept open.

### read_from_pak
`static read_from_pak(filename)`

Parameters
 + `filename -> String` filename to read from.
 
Reads an entire file into a string and returns it from `game.pak`. See [pak](../Pak.md) for
more information.

### write
`static write(filename, contents)`

Parameters
 + `filename -> String` filename to write to.
 + `contents -> String` or `Buffer` contents to write to the file.

Opens a file and writes `contents` to it, overwriting the file if it already exists. If contents
are a [Buffer](Buffer.md), the whole contents are dumped as binary to the file.

### exists
`static exists(filename)`

Parameters
 + `filename -> String` filename to check.
 
Returns true if the given `filename` exists and false otherwise.

### exists_in_pak
`static exists_in_pak(filename)`

Parameters
 + `filename -> String` filename to check.
 
Returns true if the given `filename` exists in `game.pak` and false otherwise. See [pak](../Pak.md) for
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

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)