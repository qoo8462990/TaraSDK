========================================================================
    SHARED OBJECT LIBRARY : xunit_lib_tara Project Overview
========================================================================

This library defines the functions for HID commands.
The following HID commands are implemented,

	(i)	InitExtensionUnit
	(ii)	UninitExtensionUnit
	(iii)	ReadFirmwareVersion
	(iv)	GetCameraUniqueID
	(v)	GetManualExposureValue_Stereo
	(vi)	SetManualExposureValue_Stereo
	(vii)	GetIMUConfig
	(viii)	SetIMUConfig
	(ix)	ControlIMUCapture
	(x)	GetIMUValueBuffer
	(xi)	StereoCalibRead

For detailed explanation, please refer to the Tara_Extension_Unit_API_Document.pdf
document provided in the release package.


Command to create libecon_xunit.so:
===================================

	Make sure all the necessary dependencies and opencv-3.1.0 with e-con's patch are installed.
	From the current directory type the command.

	$ cd <current directory>
	
	For 32 bit OS, LTS 14.04
	
		$ g++ -Wall -g -fPIC -shared xunit_lib_tara.cpp -o libecon_xunit.so -I ./../include
				
	For 64 bit OS, LTS 14.04
	
		$ g++ -Wall -g -fPIC -shared xunit_lib_tara.cpp -o libecon_xunit.so -I ./../include
				
====================================
