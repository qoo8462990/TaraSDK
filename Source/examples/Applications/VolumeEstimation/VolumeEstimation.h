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
#pragma once
#include "Tara.h"

using namespace cv;
using namespace Tara;

class VolumeEstimation
{
public:
	//Constructor
	VolumeEstimation();
		
	//Initialises the methods
	int Init();

private:

	int ManualExposure;

	//Images
	Mat LeftImage, RightImage;
	Mat gDisparityMap, gDisparityMap_viz;

	//Depth of the base
	float BaseDepth;

	//Streams the input from the camera 
	int CameraStreaming();

	//Detects the edge points in the scene
	vector<Point> EdgeDetection(Mat Image);

	//Processes the input images and displays the results
	int ProcessImages(Mat LImage, Mat RImage);

	//Sort the points passed and returns the valid corners
	vector<Point> Sorting(vector<Point> corners);	
	
	//Projects the 3D Points and calculates the volume
	int DisplayVolume(vector<Point> LP, vector<Point> RP);

	//Estimates the depth of the base
	int BaseDepthEstimation();	

	Disparity _Disparity;
};
