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

/*************************************************************************
	HeightCalibration: Finds the depth of the base from the camera which
				is later used by the Height estimation for estimating the 
				human height.
*************************************************************************/
#include "HeightCalibration.h"

Point DepthPoint(-1, -1);

//Call back Function for the mouse click on the input
void onMouse(int event, int x, int y, int flags, void* userdata);

//Constructor
HeightCalibration::HeightCalibration(void)
{
	ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;
	DepthPointSelection = true;
}

//Initialises all the necessary files
int HeightCalibration::Init()
{
	cout << endl  << "		Height Calibration Application " << endl  << endl;
	cout << " Calibrates the height of the base from the camera!" << endl << " Select the point of which the depth is to estimated!" << endl << endl;
	
	//Initialise the camera 
	if(!_Disparity.InitCamera(true, true))
	{
		if(DEBUG_ENABLED)
			cout << "Camera Initialisation Failed\n";
		return FALSE;
	}

	//Setting up the exposure
	if(ManualExposure > 10)
		_Disparity.SetExposure(ManualExposure);
	
	//Streams the camera and process the height
	CameraStreaming();

	return TRUE;
}

//Streams the input from the camera
int HeightCalibration::CameraStreaming()
{	
	//Images
	Mat LeftImage, RightImage;
	int BrightnessVal = 4;		//Default value
	bool RImageDisplay = false;
	float Depth = 0, BaseDepth = 0;
	int i = 0;
	char WaitKeyStatus;
	
	cout << endl << "Press q/Q/Esc on the Image Window to quit the application!" << endl;
	cout << endl << "Press r/R on the Image Window to see the right image" << endl;
	cout << endl << "Press e/E on the Image Window to change the exposure of the camera" << endl;
	cout << endl << "Press b/B on the Image Window to change the brightness of the camera" << endl << endl;

	//Estimates the depth of the base from 10 frames and averages it 
	while(i < 10)
	{
		_Disparity.GrabFrame(&LeftImage, &RightImage); //Grabs the frame and returns the rectified image

		//DisparityMap
		_Disparity.GetDisparity(LeftImage, RightImage, &gDisparityMap, &gDisparityMap_viz);
		
		if(DepthPointSelection)
		{
			DepthPoint = SelectDepthPoint();
		}
		else
		{
			circle(gDisparityMap_viz, DepthPoint, 7, Scalar::all(255), -1, 8);
			DisplayText(gDisparityMap_viz, "Selected Point", DepthPoint);
		
			//Returns the deoth of the point specified
			_Disparity.EstimateDepth(DepthPoint, &Depth);
			
			BaseDepth += Depth;
			i++;

			imshow("Disparity Map", gDisparityMap_viz);			
		}
		
		//Display the images
		imshow("Left Frame", LeftImage);

		if(RImageDisplay)
                {
                        imshow("Right Frame", RightImage);
                }

			
		//waits for the Key input
		WaitKeyStatus = waitKey(100);
		if(WaitKeyStatus == 'q' || WaitKeyStatus == 'Q' || WaitKeyStatus == 27) //Quit
		{			
			break;
		}	
		else if(WaitKeyStatus == 'r' || WaitKeyStatus == 'R') //Show the right image
                {
                        if(!RImageDisplay)
                        {
                                RImageDisplay = true;
                                namedWindow("Right Frame", WINDOW_AUTOSIZE);
                        }
                        else
                        {
                                RImageDisplay = false;
                                destroyWindow("Right Frame");
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
			cout << endl << "Enter the Exposure Value Range(10 t0 1000000 micro seconds): " << endl;
			
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

	//Average value of 10 frames
	//mm to cm caonversions
	BaseDepth = BaseDepth / i / 10;
	
	//Generates a file with the depth value
	DepthFileGeneration(BaseDepth);

	//Closes all the windows
	destroyAllWindows();

	return TRUE;
}

//writes the Depth Value to a file
int HeightCalibration::DepthFileGeneration(float Depth)
{
	FILE *fp = NULL;
	stringstream ss;
	ss << Depth;
	
	//Open file
	fp = fopen("//usr//local//tara-sdk//bin//Height//BaseHeight.txt", "w");

	if(fp == NULL)
	{
		if(DEBUG_ENABLED)
			cout << "BaseHeight.txt file creation in the path /usr/local/tara-sdk/bin/height failed!!\n";
		return FALSE;
	}

	fwrite(ss.str().c_str(), 1, sizeof(char) * ss.str().size(), fp);
	fclose(fp);
	
	return TRUE;
}

//User gets to select the point for which the depth has to be determined
Point HeightCalibration::SelectDepthPoint()
{
	//Put text
	DisplayText(gDisparityMap_viz, "Select Depth Point", Point(10, 50));

	namedWindow("Depth Point Selection", WINDOW_AUTOSIZE);
	imshow("Depth Point Selection", gDisparityMap_viz);

	//Call back
	setMouseCallback("Depth Point Selection", onMouse, NULL);

	if(DepthPoint.x != -1)
	{		
		DepthPointSelection = false;
		destroyWindow("Depth Point Selection");
	}

	return DepthPoint;
}

//Call back Function for the mouse click on the input
void onMouse(int event, int x, int y, int flags, void* userdata)
{
	if(event == EVENT_LBUTTONDOWN)
	{	
		DepthPoint.x = x;
		DepthPoint.y = y;
    }

}

//Main Application
int main()
{
	if(DEBUG_ENABLED)
		cout << "Height Calibration - Application\n";

	//Object Creation
	HeightCalibration _HeightCalibration;
	_HeightCalibration.Init();

	if(DEBUG_ENABLED)
		cout << "Exit : Height Calibration - Application\n";
	return TRUE;
}
