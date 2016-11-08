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

class HeightCalibration
{
public:

	//Constructor
	HeightCalibration(void);

	//Initalises all the methods
	int Init();

private:

	int ManualExposure;

	Mat gDisparityMap, gDisparityMap_viz;
	
	bool DepthPointSelection;
	
	//int to text conversion
	stringstream ss;
		
	//Streams the input from the camera 
	int CameraStreaming();

	//User gets to select the point for which the depth has to be determined
	Point SelectDepthPoint();

	//writes the Dpeth Value to a file
	int DepthFileGeneration(float Depth);

	//Disparity Object
	Disparity _Disparity;
};
