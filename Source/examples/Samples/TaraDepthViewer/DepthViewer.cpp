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
 DepthViewer: Displays the depth of the point selected by the user
			   using the disparity image computed.
**********************************************************************/

#include "DepthViewer.h"

//Local point to access the user selected value
Point g_SelectedPoint(-1, -1);

//Initialises all the necessary files
int DepthViewer::Init()
{
	cout << endl << "		Depth Viewer Application " << endl  << endl;
	cout << " Depth Viewer - Displays the Filtered Disparity between the two frames" << endl << " Closer objects appear in Red and Farther objects appear in Blue Color!"<< endl;
	cout << " Select a point to display the depth of the point!" << endl  << endl;
	
	//Initialise the Camera
	if(!_Disparity.InitCamera(true, true))
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
int DepthViewer::CameraStreaming()
{	
	float DepthValue = 0;
	char WaitKeyStatus;
	bool GrayScaleDisplay = false;
	Mat LeftImage, RightImage;
	Mat gDisparityMap, gDisparityMap_viz;
	int BrightnessVal = 4;		//Default value

	//Window Creation
	namedWindow("Disparity Map", WINDOW_AUTOSIZE);
	namedWindow("Left Image", WINDOW_AUTOSIZE);
	namedWindow("Right Image", WINDOW_AUTOSIZE);

	//Mouse callback set to disparity window
        setMouseCallback("Disparity Map", DepthPointSelection);

	cout << endl << "Press q/Q/Esc on the Image Window to quit the application!" << endl;
	cout << endl << "Press e/E on the Image Window to change the exposure of the camera" << endl;
	cout << endl << "Press b/B on the Image Window to change the brightness of the camera" << endl;
	cout << endl << "Press d/D on the Image Window to view the grayscale disparity map!" << endl << endl;

	//Dispalys the filtered disparity, the depth of the point selected is displayed
	while(1)
	{
		_Disparity.GrabFrame(&LeftImage, &RightImage); //Grabs the frame and returns the rectified image		
	
		//Get disparity
		_Disparity.GetDisparity(LeftImage, RightImage, &gDisparityMap, &gDisparityMap_viz);

		//Estimate the Depth of the point selected
		_Disparity.EstimateDepth(g_SelectedPoint, &DepthValue);

		if(g_SelectedPoint.x > -1 && DepthValue > 0) //Mark the point selected by the user
			circle(gDisparityMap_viz, g_SelectedPoint, 3, Scalar::all(0), 3, 8);

		if(DepthValue > 0)
		{
			stringstream ss;
			ss << DepthValue / 10 << " cm\0" ;
			DisplayText(gDisparityMap_viz, ss.str(), g_SelectedPoint);
		}

		//Display the Images
		imshow("Disparity Map", gDisparityMap_viz);		
		imshow("Left Image",  LeftImage);		
		imshow("Right Image", RightImage);

		if(GrayScaleDisplay)
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
			
			if (!(cin >> BrightnessVal))
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
			
			if(!(cin >> ManualExposure))
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

//Call back function
void DepthPointSelection(int MouseEvent, int x, int y, int flags, void* param) 
{
	if(MouseEvent == CV_EVENT_LBUTTONDOWN)  //Clicked
	{
		g_SelectedPoint = Point(x, y);
	}
}

//Main Application
int main()
{
	if(DEBUG_ENABLED) 
		cout << "Depth Viewer\n";

	//Object creation
	DepthViewer _DepthViewer;
	_DepthViewer.Init();

	if(DEBUG_ENABLED) 
		cout << "Exit : Depth Viewer\n";

	return TRUE;
}
