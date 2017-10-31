//
//  u3Extended.h
//  XCodePlugin
//
//  Created by Corey Novich on 4/29/15.
//
//

#ifndef __XCodePlugin__u3Extended__
#define __XCodePlugin__u3Extended__

#include <stdio.h>
#include "u3.h"
#include "libusb.h"

#define FTD_VENDOR_ID 0x0403
#define FTD_PRODUCT_ID 0x6001


/*new functionality for Unity plugin -- by corey*/
#ifdef __cplusplus
extern "C"{
#endif
	//const char* OpenUSB();
	HANDLE openUSB(int ID);
    void closeUSB(HANDLE hDevice);
	void sendPulse(HANDLE hDevice, int channel);
	int returnStatus(HANDLE h, int channel);
	/*
    void toggleHandleLEDOn(HANDLE hDevice, int channel);
    void toggleHandleLEDOff(HANDLE hDevice, int channel);
    void SetFIOState(HANDLE hDevice, int port, int state);
    int GetFIOState(HANDLE hDevice, int port);
    void configIO(HANDLE hDevice, int TimerCounterPinOffset, bool EnableCounter1, bool EnableCounter0, int NumberOfTimersEnabled, int FIOAnalog, int EIOAnalog, bool EnableUART);
    void configTC(HANDLE hDevice, int TimerCounterPinOffset, int NumberOfTimersEnabled, long *aEnableTimers, long *aEnableCounters, int tc_base , float divisor, long *aTimerModes, double *aTimerValues);
    */
#ifdef __cplusplus
}
#endif




#endif /* defined(__XCodePlugin__u3Extended__) */
