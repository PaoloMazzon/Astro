## File
`File` provides some basic tools for manipulating text files. This is a core class
that you are free to use before the engine is initialized.

+ [read](#read)
+ [write](#write)
+ [exists](#exists)

### read
`static read(filename)`

Parameters
 + `filename -> String` filename to read from.
 
Reads an entire file into a string and returns it. The file is not kept open.

### write
`static write(filename, contents)`

Parameters
 + `filename -> String` filename to write to.
 + `contents -> String` contents to write to the file.

Opens a file and writes `contents` to it, overwriting the file if it already exists.

### exists
`static exists(filename)`

Parameters
 + `filename -> String` filename to check.
 
Returns true if the given `filename` exists and false otherwise.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)