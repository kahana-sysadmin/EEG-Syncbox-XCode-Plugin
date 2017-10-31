//
//  u3Extended.c
//  XCodePlugin
//
//  Created by Corey Novich on 4/29/15.
//
//

#include "u3Extended.h"

/*new functions*/

LJ_HANDLE hDevice;
bool isDeviceOpen = false;
float testFunct(){
	//hDevice = openUSB(-1);
    return 1;
}

//const char* OpenUSB() {
	/*int localID;
	//long error;

	//Open first found U3 over USB
	localID = -1;

	hDevice = openUSB(localID);
	if (hDevice != NULL) {
		isDeviceOpen = true;
		return "opened USB!";
	}
	else {
		isDeviceOpen = false;
		return "didn't open USB...";
	}*/
	//return "yay";
//}

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