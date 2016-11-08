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
	FaceDepth: Estimates the depth of the person from the camera by 
			    detecting the face of the person
**********************************************************************/
#include "FaceDepth.h"
#define RIGHTMATCH 150 //disparity range starts from 150 so in case of the point being less than 150 it means that the face is not fully covered in the right.

//Constructor
FaceDepth::FaceDepth(void)
{
	ManualExposure = SEE3CAM_STEREO_EXPOSURE_DEF_SDK;
	ss.precision(2); //Restricting the precision after decimal
}

//Destructor
FaceDepth::~FaceDepth(){};

//Initialises all the variables and methods
int FaceDepth::Init()
{	
	cout << endl << "		Face Detection Application " << endl  << endl;
	cout << " Detects multiple faces in the frame!" << endl << " Uses OpenCV Haarcascade file in the Face folder!" << endl;
	cout << " Rectangle around the detected faces with the depth is displayed!" << endl << " Only the left window is displayed!" << endl << endl;

	//Loads the cascade file
	FaceCascade = CascadeClassifier("//usr//local//tara-sdk//bin//Face//haarcascade_frontalface_alt2.xml");

	//If the file is not loaded properly
	if(FaceCascade.empty())
	{		
		if(DEBUG_ENABLED)
			cout << "haarcascade_frontalface_alt2.xml File Missing in the path /usr/local/tara-sdk/bin/Face!\n";		
		return FALSE;
	}
	

	if(!_Disparity.InitCamera(true, true)) //Initialise the camera
	{
		if(DEBUG_ENABLED)
			cout << "Camera Initialisation Failed!\n";
		return FALSE;
	}

	//Setting up the exposure
	if(ManualExposure > 10)
		_Disparity.SetExposure(ManualExposure);	

	//Streams the camera and process the height
	CameraStreaming();

	return TRUE;
}

//Unscaling the point to the actual image size
Point2f FaceDepth::unscalePoint(Point2f Pt, Size CurrentSize, Size TargetSize)
{
	Point2f DesPt;
	DesPt.x = (Pt.x / (float)CurrentSize.width)*TargetSize.width;
	DesPt.y = (Pt.y / (float)CurrentSize.height)*TargetSize.height;
 
	return DesPt;
}

//Streams the input from the camera
int FaceDepth::CameraStreaming()
{	
	vector<Rect> LFaces;
	float DepthValue;
	bool RImageDisplay = false;
	int BrightnessVal = 4;		//Default value
	Mat gDisparityMap, gDisparityMap_viz, RightImage;
	Point g_SelectedPoint;
	//user key input
	char WaitKeyStatus;

	//Window Creation
	namedWindow("Left Image", WINDOW_AUTOSIZE);
	
	cout << endl << "Press q/Q/Esc on the Image Window to quit the application!" <<endl;
	cout << endl << "Press r/R on the Image Window to see the right image" << endl;
	cout << endl << "Press e/E on the Image Window to change the exposure of the camera" << endl;
	cout << endl << "Press b/B on the Image Window to change the brightness of the camera" << endl << endl;

	//Estimates the depth of the face using Haarcascade file of OpenCV
	while(1)
	{
		_Disparity.GrabFrame(&LeftImage, &RightImage); //Grabs the frame and returns the rectified image		
	
		//Get disparity
		_Disparity.GetDisparity(LeftImage, RightImage, &gDisparityMap, &gDisparityMap_viz);

		//Detect the faces
		LFaces = DetectFace(LeftImage);
	
		//Estimate the Depth of the point selected
		for (size_t i = 0; i < LFaces.size(); i++)
		{
			//Pointing to the center of the face
			g_SelectedPoint = Point(LFaces[i].x + LFaces[i].width / 2, LFaces[i].y + LFaces[i].height / 2);

			Point scaledPoint = unscalePoint(g_SelectedPoint, LeftImage.size(),  gDisparityMap.size());

			_Disparity.EstimateDepth(scaledPoint, &DepthValue);
			
			if(g_SelectedPoint.x > RIGHTMATCH && DepthValue > 0) //Mark the point selected by the user
			{
				circle(LeftImage, g_SelectedPoint, 3, Scalar::all(0), 3, 8);
				circle(gDisparityMap_viz, scaledPoint, 3, Scalar::all(0), 3, 8);
			}

			if(DepthValue > 0)
			{
				stringstream ss;
				ss << DepthValue / 10 << " cm\0" ;
				DisplayText(LeftImage, ss.str(), g_SelectedPoint);
			}

		}
		
		//Display the Images
		imshow("Left Image",  LeftImage);

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
		else if(WaitKeyStatus == 'r' || WaitKeyStatus == 'R') //Show the right image
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

	//Closes all the windows
	destroyAllWindows();

	return TRUE;
}
 
//Detects the faces in the image passed
vector<Rect> FaceDepth::DetectFace(Mat InputImage)
{		
	vector<Rect> FacesDetected;

	//Detects the faces in the scene
	FaceCascade.detectMultiScale(InputImage, FacesDetected, 1.3, 5);

	//Marks the faces on the image
	for(size_t i = 0; i < FacesDetected.size(); i++)
	{
		rectangle(InputImage, FacesDetected[i], Scalar(255, 0, 0), 2);  
	}

	//Return the detected faces
	return FacesDetected;
}

//Main Function
int main()
{
	if(DEBUG_ENABLED)
		cout << "Face Depth Estimation - Application!\n";

	//Object creation
	FaceDepth _FaceDepth;
	_FaceDepth.Init();

	if(DEBUG_ENABLED)
		cout << "Exit: Face Depth Estimation - Application!\n";
	return TRUE;
}
