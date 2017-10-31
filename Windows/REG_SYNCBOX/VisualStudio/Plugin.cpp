
#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

/*
	This is a simple plugin, a bunch of functions that do simple things.
*/

//#include "Plugin.pch"
#include "u3Extended.h"
#include <cstdlib> //for random()
//#include <unistd.h> //for usleep(microseconds)
//#include <pthread.h>
#include <exception>
#include <chrono>

#include "sys_time_extended.h"


using namespace std::chrono;

LJ_HANDLE hDevice;
bool isDeviceOpen = false;
bool isLightOn = false;
int stimChannel = 4;
int syncChannel = 0;

//random seed must be set once during the duration of the program
//random is used in sync pulse for time jitter
bool isRandomSeedSet = false;
float syncTimeBeforePulseSeconds = 0.0f;
long long syncOnSystemTime = 0;

// ------------------------------------------------------------------------
// Plugin itself


// Link following functions C-style (required for plugins)
extern "C"
{

	// The functions we will call from Unity.
	//
	const EXPORT_API char*  PrintHello() {
		return "PRE U3";
	}

	int EXPORT_API PrintANumber() {
		return 5;
	}

	int EXPORT_API AddTwoIntegers(int a, int b) {
		return a + b;
	}

	float EXPORT_API AddTwoFloats(float a, float b) {
		return a + b;
	}

	//extern "C" char* OpenUSB();
	extern "C" LJ_HANDLE openUSB(int ID);
	extern "C" void testFunct();
	extern "C" void closeUSB(LJ_HANDLE hDevice);
	extern "C" void toggleHandleLEDOn(LJ_HANDLE hDevice, int channel);
	extern "C" void toggleHandleLEDOff(LJ_HANDLE hDevice, int channel);
	extern "C" void SetFIOState(LJ_HANDLE hDevice, int port, int state);
	extern "C" int GetFIOState(LJ_HANDLE hDevice, int port);
	extern "C" void configTC(LJ_HANDLE hDevice, int TimerCounterPinOffset, int NumberOfTimersEnabled, long *aEnableTimers, long *aEnableCounters, int tc_base, float divisor, long *aTimerModes, double *aTimerValues);


	//labjackusb
	extern "C" float LJUSB_GetLibraryVersion();
	const EXPORT_API float GetLJLibVersion() {
		return LJUSB_GetLibraryVersion();
	}


/*
	//u3.c
	//const EXPORT_API int OpenUSB(){
	const EXPORT_API char* OpenUSB() {
		int localID;
		//long error;

		//Open first found U3 over USB
		localID = -1;

		hDevice = openUSB(localID);
		if (hDevice != NULL) {
			isDeviceOpen = true;
			//return "opened USB!";
			return "open";
		}
		else {
			isDeviceOpen = false;
			//return "didn't open USB...";
			return "close";
		}
	}

	const EXPORT_API char* CloseUSB() {
		//closeUSBConnection(myDeviceHandle);
		if (isDeviceOpen == true) {
			isDeviceOpen = false;
			closeUSB(hDevice);
			return "closed USB!";
		}
		else {
			return "didn't close USB.";
		}
	}
	*/

	const EXPORT_API char* TurnLEDOn() {
		if (isDeviceOpen) {
			toggleHandleLEDOn(hDevice, syncChannel);
			return "Light On!";
		}
		return "No device to turn on LED.";


	}

	const EXPORT_API char* TurnLEDOff() {
		if (isDeviceOpen) {
			toggleHandleLEDOff(hDevice, syncChannel);
			return "Light Off!";
		}
		return "No device to turn off LED.";
	}
}

	/*

	//ex: a 10 ms pulse every second — until the duration is over...
	//should be called from SYNCPULSE() function on its own thread so that when it calls USLEEP, it will not sleep the entire application.
	DWORD WINAPI ExecuteSyncPulse(LPVOID lpParam) {
		//creates a random float between 0.8f and 1.2f
		//float minTimeBetweenPulses = 0.8f;
		//float maxTimeBetweenPulses = 1.2f;
		//float timeBetweenPulseSeconds = minTimeBetweenPulses + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxTimeBetweenPulses-minTimeBetweenPulses)));





		float pulseTimeSeconds = 0.02f; //TODO: make this a parameter???   --> 10MS
		int pulseTimeMilliseconds = (int)(pulseTimeSeconds * 1000);
		float maxTimeToWait = 1.0f - pulseTimeSeconds;


		//global var to keep track of this/be able to print it from SyncPulse()...
		syncTimeBeforePulseSeconds = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / maxTimeToWait); //creates a random float between 0.0f and maxTimeToWait

		unsigned int timeBeforePulseMilliseconds = (int)(syncTimeBeforePulseSeconds * 1000);

		//unsigned int microseconds = timeBeforePulseMilliseconds*1000;
		Sleep(timeBeforePulseMilliseconds); //sleep for the random time to wait before the pulse


											//turn on pulse
		if (isDeviceOpen) {
			//struct timeval2 tv;
			//gettimeofday2(&tv, NULL);

			//syncOnSystemTime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
			//milliseconds ms = duration_cast < milliseconds >(system_clock::now().time_since_epoch());
			
			//unsigned long milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
			//syncOnSystemTime = milliseconds_since_epoch;


			auto millitime = std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::system_clock::now().time_since_epoch()).count();
		
			syncOnSystemTime = millitime;



			toggleHandleLEDOn(hDevice, syncChannel);
		}

		//sleep for pulselength
		//microseconds = pulseTimeMilliseconds*1000;
		Sleep(pulseTimeMilliseconds); //sleep for the random time to wait before the pulse

									  //turn off pulse
		if (isDeviceOpen) {
			toggleHandleLEDOff(hDevice, syncChannel);
		}


		//wait for rest of second
		float remainderOfSecondMilliseconds = 1000 - pulseTimeMilliseconds - timeBeforePulseMilliseconds;
		Sleep(remainderOfSecondMilliseconds);

		//return timeBeforePulseSeconds;
		return 1;
	}

	//THREADED sync pulse. used because USLEEP is called in ExecuteSyncPulse. don't want the whole app to sleep.
	EXPORT_API long long SyncPulse() {

		//pthread_t t1 ; // declare thread
		//pthread_create( &t1, NULL, ExecuteSyncPulse,NULL); // create a thread running function1

		if (!isRandomSeedSet) {
			//set random seed
			srand(static_cast <unsigned> (time(0)));
			isRandomSeedSet = true;
		}

		unsigned int myCounter = 0;
		DWORD myThreadID;

		HANDLE myHandle = CreateThread(NULL, 0, ExecuteSyncPulse, &myCounter, 0, &myThreadID);

		//return syncTimeBeforePulseSeconds;
		return syncOnSystemTime;
	}

	//stim pulse: see mac version of this plugin code.

} // end of export C block
*/