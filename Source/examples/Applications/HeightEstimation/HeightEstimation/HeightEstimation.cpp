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
	HeightEstimation: Estimates the height of the person with reference 
				to the base height already found.
**********************************************************************/

#include "HeightEstimation.h"

//Constructor
HeightEstimation::HeightEstimation(void)
{
	//Initialise the depth
	HeadDepth = 0;
	BaseDepth = 0;
	ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;

	ss.precision(2);
}

//Initialises all the necessary files
int HeightEstimation::Init()
{
	cout << endl  << "		Height Estimation Application " << endl  << endl;
	cout << " Displays the height of the person below the camera with reference to the base height in the folder Height!" << endl << " Displays the Left Frame and the disparity map!" << endl << endl;
	
	//Initialise the disparity options
	if(!_Disparity.InitCamera(true, true))
	{
		if(DEBUG_ENABLED)
			cout << "Camera Initialisation Failed\n";
		return FALSE;
	}
	
	//Loads the camera matrix from the files
	if(!BaseHeightReading())
	{
		if(DEBUG_ENABLED)
			cout << "Reading Base Height Failed\n";
		return FALSE;
	}

	//Setting up the exposure
	if(ManualExposure  > 10)
		_Disparity.SetExposure(ManualExposure);	

	//Streams the camera and process the height
	CameraStreaming();

	return TRUE;
}

//Streams the input from the camera
int HeightEstimation::CameraStreaming()
{
	double PersonHeight = -1;
	char WaitKeyStatus;
	bool RImageDisplay = false;
	int BrightnessVal = 4;		//Default value
	
	//Window creation
	namedWindow("Disparity Map", WINDOW_AUTOSIZE);
	namedWindow("LFrame", WINDOW_AUTOSIZE);
	
	cout << endl << "Press q/Q/Esc on the Image Window to quit the application!" <<endl;
	cout << endl << "Press r/R on the Image Window to see the right image" << endl ;
	cout << endl << "Press e/E on the Image Window to change the exposure of the camera" << endl;
	cout << endl << "Press b/B on the Image Window to change the brightness of the camera" << endl << endl;

	//Estimates the height of the person standing under the camera
	while(1)
	{
		_Disparity.GrabFrame(&LeftImage, &RightImage); //Reads the frame and returns the rectified image
						
		//Disparity map
		_Disparity.GetDisparity(LeftImage, RightImage, &gDisparityMap, &gDisparityMap_viz);
		
		//Estimates the heights from the images passed after computing the disparity map
		PersonHeight = EstimateHeight();		

		if(PersonHeight > -1) 
		{
			//Displays the depth
			ss << "Height: " << fixed << PersonHeight / 0.0328084 << " cm(" << fixed << PersonHeight << "')";
			
			DisplayText(gDisparityMap_viz, ss.str(), Point(60, 80));
			ss.str(String());		
		}	
		
		//Display the images
		imshow("Disparity Map", gDisparityMap_viz);
		imshow("LFrame", LeftImage);
		
		if(RImageDisplay)
		{			
			imshow("Right Image", RightImage);
		}

		//waits for the Key input
		WaitKeyStatus = waitKey(10);
		if(WaitKeyStatus == 'q' || WaitKeyStatus == 'Q' || WaitKeyStatus == 27) //Quit
		{			
			break;
		}
		if (WaitKeyStatus == 'b' || WaitKeyStatus == 'B') //Brightness
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
		if(WaitKeyStatus == 'r' || WaitKeyStatus == 'R') //Show the right image
		{			
			if(!RImageDisplay)
			{
				RImageDisplay = true;
				namedWindow("Right Image", WINDOW_AUTOSIZE);
			}
			else 
			{
				RImageDisplay = false;
				destroyWindow("Right Image");
			}			
		}		
		if(WaitKeyStatus == 'e' || WaitKeyStatus == 'E') //Set Exposure
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

	//Closes all the windows
	destroyAllWindows();

	return TRUE;
}

//Detects the minimum depth at the top of the image
double HeightEstimation::EstimateHeight()
{
	//Minimum Height to avoid false detections
	double HeightThreshold = 100.0;
		
	Mat DepthHalf; 	
	Point minLoc, maxLoc;

	vector<float> DepthValue; 
	vector<Point> Depthpoints;	

	int DepthOffset = 20;
	float minDepthValue = 0, tempDepthValue = 0;
	double minVal = 0, maxVal = 0, HumanHeight = 0; 
	
	//Copying 1 / 3 of the image alone
	DepthHalf = gDisparityMap(Rect(0, 0, gDisparityMap.cols, 160)).clone();
	
	//Finding the Minimum and Maximum location of the image
	minMaxLoc(DepthHalf, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	//Estimating the Depth of the points in the image
	for(int i = 50; i < DepthHalf.cols - DepthOffset; i += DepthOffset)
	{
		for(int j = 50; j < DepthHalf.rows - DepthOffset; j += DepthOffset)
		{
			_Disparity.EstimateDepth(Point(i , j), &tempDepthValue);
			DepthValue.push_back(abs(tempDepthValue));
			Depthpoints.push_back(Point(i, j));
		}
	}

	//Copy the first value
	minDepthValue = DepthValue[0];

	//Finding the Lowest Depth
	for(size_t j = 0; j < DepthValue.size(); j++)
	{
		if(DepthValue[j] < minDepthValue)
		{
			minDepthValue = DepthValue[j];			
		}
	}

	//mm to cm conversion
	HeadDepth = minDepthValue / 10;	
	HumanHeight = BaseDepth - HeadDepth;

	//To avoid false detections
	if(HumanHeight > HeightThreshold)
	{			
		return (HumanHeight * 0.0328084);
	}
	else
	{
		return -1;
	}

}

//Reads the depth of the base from the camera
int HeightEstimation::BaseHeightReading()
{
	FILE *fp = NULL;
	char *BaseDepthValue;
	BaseDepthValue = new char[255];

	//Check whether it is a valid file
	fp = fopen("//usr//local//tara-sdk//bin//Height//BaseHeight.txt", "rb");
	if(fp == NULL)
	{		
		if(DEBUG_ENABLED)
			cout << "BaseHeight.txt Reference File Missing in the path /usr/local/tara-sdk/bin/Height\n";
		return FALSE;
	}

	//Read the content
	fread(BaseDepthValue, sizeof(char), 255, fp);
	fclose(fp);
	
	//Assign the value to the Depth variable
	BaseDepth = (float)atof(BaseDepthValue);

	if(BaseDepth <= -1) //Validate the Depth Value
	{		
		if(DEBUG_ENABLED)
			cout << "Invalid Base Depth\n";		
		return FALSE;
	}

	return TRUE;
} 

//Main function
int main()
{
	if(DEBUG_ENABLED)
		cout << "Height Estimation - Application\n";	
	
	//Object Creation
	HeightEstimation _HeightEstimation;
	if(!_HeightEstimation.Init())
	{
		cout << "\n HeightEstimation: Initialisation Failed!\n";
	}
	
	if(DEBUG_ENABLED)
		cout << "Exit : Height Estimation - Application\n";
	return TRUE;
}
