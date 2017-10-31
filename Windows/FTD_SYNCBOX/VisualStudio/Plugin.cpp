
#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

/*
	This is a simple plugin, a bunch of functions that do simple things.
*/

#include "Plugin.pch"
#include "u3Extended.h"
#include <cstdlib> //for random()
//#include <unistd.h> //for usleep(microseconds)
//#include <pthread.h>
#include <exception>
#include <chrono>

#include "sys_time_extended.h"


using namespace std::chrono;

HANDLE hDevice;
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
	

	//extern "C" char* OpenUSB();
	extern "C" HANDLE openUSB(int ID);
	extern "C" int returnStatus(HANDLE hDevice, int channel);
	extern "C" void sendPulse(HANDLE hDevice, int channel);
	extern "C" void closeUSB(HANDLE hDevice);

	const char* OpenUSB()
	{
		int localID;
	//open first ftd syncbox over USB
		localID = -1;

		hDevice = openUSB(localID);
		if (hDevice != NULL)
		{
			isDeviceOpen = true;
			return "opened USB";
		}
		else
		{
			isDeviceOpen = false;
			return "didn't open USB!";
		}
	}

	const char* CloseUSB()
	{
		if (isDeviceOpen)
		{
			isDeviceOpen = false;
			closeUSB(hDevice);
			return "closed USB device";
		}
		else
		{
			return "didn't close USB!";
		}
	
	}

	int CheckUSB()
	{
		if (hDevice != NULL)
		{
			int status = returnStatus(hDevice, syncChannel);
			return status;
		}
		else
		{
			return -1;
		}

	}

	const char* TurnLEDOn()
	{
		if (isDeviceOpen)
		{
			sendPulse(hDevice, syncChannel);
			return "Light On!";
		}
		return "No device to turn on LED. ";
	}

	const char* TurnLEDOff()
	{
		if (isDeviceOpen)
		{
			return "light off!";
		}
		return "no device to turn off";
	}


}
