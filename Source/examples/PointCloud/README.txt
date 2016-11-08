========================================================================
    CONSOLE APPLICATION : PointCloud Project Overview
========================================================================
	Point Cloud view of the scene is displayed.


Command to create PointCloudApp binary:
=======================================

	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	Make sure the common libraries are built and the libs are placed in proper folder for linking.
	From the current directory type the command.
	
	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
	
		$ g++ PointCloudApp.cpp -o PointCloud -I ./../../common/include -I /usr/include/glib-2.0 -I /usr/lib/i386-linux-gnu/glib-2.0/include -I /usr/include/pcl-1.7 -I /usr/include/eigen3 -I /usr/include/vtk-5.8 -L ./../../../Prebuilts/Ubuntu-14.04/lib -lecon_tara -lecon_xunit -lopencv_core -lopencv_calib3d -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc -Wno-deprecated -lpcl_visualization -lboost_system

	For 64 bit OS, LTS 14.04
	
		$ g++ PointCloudApp.cpp -o PointCloud -I ./../../common/include -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pcl-1.7 -I /usr/include/eigen3 -I /usr/include/vtk-5.8 -L ./../../../Prebuilts/Ubuntu-14.04/lib_x64 -lecon_tara -lecon_xunit -lopencv_core -lopencv_calib3d -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_ximgproc -Wno-deprecated -lpcl_visualization -lboost_system
		
========================================