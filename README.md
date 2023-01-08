# VSeth16

[![GitHub Workflow Status](https://github.com/AgitatedAlice/VSeth16/workflows/cmake/badge.svg)](https://github.com/AgitatedAlice/VSeth16/actions?query=workflow%3Acmake)

 A stack oriented 16 bit fantasy console VM implemented solely with raylib and the standard C library.  
 Currently extremely WIP.  
 Build tested with gcc, cmake and ninja on MSYS2 MINGW64 on Windows 10 64 bit.  
 To build:  
 > cmake -B build  
 > cd build  
 > ninja  

Dependency:
	- xorg-dev (for linux)

## How to not tear your hair out trying to build this:
	- Make sure you aren't trying to run the binary inside of a VM, raylib doesn't seem to like VMware's "gpu"
	- When building with MSYS2 make sure you run the correct instance, msys2 mingw64 for example. Also make sure you got the right cmake package installed through pacman. The basic cmake will not work! Install the one for w64 with pacman -S mingw-w64-x86_64-cmake
	- Remember the daily offerings to the machine spirit, light a candle, try to plug in a USB and flip it several times around before correctly pushing it in. Caress your toaster, etc.
	
