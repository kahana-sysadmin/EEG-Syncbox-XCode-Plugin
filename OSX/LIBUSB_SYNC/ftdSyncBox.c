//
//  ftdSyncBox.c
//  XCodePlugin
//
//  Created by Armin Brandt on 06/06/15, based on Corey Novich's u3Extended.c as of 4/29/15.
//
//

#include "ftdSyncBox.h"


// TODO: improve error handling


HANDLE openUSB(int ID){

    // the argument provided for <ID> is silently ignored; configurations with several ftd sync boxes are not supported

    libusb_device_handle *hDevice = NULL;

    libusb_init(NULL);
    hDevice = libusb_open_device_with_vid_pid(NULL, FTD_VENDOR_ID, FTD_PRODUCT_ID);
    if (!hDevice)  {
      fprintf(stderr, "Couldn't open ftd sync box.\n");
    }

    if (hDevice && (libusb_kernel_driver_active(hDevice, 0) == 1))  {
      if (libusb_detach_kernel_driver(hDevice, 0) != 0)  { 
        fprintf(stderr, "Couldn't unload the Kernel driver for the ftd sync box.\n");
        fprintf(stderr, "You may need to execute: sudo kextunload -b com.apple.driver.AppleUSBFTDI\n");
        hDevice = NULL;
      }
    }
    if (hDevice)  {
      if (libusb_claim_interface(hDevice, 0) < 0)  {
        fprintf(stderr, "The ftd sync box couldn't be claimed after opening.\n");
        hDevice = NULL;
      }
    }
    return (HANDLE) hDevice;
}

int returnStatus(HANDLE hDevice,int channel)
{
    
    unsigned char d = 0;
    int actual_bytes_transferred;
    int syncStatus;
    syncStatus=libusb_bulk_transfer((libusb_device_handle *)hDevice, (2 | LIBUSB_ENDPOINT_OUT), &d, sizeof(d), &actual_bytes_transferred, 0);
    return syncStatus;
}


void closeUSB(HANDLE hDevice){
  if (hDevice)  {
    libusb_release_interface((libusb_device_handle *)hDevice, 0);
    libusb_close((libusb_device_handle *)hDevice);
  }
}


void sendPulse(HANDLE hDevice, int channel){

    // the argument provided for <channel> is silently ignored for the time being
  if (hDevice)  {
    int error_code;
    unsigned char d = 0;
    int actual_bytes_transferred;
    error_code = libusb_bulk_transfer((libusb_device_handle *)hDevice, (2 | LIBUSB_ENDPOINT_OUT), &d, sizeof(d), &actual_bytes_transferred, 0);
    if (error_code != 0)  {
      fprintf(stderr, "Writing to the ftd device failed with error code %d.\n", error_code);
    }
  }
}


/***
int main (int argc, char** argv)  {
  HANDLE h = openUSB(0);
  fprintf(stderr, "h=%p\n", h);
  sendPulse(h, 0);
  closeUSB(h);    
  return 0;
}
***/
