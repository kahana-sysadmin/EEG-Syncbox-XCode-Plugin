#include "ThreadedSyncFunctions.h"

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
	float syncTimeBeforePulseSeconds = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / maxTimeToWait); //creates a random float between 0.0f and maxTimeToWait

	unsigned int timeBeforePulseMilliseconds = (int)(syncTimeBeforePulseSeconds * 1000);

	//unsigned int microseconds = timeBeforePulseMilliseconds*1000;
	Sleep(timeBeforePulseMilliseconds); //sleep for the random time to wait before the pulse


										//turn on pulse
	//if (isDeviceOpen) {
		struct timeval2 tv;
		gettimeofday2(&tv, NULL);

		syncOnSystemTime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
		toggleHandleLEDOn(hDevice, syncChannel);
	//}

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