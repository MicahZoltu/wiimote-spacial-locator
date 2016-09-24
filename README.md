This is part of a larger project to get "proper" head tracking into first person shooter. Previously, head tracking software used the head tracking as a means of controlling where your avatar in the virtual world was facing but this feels wrong because when you turn your head to the left, you are no longer looking at your monitor. If you were to have some kind of surround display, you would still not want the head tracking to do this because looking to the left would simply allow you to see the left monitor.

The correct immersive effect for a head tracking device is to allow the user to view the monitor as a window into another world, allowing them to look "around the corner" of the monitor. In order to do this, the the 3D application needs to use an asymmetric view frustum instead of the normal symmetric one.

The source code available here is for the:
* Wii remote driver (the part that let's you use a Wii remote on your PC).
* The spacial locator (the part that figures out where in space the LED is relative to the camera).
* A small test application to test the use of these two things.

For the curious, I did get this working in Half-Life 2 as a mod but unfortunately the HL2 engine doesn't allow a fine enough level of control over the rendering system to implement an asymmetric view frustum. Plus my 2 week Christmas vacation ended. :)

This code is not maintained anymore. If you are interested in making changes feel free to fork and if you make significant progress I'll drop a link to your fork in here.

### Compiling

Currently there is only a Win32 implementation for the Wii Remote driver though it is designed such that a port to other operating systems should be simple.

The project and solution files are for Visual Studio 2008. Compiling under another operating system/compiler should be simple, though I do not have any other project types setup. If you are building for Windows you will need the Windows Driver Development Kit from Microsoft. I build with version 3790.1830 but it's possible it will work with other versions.

If you meet the above requirements building is as easy as opening the .sln in the root directory and pressing F5.

----

This code was imported from Google Code: https://code.google.com/archive/p/wiimote-spacial-locator/
