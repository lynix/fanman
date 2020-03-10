# FanMan

A Qt based GUI for [FanBoy](https://lynix.github.io/fanboy)

[![License: GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://opensource.org/licenses/GPL-3.0)
[![Build Status](https://travis-ci.org/lynix/fanman.svg?branch=master)](https://travis-ci.org/lynix/fanman)


## Summary

*FanMan* is a Qt5 based GUI for the *FanBoy* Open Source PWM USB fan controller.
It provides a convenient interface for monitoring temperatures and fan status as
well as for configuring fan control parameters.


## Requirements

This application relies on the Qt5 Application Framework, version 5.12 or later.
If your distribution (Linux) or operating system (Windows, MacOS) uses a modular
deployment of Qt make sure you have the following modules installed:

* qt5-base
* qt5-widgets
* qt5-charts
* qt5-serialport


## Building

The application makes use of [CMake](https://https://cmake.org) to provide a
cross-platform build environment. Builds can be done out-of-tree as well as from
within the repository.

A typical build on Linux can be done as follows:

```
$ cmake .
$ make -j8
```


## Usage

*FanMan* consists of a single executable. It auto-detects the (USB) serial port
to use for communicating with *FanBoy*. Please note that this mechanism relies
on USB *Vendor ID* and *Product ID* and may cause confusion with other
[Arduino](https://www.arduino.cc) based devices.

*FanMan* saves the latest position and state of the main window. For having it
run via autostart with hidden window, just make sure you close or hide
(left-click on the tray icon) it before terminating the application.


## Contributing

Pull requests and bug reports are greatly appreciated. Please use the
appropriate GitHub functions.


## License

This project is published under the terms of the GNU General Public License,
Version 3. See file `LICENSE`.
