#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif
#include <stdio.h>
#include "libusb.h"

#define FTD_VENDOR_ID 0x0403
#define FTD_PRODUCT_ID 0x6001

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



// Link following functions C-style (required for plugins)
extern "C"
{

	// The functions we will call from Unity.
	//
	const EXPORT_API char*  PrintHello() {
		return "PRE U3";
	}

	int EXPORT_API PrintANumber() {
		return 6977;
	}

	int EXPORT_API AddTwoIntegers(int a, int b) {
		return a + b;
	}

	float EXPORT_API AddTwoFloats(float a, float b) {
		return a + b;
	}
	HANDLE openUSB(int ID) {
		libusb_device_handle *hDevice = NULL;

		libusb_init(NULL);
		hDevice = libusb_open_device_with_vid_pid(NULL, FTD_VENDOR_ID, FTD_PRODUCT_ID);
		if (!hDevice)
		{
			fprintf(stderr, "Couldn't open ftd syncbox. \n");
		}

		if (hDevice && (libusb_kernel_driver_active(hDevice, 0) == 1))
		{
			if (libusb_detach_kernel_driver(hDevice, 0) != 0)
			{
				fprintf(stderr, "Couldn't unload the kernel driver for ftd syncbox");
				hDevice = NULL;
			}
		}

		if (hDevice)
		{
			if (libusb_claim_interface(hDevice, 0) < 0)
			{
				fprintf(stderr, "The FTD syncbox couldn't be claimed after opening. \n");
				hDevice = NULL;
			}
		}

		return (HANDLE)hDevice;
	}

	int returnStatus(HANDLE hDevice, int channel)
	{
		unsigned char d = 0;
		int actual_bytes_transferred;
		int syncStatus;
		syncStatus = libusb_bulk_transfer((libusb_device_handle *)hDevice, (2 | LIBUSB_ENDPOINT_OUT), &d, sizeof(d), &actual_bytes_transferred, 0);
		return syncStatus;
	}


	void closeUSB(HANDLE hDevice) {
		if (hDevice)
		{
			libusb_release_interface((libusb_device_handle *)hDevice, 0);
			libusb_close((libusb_device_handle *)hDevice);
		}

	}

	void sendPulse(HANDLE hDevice, int channel)
	{
		if (hDevice)
		{
			int error_code;
			unsigned char d = 0;
			int actual_bytes_transferred;

			error_code = libusb_bulk_transfer((libusb_device_handle *)hDevice, (2 | LIBUSB_ENDPOINT_OUT), &d, sizeof(d), &actual_bytes_transferred, 0);
			if (error_code != 0)
			{
				fprintf(stderr, "Writing to the ftd device failed with error code %d . \n", error_code);
			}
		}
	}


	
	EXPORT_API int OpenUSB()
	{
		int localID;
		//open first ftd syncbox over USB
		localID = -1;

		hDevice = openUSB(localID);
		if (hDevice != NULL)
		{
			isDeviceOpen = true;
			return 1;
		}
		else
		{
			isDeviceOpen = false;
			return 0;
		}
	}
	
	EXPORT_API int CloseUSB()
	{
		if (isDeviceOpen)
		{
			isDeviceOpen = false;
			closeUSB(hDevice);
			return 1;
		}
		else
		{
			return 0;
		}

	}
	
	int EXPORT_API CheckUSB()
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
	
	const EXPORT_API char* TurnLEDOn()
	{
		if (isDeviceOpen)
		{
			sendPulse(hDevice, syncChannel);
			return "Light On!";
		}
		return "No device to turn on LED. ";
	}
	
	
	const EXPORT_API char* TurnLEDOff()
	{
		if (isDeviceOpen)
		{
			return "light off!";
		}
		return "no device to turn off";
	}

}
