========================================================================
    CONSOLE APPLICATION : ImuApplication Project Overview
========================================================================
IMU application:

 A window with three circles denoting the three axis is created. The circles are rotated based on the rotation of the device.
 
 For detailed explanation, refer to the TARA-SDK_IMU_SampleApp_User_Manual.pdf in the documents folder.

 
Command to create ImuApplication binary:
========================================

	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	Make sure the common libraries are built and the libs are placed in proper folder for linking.
	From the current directory type the command.

	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
	
		$ g++ IMU_Sample.cpp -o ImuApplication -I ./../../common/include -I /usr/include/glib-2.0 -I /usr/lib/i386-linux-gnu/glib-2.0/include -L ./../../../Prebuilts/Ubuntu-14.04/lib -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc -lpthread

	For 64 bit OS, LTS 14.04
	
		$ g++ IMU_Sample.cpp -o ImuApplication -I ./../../common/include -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -L ./../../../Prebuilts/Ubuntu-14.04/lib_x64 -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc -lpthread
		
========================================
