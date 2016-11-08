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
 DisparityViewer: Displays the disparity image.
**********************************************************************/

#include "DisparityViewer.h"

//Initialises all the necessary files
int DisparityViewer::Init()
{
	cout << endl  << "		Disparity Viewer Application " << endl  << endl;
	cout << " Disparity Viewer - Displays the Disparity between the two frames" << endl << " Closer objects appear in Red and Farther objects appear in Blue Color!"<< endl;
	cout << " Displays the actual disparity without any filter" << endl << endl; 
	//Initialise the camera
	if(!_Disparity.InitCamera(true, false))
	{
		if(DEBUG_ENABLED)
			cout << "Camera Initialisation Failed\n";
		return FALSE;
	}
	
	ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;
		
	//Setting up the exposure
	if(ManualExposure > 10)
		_Disparity.SetExposure(ManualExposure);

	//Camera Streaming
	CameraStreaming();	

	return TRUE;
}

//Streams the input from the camera
int DisparityViewer::CameraStreaming()
{	
	Mat gDisparityMap, gDisparityMap_viz;
	Mat LeftImage, RightImage;
	char WaitKeyStatus;
	bool GrayScaleDisplay = false;
	int  BrightnessVal = 4;		//Default value
	
	//Window creation
	namedWindow("Disparity Image", WINDOW_AUTOSIZE);
	namedWindow("Left Image", WINDOW_AUTOSIZE);
	namedWindow("Right Image", WINDOW_AUTOSIZE);	

	cout << endl << "Press q/Q/Esc on the Image Window to quit the application!" << endl;
	cout << endl << "Press e/E on the Image Window to change the exposure of the camera" << endl;
	cout << endl << "Press b/B on the Image Window to change the brightness of the camera" << endl;
	cout << endl << "Press d/D on the Image Window to see the grayscale disparity map" << endl << endl;

	//Displays the actual disparity of the Camera without any filter applied to the image
	while(1)
	{
		_Disparity.GrabFrame(&LeftImage, &RightImage); //Reads the frame and returns the rectified image
		
		//Get disparity
		_Disparity.GetDisparity(LeftImage, RightImage, &gDisparityMap, &gDisparityMap_viz);
				
		//Display the Images
		imshow("Disparity Image", gDisparityMap_viz);
		imshow("Left Image", LeftImage);		
		imshow("Right Image", RightImage);

		if(GrayScaleDisplay) //if Enabled
		{			
			imshow("Disparity Map GrayScale", gDisparityMap);						
		}

		//waits for the Key input
		WaitKeyStatus = waitKey(10);
		if(WaitKeyStatus == 'q' || WaitKeyStatus == 'Q' || WaitKeyStatus == 27) //Quit
		{			
			break;
		}
		else if(WaitKeyStatus == 'd' || WaitKeyStatus == 'D')
		{	
			if(!GrayScaleDisplay)
			{
				GrayScaleDisplay = true;
				namedWindow("Disparity Map GrayScale", WINDOW_AUTOSIZE);
			}
			else 
			{
				GrayScaleDisplay = false;
				destroyWindow("Disparity Map GrayScale");
			}
			
		}
		else if (WaitKeyStatus == 'b' || WaitKeyStatus == 'B') //Brightness
		{
			cout << endl << "Enter the Brightness Value, Range(1 to 7): " << endl;
			
			if(!(cin >> BrightnessVal))
			{
				BrightnessVal = 4; // Default value
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			//Setting up the brightness of the camera
			if (BrightnessVal >= 1 && BrightnessVal <= 7)
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
				ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;
				cout << endl << "Invalid Exposure Value!" << endl;
			}
		}
	}

	return TRUE;
}

//main application
int main()
{
	if(DEBUG_ENABLED)
		cout << "Disparity Viewer\n";

	//Disparity Viewer
	DisparityViewer _DisparityViewer;
	_DisparityViewer.Init();
	
	if(DEBUG_ENABLED)
		cout << "Exit : Disparity Viewer\n";

	return TRUE;
}

