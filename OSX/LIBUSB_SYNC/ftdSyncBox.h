//
//  u3Extended.h
//  XCodePlugin
//
//  Created by Corey Novich on 4/29/15.
//
//

#ifndef __XCodePlugin__ftdSyncBox__
#define __XCodePlugin__ftdSyncBox__

#include <stdio.h>
#include "libusb.h"

typedef void * HANDLE;

#define FTD_VENDOR_ID    0x0403
#define FTD_PRODUCT_ID   0x6001

#ifdef __cplusplus
extern "C"{
#endif


    HANDLE openUSB(int ID);
    void closeUSB(HANDLE hDevice);
    void sendPulse(HANDLE hDevice, int channel);

    
    HANDLE openUSB(int ID);
    void closeUSB(HANDLE h);
    int returnStatus(HANDLE h,int channel);
    void sendPulse(HANDLE h, int channel);

    
#ifdef __cplusplus
}
#endif




#endif /* defined(__XCodePlugin__ftdSyncBox__) */
