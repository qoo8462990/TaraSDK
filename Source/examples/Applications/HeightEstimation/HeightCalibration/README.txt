========================================================================
    CONSOLE APPLICATION : HeightCalibration Project Overview
========================================================================
Height Calibration:

	This is used for calibrating the height of the base from the camera. 
	The base is determined by averaging(minimum of 10 frames) the depth of a point selected by a user on the disparity map.
	The depth calculated is written to the file named "BaseHeight" and placed under the folder named "Height" which is used by the height estimation to determine the height.
	This file is used by the height estimation to determine the height.

Note: 
	Make sure the folder "Height" is created in prior to launching the application.


Command to create HeightCalibration binary:
===========================================

	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	Make sure the common libraries are built and the libs are placed in proper folder for linking.
	From the current directory type the command.

	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
	
		$ g++ HeightCalibration.cpp -o HeightCalibration -I ./../../../../common/include -I /usr/include/glib-2.0 -I /usr/lib/i386-linux-gnu/glib-2.0/include -L ./../../../../../Prebuilts/Ubuntu-14.04/lib -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc

	For 64 bit OS, LTS 14.04
	
		$ g++ HeightCalibration.cpp -o HeightCalibration -I ./../../../../common/include -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -L ./../../../../../Prebuilts/Ubuntu-14.04/lib_x64 -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc
		
===========================================