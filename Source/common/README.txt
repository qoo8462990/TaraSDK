========================================================================
    Tara - SHARED OBJECT LIBRARY : Tara Project Overview
========================================================================

This library defines the functions for Commonly used functions.

TaraCamParameters:
	Its used to load camera parameters i.e the calibrated files from the camera flash and compute the Q matrix.

eDisparity:
	This class contains methods to estimate the disparity, get depth of the point selected, remap/ rectify the images. 

CameraEnumeration:
	This class enumerates the camera device connected to the PC and list outs the resolution supported. Initialises the camera with the resolution selected. 
	Initialises the Extension unit.

	
========================================================================
    xunit - SHARED OBJECT LIBRARY : xunit_lib_tara Project Overview
========================================================================

This library defines the functions for HID commands.
The following HID commands are implemented	

	(i)	InitExtensionUnit
	(ii)	DeinitExtensionUnit
	(iii)	ReadFirmwareVersion
	(iv)	GetCameraUniqueID
	(v)	GetManualExposureValue_Stereo
	(vi)	SetManualExposureValue_Stereo
	(vii)	GetIMUConfig
	(viii)	SetIMUConfig
	(ix)	ControlIMUCapture
	(x)	GetIMUValueBuffer
	(xi)	StereoCalibRead

For detailed explanation, please refer to the document Tara_Extension_Unit_API_Document.pdf provided 
in the release package.


========================================================================
    include - Common Headers to use in the application
========================================================================
	(i)  Tara.h
	(ii) xunit_lib_tara.h
