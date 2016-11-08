========================================================================
    CONSOLE APPLICATION : TaraDisparityViewer Project Overview
========================================================================
Tara Disparity Viewer:

	Disparity along with the left and right images are displayed.


Command to create TaraDisparityViewer binary:
=============================================

	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	Make sure the common libraries are built and the libs are placed in proper folder for linking.
	From the current directory type the command.
	
	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
	
		$ g++ DisparityViewer.cpp -o TaraDisparityViewer -I ./../../../common/include -I /usr/include/glib-2.0 -I /usr/lib/i386-linux-gnu/glib-2.0/include -L ./../../../../Prebuilts/Ubuntu-14.04/lib -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc

	For 64 bit OS, LTS 14.04	
		
		$ g++ DisparityViewer.cpp -o TaraDisparityViewer -I ./../../../common/include -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -L ./../../../../Prebuilts/Ubuntu-14.04/lib_x64 -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc

=============================================
