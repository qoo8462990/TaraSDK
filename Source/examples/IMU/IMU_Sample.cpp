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
 IMU Sample: A sample app for displaying the IMU data.
**********************************************************************/
#include "IMU_Sample.h"

/*
  **********************************************************************************************************
 *  Name		:	GetIMUIntervalTime											   *
 *  Returns		:	Interval time in float										   *
 *  Description	:   Returns the interval time for sampling the values of the IMU.  * **********************************************************************************************************
*/
float IMU_Sample::GetIMUIntervalTime(IMUCONFIG_TypeDef	lIMUConfig)
{
	float lIMUIntervalTime = 10;
	if(lIMUConfig.IMU_MODE == IMU_ACC_GYRO_ENABLE)
	{
		switch(lIMUConfig.IMU_ODR_CONFIG)
		{
			case IMU_ODR_10_14_9HZ:
				lIMUIntervalTime = float(1000.00/14.90);
				break;

			case IMU_ODR_50_59_9HZ:
				lIMUIntervalTime = float(1000.00/59.50);
				break;

			case IMU_ODR_119HZ:
				lIMUIntervalTime = float(1000.00/119.00);
				break;

			case IMU_ODR_238HZ:
				lIMUIntervalTime = float(1000.00/238.00);
				break;

			case IMU_ODR_476HZ:
				lIMUIntervalTime = float(1000.00/476.00);
				break;

			case IMU_ODR_952HZ:
				lIMUIntervalTime = float(1000.00/952.00);
				break;
		}
	}
	else if(lIMUConfig.IMU_MODE == IMU_ACC_ENABLE)
	{
		switch(lIMUConfig.IMU_ODR_CONFIG)
		{
			case IMU_ODR_10_14_9HZ:
				lIMUIntervalTime = float(1000.00/10.00);
				break;

			case IMU_ODR_50_59_9HZ:
				lIMUIntervalTime = float(1000.00/50.00);
				break;

			case IMU_ODR_119HZ:
				lIMUIntervalTime = float(1000.00/119.00);
				break;

			case IMU_ODR_238HZ:
				lIMUIntervalTime = float(1000.00/238.00);
				break;

			case IMU_ODR_476HZ:
				lIMUIntervalTime = float(1000.00/476.00);
				break;

			case IMU_ODR_952HZ:
				lIMUIntervalTime = float(1000.00/952.00);
				break;
		}
	}
	return lIMUIntervalTime;
}

/* Initialises all the values */
IMU_Sample::IMU_Sample(void)
{
	glIMUAbortThread = FALSE;
	glIMU_Interval = 0.0f;

	angleX = 0.0f;
	angleY = 0.0f;
	angleZ = 0.0f;
}

/* Square of a number */
float IMU_Sample::squared(float x)
{
	return x * x;
}

/* Computes the angle of rotation with respect to the axes */
void IMU_Sample::getInclination(float g_x, float g_y, float g_z, float a_x, float a_y, float a_z)
{
	int w = 0;
	float tmpf = 0.0;
	int signRzGyro;
	static bool firstSample = true;
	float wGyro = 10.0;
	float norm;

	// Normalise the accelerometer measurement
	norm = sqrt(a_x * a_x + a_y * a_y + a_z * a_z);
	a_x /= norm;
	a_y /= norm;
	a_z /= norm;

	float RwAcc[3] = {a_x, a_y, a_z};
	float RwGyro[3] = { g_x, g_y, g_z };
	float Awz[2];
	float Gyro[3];


	if (firstSample)
	{ 
		//initialize with accelerometer readings
		for (w = 0; w <= 2; w++) 
		{
			RwEst[w] = RwAcc[w];    
		}
	}

	else 
	{
		//evaluate Gyro vector
		if (fabs(RwEst[2]) < 0.1) 
		{
			//Rz is too small and because it is used as reference for computing Axz, Ayz it's error fluctuations will amplify leading to bad results
			//in this case skip the gyro data and just use previous estimate
			for (w = 0;w <= 2;w++) 
			{
				Gyro[w] = RwEst[w];
			}
		}
		else {
			//get angles between projection of R on ZX/ZY plane and Z axis, based on last RwEst
			for (w = 0;w <= 1;w++) 
			{
				tmpf = RwGyro[w];                        //get current gyro rate in deg/s
				tmpf *= glIMU_Interval / 1000.0f;                     //get angle change in deg
				Awz[w] = atan2(RwEst[w], RwEst[2]) * 180 / float(M_PI);   //get angle and convert to degrees 
				Awz[w] += tmpf;             //get updated angle according to gyro movement
			}

			//estimate sign of RzGyro by looking in what qudrant the angle Axz is, 
			//RzGyro is pozitive if  Axz in range -90 ..90 => cos(Awz) >= 0
			signRzGyro = (cos(Awz[0] * float(M_PI) / 180) >= 0) ? 1 : -1;

			//reverse calculation of Gyro from Awz angles, for formulas deductions see  http://starlino.com/imu_guide.html
			for (w = 0; w <= 1; w++) 
			{
				Gyro[0] = sin(Awz[0] * float(M_PI) / 180);
				Gyro[0] /= sqrt(1 + squared(cos(Awz[0] * float(M_PI) / 180)) * squared(tan(Awz[1] * float(M_PI) / 180)));
				Gyro[1] = sin(Awz[1] * float(M_PI) / 180);
				Gyro[1] /= sqrt(1 + squared(cos(Awz[1] * float(M_PI) / 180)) * squared(tan(Awz[0] * float(M_PI) / 180)));
			}
			Gyro[2] = signRzGyro * sqrt(1 - squared(Gyro[0]) - squared(Gyro[1]));
		}

		//combine Accelerometer and gyro readings
		for (w = 0; w <= 2; w++) 
			RwEst[w] = (RwAcc[w] + wGyro * Gyro[w]) / (1 + wGyro);

		//Normalizing the estimates
		norm = sqrt(RwEst[0] * RwEst[0] + RwEst[1] * RwEst[1] + RwEst[2] * RwEst[2]);
		RwEst[0] /= norm;
		RwEst[1] /= norm;
		RwEst[2] /= norm;
	}

	firstSample = false;

	//Computing the angles
	angleX = RwEst[0] * float(M_PI) / 2 * RAD2DEG;
	angleY = RwEst[1] * float(M_PI) / 2 * RAD2DEG;
	angleZ = RwEst[2] * float(M_PI) / 2 * RAD2DEG;

	//printf("%0.2f %0.2f %0.2f , %0.2f %0.2f %0.2f , %0.2f %0.2f %0.2f \n", g_x, g_y, g_z, a_x, a_y, a_z, angleX, angleY, angleZ);
}

/* Drawing Points in circles for illustration */
Point IMU_Sample::PointOnCircle(float radius, float angleInDegrees, Point origin)
{
	//radius -> Radius of Circle & Origin -> Circle Centre.
	// Convert from degrees to radians via multiplication by PI/180        
	float x = (float)(radius * cos(angleInDegrees * float(M_PI) / 180.0)) + origin.x;
	float y = (float)(radius * sin(angleInDegrees * float(M_PI) / 180.0)) + origin.y;

	return Point((int)x, (int)y);
}

/* Drawing angles in circles for illustration */
void IMU_Sample::updateCircles()
{
	Mat drawImage = Mat::zeros(Size(800, 400), CV_8UC3);

	//Static Labelling.
	putText(drawImage, "x", Point(150, 50), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0, 0, 255), 1);
	putText(drawImage, "y", Point(400, 50), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0, 255, 0), 1);
	putText(drawImage, "z", Point(650, 50), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 0, 0), 1);


	//Labelling the angles.
	stringstream ss;
	ss << (int)angleX << " deg";
	putText(drawImage, ss.str(), Point(150, 350), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
	ss.str("");

	ss << (int)angleY << " deg";
	putText(drawImage, ss.str(), Point(400, 350), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
	ss.str("");

	ss << (int)angleZ << " deg";
	putText(drawImage, ss.str(), Point(650, 350), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 0), 1);


	//Drawing x angles.
	Point P1 = PointOnCircle(100, -angleX + 180, Point(150, 200));
	Point P2 = PointOnCircle(100, -angleX, Point(150, 200));

	ellipse(drawImage, Point(150, 200), Size(100, 100), 0, -angleX, -angleX - 180, Scalar(20, 20, 20), -1);
	ellipse(drawImage, Point(150, 200), Size(100, 100), 0, -angleX, -angleX + 180, Scalar(0, 0, 255), -1);
	circle(drawImage, P1, 2, Scalar(0, 0, 255), 2);
	circle(drawImage, P2, 2, Scalar(0, 0, 255), 2);
	line(drawImage, P1, P2, Scalar(0, 0, 255), 2);


	//Drawing y angles.
	P1 = PointOnCircle(100, -angleY + 180, Point(400, 200));
	P2 = PointOnCircle(100, -angleY, Point(400, 200));

	ellipse(drawImage, Point(400, 200), Size(100, 100), 0, -angleY, -angleY - 180, Scalar(20, 20, 20), -1);
	ellipse(drawImage, Point(400, 200), Size(100, 100), 0, -angleY, -angleY + 180, Scalar(0, 255, 0), -1);
	circle(drawImage, P1, 2, Scalar(0, 255, 0), 2);
	circle(drawImage, P2, 2, Scalar(0, 255, 0), 2);
	line(drawImage, P1, P2, Scalar(0, 255, 0), 2);


	//Drawing z angles.
	P1 = PointOnCircle(100, -angleZ + 180, Point(650, 200));
	P2 = PointOnCircle(100, -angleZ, Point(650, 200));

	ellipse(drawImage, Point(650, 200), Size(100, 100), 0, -angleZ, -angleZ - 180, Scalar(20, 20, 20), -1);
	ellipse(drawImage, Point(650, 200), Size(100, 100), 0, -angleZ, -angleZ + 180, Scalar(255, 0, 0), -1);
	circle(drawImage, P1, 2, Scalar(255, 0, 0), 2);
	circle(drawImage, P2, 2, Scalar(255, 0, 0), 2);
	line(drawImage, P1, P2, Scalar(255, 0, 0), 2);

	imshow("Inclination", drawImage);
	waitKey(1);
}

/* Auxiliary Functions */
/* To detect the enter key press event */
int kbhit(void)
{
	int i;
	fd_set fds;
	struct timeval tv;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    tv.tv_sec = tv.tv_usec = 0;
    i = select(1, &fds, NULL, NULL, &tv);
    if (i == -1) return(0);
    if (FD_ISSET(STDIN_FILENO, &fds)) return(1);
    return(0); 
}

/* To set idle for specified time */
void Sleep(int TimeInMilli)
{
	BOOL timeout = TRUE;
	unsigned int start, end = 0;

	start = GetTickCount();
	while(timeout)
	{
		end = GetTickCount();
		if(end - start > TimeInMilli)
		{
			timeout = FALSE;
		}
	}
	return;
}

/* Killing the thread */
void KillThread(int sig)
{
	pthread_exit(0);
}

/* GetIMUValue Thread calls the HID command */
void* GetIMUValueThread(void *lpParameter) 
{
	IMUDATAOUTPUT_TypeDef *lIMUOutput = (IMUDATAOUTPUT_TypeDef*)(lpParameter);
	signal(SIGUSR1, KillThread);
	
	//HID command
	if(GetIMUValueBuffer(&IMUDataReadyEvent , lIMUOutput))
	{
		cout << "GetIMUValueBuffer success\n";	
	}
	else
	{
		cout << "GetIMUValueBuffer Ended\n";
	}

}

/* UpdateIMUValue Thread uses the values from the GetIMUValue everytime when the semaphore is set */
void* UpdateIMUValueThread(void *lpParameter) 
{
	IMUDATAOUTPUT_TypeDef *lIMUOutputAdd, *lIMUOutput = NULL;
	lIMUOutputAdd = lIMUOutput = (IMUDATAOUTPUT_TypeDef*)(lpParameter);
	signal(SIGUSR1, KillThread);

	//Blocking call waits for unlock event
	for( ;((glIMUAbortThread == FALSE)  && ((glIMUInput.IMU_UPDATE_MODE == IMU_CONT_UPDT_EN) || 
		(lIMUOutput->IMU_VALUE_ID <= glIMUInput.IMU_NUM_OF_VALUES))) ; )
	{
		if(glIMUInput.IMU_UPDATE_MODE != IMU_CONT_UPDT_DIS)
		{
			
			pthread_mutex_lock(&IMUDataReadyEvent);
		}
				
		//Calculating angles based on the current raw values from IMU			
		IMU_SampleObj.getInclination(lIMUOutput->gyroX, lIMUOutput->gyroY, lIMUOutput->gyroZ,
				lIMUOutput->accX, lIMUOutput->accY, lIMUOutput->accZ);
		{
			//Round robin mechanism to use the same buffer
			if(lIMUOutput->IMU_VALUE_ID < IMU_AXES_VALUES_MAX) 
				lIMUOutput++;
			else
				lIMUOutput = lIMUOutputAdd;
		}		
	}
	return NULL;
}

/* Init the camera and read the values */
int IMU_Sample::Init()
{	 
	cout << endl  << "		IMU Sample Application " << endl  << endl;
	cout << " Application to illustrate the IMU unit LSM6DS0 integrated with Tara Camera" << endl;
	cout << " Demonstrating the rotations of camera around x-axis and y-axis " << endl;
	cout << " IMU values are limited from -90 to +90 degrees for illustration " << endl << endl;

	IMUDATAINPUT_TypeDef lIMUInput;
	IMUCONFIG_TypeDef lIMUConfig;
	UINT8 uStatus = 0;
	
	//Disparity Object to initilaise the camera and HID to get access to the IMU data
	Disparity _Disparity;

	//Initialise the camera
	if(!_Disparity.InitCamera(false, false))
	{
		cout << "Camera Initialisation Failed\n";
		return FALSE;
	}

	//Configuring IMU rates
	lIMUConfig.IMU_MODE = IMU_ACC_GYRO_ENABLE;
	lIMUConfig.ACC_AXIS_CONFIG = IMU_ACC_X_Y_Z_ENABLE;
	lIMUConfig.IMU_ODR_CONFIG = IMU_ODR_119HZ;
	lIMUConfig.ACC_SENSITIVITY_CONFIG = IMU_ACC_SENS_2G;
	lIMUConfig.GYRO_AXIS_CONFIG = IMU_GYRO_X_Y_Z_ENABLE;
	lIMUConfig.GYRO_SENSITIVITY_CONFIG = IMU_GYRO_SENS_245DPS;

	//Setting the configuration using HID command
	uStatus = SetIMUConfig(lIMUConfig);
	if(!uStatus)			
	{
		cout << "SetIMUConfig Failed\n";
		return -1;
	}
	else
	{
		//Reading the configuration to verify the values are set 
		uStatus = GetIMUConfig(&lIMUConfig);
		if(!uStatus)			
		{
			cout << "GetIMUConfig Failed\n";
			return -1;
		}
		//Finding the sampling interval time
		glIMU_Interval = GetIMUIntervalTime(lIMUConfig);
	}

	//Configuring IMU update mode
	lIMUInput.IMU_UPDATE_MODE = IMU_CONT_UPDT_EN;
	lIMUInput.IMU_NUM_OF_VALUES = IMU_AXES_VALUES_MIN;

	//Setting the IMU update mode using HID command
	uStatus = ControlIMUCapture(&lIMUInput);
	if(uStatus == FALSE)			
	{
		cout << "ControlIMUCapture Failed\n";
		return -1;
	}
	else
	{
		glIMUInput = lIMUInput;
	}

	//Getting the IMU values
	cout << "\nGetting IMU Value buffer\n";
	IMUDATAOUTPUT_TypeDef *lIMUOutput = NULL;
	pthread_t thread1, thread2;
									
	//Allocating buffers for output structure			
	if(glIMUInput.IMU_UPDATE_MODE == IMU_CONT_UPDT_EN)
	{
		lIMUOutput = (IMUDATAOUTPUT_TypeDef*)malloc(IMU_AXES_VALUES_MAX * sizeof(IMUDATAOUTPUT_TypeDef));
	}
	else 
	{
		lIMUOutput = (IMUDATAOUTPUT_TypeDef*)malloc(1 * sizeof(IMUDATAOUTPUT_TypeDef));
	}

	//Memory validation
	if(lIMUOutput == NULL)
	{
		cout << "Memory Allocation for otput failed\n";
		return -1;
	}

	lIMUOutput->IMU_VALUE_ID = 0;
	cout << "\nHit Enter key to stop\n";

	//Mutex initialization
	pthread_mutex_init(&IMUDataReadyEvent, NULL);
	pthread_mutex_lock(&IMUDataReadyEvent);

	//Thread creation
	if(pthread_create(&thread1, NULL, GetIMUValueThread, (void*) lIMUOutput) != 0)
	{
		cout << "Get IMU value thread creation failed\n";	
		return -1;
	}
	if(pthread_create(&thread2, NULL, UpdateIMUValueThread, (void*) lIMUOutput) != 0)
	{
		cout << "Print IMU value thread creation failed\n";	
		return -1;
	}
	
	//Calling function	
	for(;(!kbhit() && (glIMUInput.IMU_UPDATE_MODE != IMU_CONT_UPDT_DIS));)
	{						
		updateCircles();
		Sleep(1);
	}
	
	cout << "Keyboard hitted\n";
	glIMUAbortThread = TRUE;
	lIMUInput.IMU_UPDATE_MODE = IMU_CONT_UPDT_DIS;
	lIMUInput.IMU_NUM_OF_VALUES = IMU_AXES_VALUES_MIN;
			
	//Resetting the IMU update to disable mode
	uStatus = ControlIMUCapture(&lIMUInput);
	if(uStatus == FALSE)			
	{
		cout << "ControlIMUCapture Failed\n";
		return -1;
	}

	//Releasing the threads and mutex
	pthread_kill(thread1, SIGUSR1);
	pthread_kill(thread2, SIGUSR1);
	pthread_mutex_destroy(&IMUDataReadyEvent);
	glIMUAbortThread = FALSE;

	//Freeing the memory
	free(lIMUOutput);
	DeinitExtensionUnit();

	return TRUE;
}

/* Main Function */
int main()
{
	if(DEBUG_ENABLED)
		cout << "IMU Sample application\n";
	//Initialise the methods
	IMU_SampleObj.Init();

	if(DEBUG_ENABLED)
		cout << "Exit : IMU Sample application\n";
	return TRUE;
}

