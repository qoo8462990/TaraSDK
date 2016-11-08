========================================================================
    SHARED OBJECT LIBRARY : Tara Project Overview
========================================================================
/********** Options of the Project**********/

DISPARITY_OPTION :
==================
	when set to  1 - Best Quality Depth Map and Lower Frame Rate - Stereo_SGBM 3 Way generic Left and Right 
	when set to  0 - Low  Quality Depth Map and High  Frame Rate - Stereo_BM generic Left and Right 

	
Tara namespace :
=================
Tara namespace Tara has 3 Classes

1. TaraCamParameters:
	Its used to load camera parameters i.e the calibrated files from the camera flash and compute the Q matrix.

2. eDisparity:
	This class contains methods to estimate the disparity, get depth of the point selected, remap/ rectify the images. 

3. CameraEnumeration:
	This class enumerates the camera device connected to the PC and list outs the resolution supported. Initialises the camera with the resolution selected. 
	Initialises the Extension unit.

	
Command to create libecon_tara.so:
==================================
	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	From the current directory type the command.
	
	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
		
		$ g++ -Wall -g -fPIC -shared Tara.cpp -o libecon_tara.so -I ./../include -I /usr/include/glib-2.0 -I /usr/lib/i386-linux-gnu/glib-2.0/include -ludev -lv4l2
			
	For 64 bit OS, LTS 14.04
		
		$ g++ -Wall -g -fPIC -shared Tara.cpp -o libecon_tara.so -I ./../include -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -ludev -lv4l2
		
====================================
