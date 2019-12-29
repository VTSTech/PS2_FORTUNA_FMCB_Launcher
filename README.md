# PS2_FORTUNA_FMCB_Launcher
 
<img src="https://i.gyazo.com/774ae88a9f1ee6e99bc769342e5b9704.png">

Demo 1 (No FMCB)

https://www.youtube.com/watch?v=DbfIw2nzEC4

Demo 2 (FMCB)

https://www.youtube.com/watch?v=KRZxFm5_U7A

Changelog:
<pre>
v0.2
Code cleanup
No longer loading Pad Manager
No longer Initializing DVD
Removed all delays
No longer in Beta
Now includes Source Code, Packed/Unpacked BOOT.ELF

v0.1-beta2
Added missing mc1 BOOT2.ELF detection
Made a '0 delay/no sleep()' build available.

v0.1-beta1

First public release
Checks for FMCB on either MC.
Checks for BOOT2.ELF in /FORTUNA/ on either MC
Boots FMCB if it is present.
Otherwise boots BOOT2.ELF
Includes packed/unpacked ELF</pre>

Credits:

Uses OSDInit.c/h & libcdvd_add.c/h from 'OSD Initialization Libraries' by SP193
Compiled with current PS2SDK as of Nov 2019
Packed with PS2-Packer v1.1.1-unofficial-09ac9c6 by Nicolas "Pixel" Noble 