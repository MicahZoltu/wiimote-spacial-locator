#Compiling

Currently there is only a Win32 implementation for the Wii Remote driver though it is designed such that a port to other operating systems should be simple.

The project and solution files are for Visual Studio 2008.  Compiling under another operating system/compiler should be simple, though I do not have any other project types setup.  If you are building for Windows you will need the Windows Driver Development Kit from Microsoft.  I build with version 3790.1830 but it's possible it will work with other versions.

If you meet the above requirements building is as easy as opening the .sln in the root directory and pressing F5.