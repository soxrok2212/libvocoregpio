# libvocoregpio
Basic GPIO drivers for VoCore v2

# Use Cases
This library has been written with the VoCore v2 in mind, which uses a MediaTek MT7628 chip. This code may work on other devices using the same chip, though support won't be provided for them.

Use this library if you wish to have fast, low-level access to GPIO on your VoCore. This library was written in C to circumvent slower and traditional linux-based GPIO solutions.

This library was written for OpenWrt 18.06 using a custom built firmware. Instructions for building the firmware and setting up the cross-compile environment can be found in the wiki.

# Compiling Instructions
By default, we include a makefile with targets to build .o, .so and .a. The shared library is installed to `/usr/local/lib` as `libvocoregpio.so`. For details on how to use this and examples, please check the wiki.

# Disclaimer
This software is not in any way affiliated with VoCore, Vonger, OpenWrt, Linux or anyone else. Support for this library may be dropped at any time. This is a third party solution and is provided free of charge under the GPLv3 license.
