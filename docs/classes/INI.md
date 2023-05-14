---
layout: default
title: INI
parent: API Documentation
---

## INI 
You can easily access and save data to INI configuration files with this class.
If you just need some simple data saved to a file this class will often be easier
to use than manually manipulating files. This is a core class that you are free 
to use before the engine is initialized.

INI files are human-readable file format that organizes data into key/header pairs.
Files look something like the following:

    [header]
    key=value
    other_value=5.6
    
    [my data]
    player=False

+ [open](#open)
+ [flush](#flush)
+ [key_exists](#key_exists)
+ [get_string](#get_string)
+ [set_string](#set_string)
+ [get_num](#get_num)
+ [set_num](#set_num)
+ [get_bool](#get_bool)
+ [set_bool](#set_bool)

### open
`construct open(filename)`

Parameters
 + `filename -> String` file to read config from.
 
Creates a new INI class from an existing INI file, if the file does not exist it
will return an empty INI class.

### flush
`flush(filename)`

Parameters
 + `filename -> String` file to export the INI to.

Saves the contents of the class to a file in the form of a human-readable INI file.

### key_exists
`key_exists(header, key)`

Parameters
 + `header -> String` header to check from.
 + `key -> String` key to check for.

Returns true if the selected `key`/`header` pair exists in the INI, false otherwise.

### get_string
`get_string(header, key, def)`

Parameters
 + `header -> String` header to get the value in.
 + `key -> String` key to get the value for.
 + `def -> String` value that will be returned if they key doesn't exist.
 
Returns a value from the INI if it exists, otherwise it returns `def`.

### set_string
`set_string(header, key, val)`

Parameters
 + `header -> String` header to set value in.
 + `key -> String` key to set the value for.
 + `val -> String` value to set.
 
Sets a value in the INI.

### get_num
`get_num(header, key, def)`

Parameters
 + `header -> String` header to get the value in.
 + `key -> String` key to get the value for.
 + `def -> Num` value that will be returned if they key doesn't exist.
 
Returns a value from the INI if it exists, otherwise it returns `def`.

### set_num
`set_num(header, key, val)`

Parameters
 + `header -> String` header to set value in.
 + `key -> String` key to set the value for.
 + `val -> Num` value to set.
 
Sets a value in the INI.

### get_bool
`get_bool(header, key, def)`

Parameters
 + `header -> String` header to get the value in.
 + `key -> String` key to get the value for.
 + `def -> Bool` value that will be returned if they key doesn't exist.
 
Returns a value from the INI if it exists, otherwise it returns `def`.

### set_bool
`set_bool(header, key, val)`

Parameters
 + `header -> String` header to set value in.
 + `key -> String` key to set the value for.
 + `val -> Bool` value to set.
 
Sets a value in the INI.

