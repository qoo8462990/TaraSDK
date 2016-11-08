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
	IMU_Sample.h : 	Declares the class to be used in the application.
**********************************************************************/
#pragma once
#include "Tara.h"
#include <math.h>
#include <pthread.h>
#include <signal.h>

using namespace Tara;
using namespace cv;

#define		M_PI				3.14159265358979323846
#define		DEG2RAD				(float)(M_PI / 180.f)
#define		RAD2DEG				(float)(180.f / M_PI)

class IMU_Sample
{
public:
	//Constructor
	IMU_Sample(void);
	
	//Initialises the variables
	int Init();

	// Function declarations
	void getInclination(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z);

private:
		
	float angleX, angleY, angleZ; // Rotational angle for cube [NEW]
	float RwEst[3];
	float glIMU_Interval;
	
	float squared(float x);
	
	/* Drawing Points in circles for illustration*/
	Point PointOnCircle(float radius, float angleInDegrees, Point origin);
	
	/* Drawing angles in circles for illustration */
	void updateCircles();

	/*  Returns the interval time for sampling the values of the IMU. */
	float GetIMUIntervalTime(IMUCONFIG_TypeDef	lIMUConfig);
}IMU_SampleObj;

IMUDATAINPUT_TypeDef			    glIMUInput;
bool					    glIMUAbortThread;
pthread_mutex_t				    IMUDataReadyEvent;

//Pthread call routine
void*	GetIMUValueThread(void *lpParameter);

void*	UpdateIMUValueThread(void *lpParameter);

//Keyboard hit detection
int 	kbhit(void);


