///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016, e-Con Systems.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
// ANY DIRECT/INDIRECT DAMAGES HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/**********************************************************************
	OpenCVViewer: Defines the methods to view the stereo images from 
				   the camera.
	
**********************************************************************/

#include "OpenCVViewer.h"

//Initialises all the variables and methods
int OpenCVViewer::Init()
{	
	cout << endl  << "		OpenCV Viewer Application " << endl << " Displays the rectified left and right image!" << endl  << endl;

	//Init 
	if(!_Disparity.InitCamera(false, false)) //Initialise the camera
	{
		if(DEBUG_ENABLED)
			cout << "Camera Initialisation Failed!\n";
		return FALSE;
	}
		
	ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;
				
	//Setting up the exposure
	if(ManualExposure > 10)
		_Disparity.SetExposure(ManualExposure);

	//Streams the camera and process the height
	TaraViewer();

	return TRUE;
}

//Streams using OpenCV Application
//Converts the 10 bit data to 8 bit data and splits the left an d right image separately
int OpenCVViewer::TaraViewer()
{
	char WaitKeyStatus;
	Mat LeftImage, RightImage;
	int BrightnessVal = 4;		//Default Value

	//Window Creation
	namedWindow("Left Frame", WINDOW_AUTOSIZE);
	namedWindow("Right Frame", WINDOW_AUTOSIZE); 

	cout << endl << "Press q/Q/Esc on the Image Window to quit the application!" << endl;
	cout << endl << "Press e/E on the Image Window to change the exposure of the camera" << endl;
	cout << endl << "Press b/B on the Image Window to change the brightness of the camera" << endl << endl;

	//Streams the Camera using the OpenCV Video Capture Object
	while(1)
	{
		_Disparity.GrabFrame(&LeftImage, &RightImage); //Grabs the frame and returns the rectified image
				
		//Display the images	
		imshow("Left Frame", LeftImage);		
		imshow("Right Frame", RightImage);

		//waits for the Key input
		WaitKeyStatus = waitKey(10);
		if(WaitKeyStatus == 'q' || WaitKeyStatus == 'Q' || WaitKeyStatus == 27) //Quit
		{			
			break;
		}

		else if(WaitKeyStatus == 'b' || WaitKeyStatus == 'B') //Brightness
		{	
			cout << endl << "Enter the Brightness Value, Range(1 to 7): " << endl;
			
			if (!(cin >> BrightnessVal))
			{
				BrightnessVal = 4; // Default value
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			
			//Setting up the brightness of the camera
			if (BrightnessVal >= 1  && BrightnessVal <= 7)
			{
				//Setting up the brightness
				//In opencv-linux 3.1.0, the value needs to be normalized by max value (7)
				_Disparity.SetBrightness((double)BrightnessVal / 7.0);			
			}			
			else
			{
				 cout << endl << "Invalid Brightness Value!" << endl;
			}			
		}
		else if(WaitKeyStatus == 'e' || WaitKeyStatus == 'E') //Set Exposure
		{			
			cout << endl << "Enter the Exposure Value Range(10 to 1000000 micro seconds): " << endl;
						
			if (!(cin >> ManualExposure))
			{
				ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			if(ManualExposure >= SEE3CAM_STEREO_EXPOSURE_MIN && ManualExposure <= SEE3CAM_STEREO_EXPOSURE_MAX)
			{					
				//Setting up the exposure
				_Disparity.SetExposure(ManualExposure);					
			}
			else
			{
				cout << endl << "Invalid Exposure Value!" << endl;
			}
		}
	}

	//Closes all the windows
	destroyAllWindows();

	return TRUE;
}

//Main function 
int main()
{
	if(DEBUG_ENABLED) 
		cout << "OpenCV Camera Viewer\n";
	
	//Object to access the application to view the stereo images  
	OpenCVViewer _OpenCVViewer;
	_OpenCVViewer.Init();
	
	if(DEBUG_ENABLED) 
		cout << "Exit : OpenCV Camera Viewer\n";

	return TRUE;
}

