# libcitrus
A C library for implementing modern multiplayer stacker games. To see
libcitrus in use, check out [txtris](https://github.com/RZ781/txtris).

To build libcitrus, simply run `./build`. If you are editing the source
code, use `./check` to format your code and run tests.

The library's source code does not depend on libc, so it can be used
practically anywhere, such as your favourite operating system or an
embedded system. This also means no memory is allocated by the library
so you can easily use any method for allocation you prefer, like arenas
or C++ smart pointers. The current build script only targets Unix-like
systems so you may need to modify it for other uses.  Currently, there
is not a stable API or ABI - it is not yet ready for production use.

libcitrus is free software, distributed under the terms of the GNU
Lesser General Public License as published by the Free Software
Foundation, either version 2.1 of the License or later. See the file
COPYING.LESSER for more information. License copyright years may be
listed using range notation, e.g., 1996-2015, indicating that every year
in the range, inclusive, is a copyrightable year that would otherwise be
listed individually.
