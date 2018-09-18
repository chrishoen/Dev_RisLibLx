# Dev_RisLibLx
### real time infrastructure library, C++, vstudio, linux

This is the Linux version of RisLib.

This library is written for Linux in Visual Studio. The classes are based on Linux realtime functions for pthreads and sockets and such. The library is portable. The details of the provided functionality that are specific to linux are contained in a small set of `*_linux.cpp` files. There is a corresponding library for Windows, RisLib, with a corresponding small set of `*_win.cpp` files that are specific to Windows. It is the intention of the library that programs built under Windows can be rebuilt seamlessly under Linux.

The current target is an asus tinkerboard, which is a raspberry pi with more memory and wifi, with Debian and gcc ARMv7.
