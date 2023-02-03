## Buffer
`Buffer` provides a way to access binary buffers in the filesystem or just in memory
from Wren. Be aware that internally all number in Wren are doubles meaning that any
method that writes anything other than a double will simply cast the double and if you
have a value not representable by the desired data type this is undefined behaviour.
Any method that reads or writes data to/from the buffer expects the pointer to be
at least x bytes behind the size of the buffer, where x is the size of that datatype
(the size of each datatype is documented below). If you attempt more bytes than are
remaining after the pointer all the read meathods will return `null`.

You may write `Buffer`s to file using [write](File.md#write).

 + [new](#new)
 + [open](#open)
 + [resize](#resize)
 + [size](#size)
 + [pointer](#pointer-getter)
 + [pointer=](#pointer-setter)
 + [read_double](#read_double)
 + [write_double](#write_double)
 + [read_float](#read_float)
 + [write_float](#write_float)
 + [read_uint64](#read_uint64)
 + [write_uint64](#write_uint64)
 + [read_uint32](#read_uint32)
 + [write_uint32](#write_uint32)
 + [read_uint16](#read_uint16)
 + [write_uint16](#write_uint16)
 + [read_uint8](#read_uint8)
 + [write_uint8](#write_uint8)
 + [read_int64](#read_int64)
 + [write_int64](#write_int64)
 + [read_int32](#read_int32)
 + [write_int32](#write_int32)
 + [read_int16](#read_int16)
 + [write_int16](#write_int16)
 + [read_int8](#read_int8)
 + [write_int8](#write_int8)
 + [read_string](#read_string)
 + [write_string](#write_string)
 + [read_bool](#read_bool)
 + [write_bool](#write_bool)

### new
`construct new(size)`

Parameters
 + `size -> Num` Size in bytes of the new buffer.
 
Creates a new buffer of size `size`.

### open
`open(filename)`

Parameters
 + `filename -> String` File to read the buffer from.
 
Loads a buffer from a file.

### resize
`resize(size)`

Parameters
 + `size -> Num` Size in bytes of the buffer.

Resizes a buffer, does not move the pointer and retains all original data if the
new size is larger than the previous one.

### size
`size`

Returns the size of the buffer in bytes.

### pointer (Setter)
`pointer=(val)`

Parameters
 + `val -> Num` Where the pointer should be.

Moves the pointer to `val`, `val` must be between 0-`size`.

### pointer (Getter)
`pointer`

Returns the current pointer position.

### read_double
`read_double()`

Returns the next 8 bytes starting from `pointer` as a double, then moves the pointer
that many bytes forward.

### write_double
`write_double(double)`

Parameters
 + `double -> Num` Value to write.

Writes a `Num` to the buffer as a double, moving the pointer 8 bytes forward.

### read_float
`read_float()`

Returns the next 4 bytes starting from `pointer` as a float, then moves the pointer
that many bytes forward.

### write_float
`write_float(float)`

Parameters
 + `float -> Num` Value to write.

Writes a `Num` to the buffer as a float, moving the pointer 4 bytes forward.

### read_uint64
`read_uint64()`

Returns the next 8 bytes starting from `pointer` as a 64-bit unsigned integer, then moves the pointer
that many bytes forward.

### write_uint64
`write_uint64(uint64)`

Parameters
 + `uint64 -> Num` Value to write.

Writes a `Num` to the buffer as a uint64, moving the pointer 8 bytes forward.

### read_uint32
`read_uint32()`

Returns the next 4 bytes starting from `pointer` as a 32-bit unsigned integer, then moves the pointer
that many bytes forward.

### write_uint32
`write_uint32(uint32)`

Parameters
 + `uint32 -> Num` Value to write.

Writes a `Num` to the buffer as a uint32, moving the pointer 4 bytes forward.

### read_uint16
`read_uint16()`

Returns the next 2 bytes starting from `pointer` as a 16-bit unsigned integer, then moves the pointer
that many bytes forward.

### write_uint16
`write_uint16(uint16)`

Parameters
 + `uint16 -> Num` Value to write.

Writes a `Num` to the buffer as a uint16, moving the pointer 2 bytes forward.

### read_uint8
`read_uint8()`

Returns the next 1 byte starting from `pointer` as an 8-bit unsigned integer, then moves the pointer
that many bytes forward.

### write_uint8
`write_uint8(uint8)`

Parameters
 + `uint8 -> Num` Value to write.

Writes a `Num` to the buffer as a uint8, moving the pointer 1 bytes forward.

### read_int64
`read_int64()`

Returns the next 8 bytes starting from `pointer` as a 64-bit signed integer, then moves the pointer
that many bytes forward.

### write_int64
`write_int64(int64)`

Parameters
 + `int64 -> Num` Value to write.

Writes a `Num` to the buffer as a int64, moving the pointer 8 bytes forward.

### read_int32
`read_int32()`

Returns the next 4 bytes starting from `pointer` as a 32-bit signed integer, then moves the pointer
that many bytes forward.

### write_int32
`write_int32(int32)`

Parameters
 + `int32 -> Num` Value to write.

Writes a `Num` to the buffer as a int32, moving the pointer 4 bytes forward.

### read_int16
`read_int16()`

Returns the next 2 bytes starting from `pointer` as a 16-bit signed integer, then moves the pointer
that many bytes forward.

### write_int16
`write_int16(int16)`

Parameters
 + `int16 -> Num` Value to write.

Writes a `Num` to the buffer as a int16, moving the pointer 2 bytes forward.

### read_int8
`read_int8()`

Returns the next 1 byte starting from `pointer` as an 8-bit signed integer, then moves the pointer
that many bytes forward.

### write_int8
`write_int8(int8)`

Parameters
 + `int8 -> Num` Value to write.

Writes a `Num` to the buffer as a int8, moving the pointer 1 bytes forward.

### read_string
`read_string(size)`

Parameters
 + `size -> Num` Desired size of the string.

Returns the next `size` bytes as a string, moving the pointer `size` forward.

> 📝 Internally `Buffer` places a null-terminating character at the end of the string. Any 0 values
> within the next `size` characters are not accounted for and will result in the string not being
> of length `size`.

### write_string
`write_string(string)`

Parameters
 + `string -> String` String to write to the buffer.

Writes a string to the buffer, moving the pointer `string.count` forward. This does not write
the null-terminating character to the buffer, as [read_string](#read_string) does not require
null-terminating characters in the buffer.

### read_bool
`read_bool()`

Returns the next 1 byte starting from `pointer` as a boolean, then moves the pointer
that many bytes forward.

### write_bool
`write_bool(bool)`

Parameters
 + `bool -> Bool` Value to write.

Writes a `Bool` to the buffer as a bool, moving the pointer 1 bytes forward.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)