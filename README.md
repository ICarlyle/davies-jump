# davies-jump
An endless vertical jumper staring Dr. Davies.  Written using GBA hardware, playable using a GBA emulator.

You can run this on a GBA emulator, but I used a virtual machine instance to both compile and run the game.  In the repo is an .ova file
that you will need to setup with Virtual Box.  The .ova file is available to download here: 

http://cs.umw.edu/~finlayson/class/spring16/cpsc305/misc/01-compiling-vm.html


Git clone:
https://github.com/ICarlyle/davies-jump


To compile the game:

`./gbacc2 delay.s jump.s theDashingDavies.c`

To run the game:

`gvba program.gba`
