# Problem 1 - XML Exercise

## Overview

This solution is a gzip-compressed XML writer built in C++. 

`BenXMLOutput` and `Serializer` work together to let you build up XML content and write it out to a `.gz` file.

## Classes

### `BenXMLOutput`

This is the main class you interact with. It holds an internal string buffer and a `gzFile` handle. You build up XML by calling methods on it, then flush the buffer to disk as you like ready.

A few things worth noting:
- If you don't include `.gz` in the filename, it gets added automatically.
- Everything outside of `Serializer` is agnostic to the type of data. `Serializer` should handle everything.
- `Flush()` writes the buffer to the file and clears it. You can call it as many times as you want.
- `Close()` flushes anything remaining and closes the file. In this way, calling `Flush()` explicitly isn't strictly necessary.
- The destructor will call `Close()` automatically if you forget, so no resource leaks.

### `Serializer`

A static utility class that converts C++ values to strings that XML can handle. 
There are still some unsupported types, so you might run intp a compile error if you try something crazy.

## Tests
There are some rudimentary unit tests fpr `BenXMLOutput` and `Serializer`. While there are probably some edge cases missing, they show the basic structure of the tests I'm used to writing in Java.

## Areas for Improvement
- The `Serializer` class could be expanded to support more types and custom formatting options.
- Error handling should be improved, especially around file I/O operations. Right now only print statements are used.
- The main method is currently just a simple demonstration. Functionality could be added to, for example, put in an end tag automatically when you start a new tag, or to support attributes on tags.
- The XML output is currently an ordinary string, without any formatting or indentation. 
