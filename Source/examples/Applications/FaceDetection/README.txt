========================================================================
    CONSOLE APPLICATION : FaceDetection Project Overview
========================================================================
FaceDetection:

	Detects the face in both the left and right images using LBP Cascade Classifier of OpenCV.
	The Point from the face after detection is converted to a 3D point and the depth of the person from the camera is displayed.
 
Note: 
	Please make sure a folder named 'Face' is in the binary location with a file 'haarcascade_frontalface_alt2.xml'.
   

Command to create FaceDetection binary:
=======================================

	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	Make sure the common libraries are built and the libs are placed in proper folder for linking.
	From the current directory type the command.
	
	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
	
		$ g++ FaceDepth.cpp -o FaceDetection -I ./../../../common/include -I /usr/include/glib-2.0 -I /usr/lib/i386-linux-gnu/glib-2.0/include -L ./../../../../Prebuilts/Ubuntu-14.04/lib -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_videoio -lopencv_ximgproc

	For 64 bit OS, LTS 14.04
	
		$ g++ FaceDepth.cpp -o FaceDetection -I ./../../../common/include -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -L ./../../../../Prebuilts/Ubuntu-14.04/lib_x64 -lecon_tara -lecon_xunit -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_videoio -lopencv_ximgproc
		
=======================================