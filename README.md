# VelocityPCI Management CLI and Library

## Summary

The code hosted here provides an easy to use CLI and C library for
communicating with Microchip's VelocityPCI management interface. It
utilizes the switchtec kernel module which, with luck, will be upstream
for 4.11. Otherwise it can be obtained from the [switchtec-kernel]
github repository and compiled for other supported kernels.

Currently the following features are supported:

* Show status for each port on all partitions
* Measure bandwidth and latency for each port
* Display and wait on event occurrances
* Setup and show event counters for various types of events
* Dump firmware logs
* Send a hard reset command to the switch
* Update and readback firmware as well as display image version and CRC info
* A simple ncurses GUI that shows salient information for the switch
* Display and configure manufacturing settings

Detailed documentation for the project can be found on the [Github Pages] site.

[switchtec-kernel]: https://github.com/Microsemi/switchtec-kernel
[Github Pages]: https://microchiptech.github.io/velocitypci-cli/

## Dependencies

This program has an optional build dependencies on the following libraries:

* libncurses5-dev (without it, 'switchtec gui' will not work)
* libssl-dev (without it, some of the commands in 'switchtec mfg' will not work)

## Installation

Installation is simple, with:

~~~
./configure
make
sudo make install
~~~~

## Building on Windows

MSYS2 should be used to build velocitypci-cli on windows seeing it
provides a reasonably compatible compiler. (Visual C++ still is
missing full support of C99.) To setup an environment:

1. Install MSYS2 by following the instructions at: http://www.msys2.org/
2. In an MSYS2 shell, run the following:

~~~
pacman -S --needed base-devel mingw-w64-i686-toolchain mingw-w64-x86_64-toolchain git
~~~

3. Close the MSYS2 shell, open an MINGW64 shell, checkout the project
and follow the above installation instructions as usual.

## Appendix

### Configuration Files
The config.h file included in this project is auto-generated by GNU Autoconf, a tool used to create portable configuration scripts for software packages.

### Why This Does Not Impact the MIT License

* The config.h file is generated based on the configure script, which is authored by the project maintainers and licensed under the MIT License.

* The use of GNU Autoconf to generate config.h does not impose any restrictions or change the licensing terms of the project. The MIT License remains fully applicable to all source code and files authored by the project maintainers.

* Files generated by GNU Autoconf, such as config.h, are considered part of the build process and do not affect the permissive nature of the MIT License applied to the project.

These files are distributed as part of the project to ensure ease of use and portability. Their inclusion does not affect the licensing of the project’s source code, which remains under the MIT License.
