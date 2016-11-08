Developing applications with Tara using OpenCV:
###############################################

If you are going to develop applications on your own using OpenCV, then this step has to be
done. To run the prebuilt binaries included in the SDK package ignore this step.

Prerequisites : opencv-3.1.0 & opencv_contrib-master

	(i) Download the latest version of opencv (Version 3.1) for Linux with the 
		following link. (http://opencv.org/downloads.html)

	(ii) Download the contrib modules additionally from the github repository.
		 (https://github.com/Itseez/opencv_contrib)

			$ unzip opencv_contrib-master
			$ unzip opencv-3.1.0

	(iii) Replace the existing 'cap_v4l.cpp' in the folder path : opencv-3.1.0/modules/videoio/src/
		  with e-con's patch file in the SDK folder path : Source/opencv-patch/
		
			$ cd opencv-3.1.0
			$ mkdir build && cd build
			$ cmake-gui

		(1) Browse the source code path	 :   <Dir>/opencv-3.1.0
		(2) Browse the build path 	 	 :   <Dir>/opencv-3.1.0/build
		(3) Press Configure button, with default native compilers set(Unix Makefiles)
		
		Select all the packages you want to use and press again the Configure button. For an easier overview of the build options make sure the Grouped option under the binary directory selection is turned on. 
		
		For some of the packages CMake may not find all of the required files or directories. In case of these CMake will throw an error in its output window (located at the bottom of the GUI) and set its field values, to not found constants. 	
		
		For these you need to manually set the queried directories or files path. After this press again the Configure button to see if the value entered by you was accepted or not. Do this until all entries are good and you cannot see errors in the field/value or the output part of the GUI.

		Make the following configurations,
		
		(4) Provide the path for extra modules,
			 OPENCV_EXTRA_MODULES_PATH : <Dir>/opencv_contrib-master/modules
		(5) Uncheck the WITH_LIBV4L option
		(6) Check the WITH_TBB & BUILD_TBB option

		(7) After configuring, and if there are no errors press the Generate button. Makefiles will be generated, close the GUI and enter the command.

	(iv)	$ sudo make -j4 install

		This will build and install OpenCV libraries in the location: /usr/local/lib/
