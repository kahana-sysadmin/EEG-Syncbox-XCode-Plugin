/*
	This is a simple plugin, a bunch of functions that do simple things.
*/

#include "FtdPlugin.pch"
#include "ftdSyncBox.h"
#include <cstdlib> //for random()
#include <unistd.h> //for usleep(microseconds)
#include <pthread.h>

#include <sys/time.h>

//ftdSyncBox.c
extern "C" HANDLE openUSB(int ID);
extern "C" void closeUSB(HANDLE hDevice);
extern "C" void sendPulse(HANDLE hDevice, int channel);
extern "C" int returnStatus(HANDLE hDevice,int channel);


HANDLE hDevice;
bool isDeviceOpen = false;
bool isLightOn = false;
int syncChannel = 0;


//Test functions
const char* PrintHello(){
	return "Hello";
}

int PrintANumber(){
	return 8;
}

int AddTwoIntegers(int a, int b) {
	return a + b;
}

float AddTwoFloats(float a, float b) {
    return a+b;
}



const char* OpenUSB(){
    int localID;
    //long error;
    
    //Open first ftd sync box over USB
    localID = -1;
    
    hDevice = openUSB(localID);
    if(hDevice != NULL){
        isDeviceOpen = true;
        return "opened USB!";
    }
    else{
        isDeviceOpen = false;
        return "didn't open USB...";
    }

}

int CheckUSB()
{
    if(hDevice != NULL){
        
    int status=returnStatus(hDevice,syncChannel);
    return status;
    }
    else
    {
        return -1;
    }
    //    if(status==0)
    //    {
    //        return "connected";
    //    }
    //    else{
    //        return "returned else";
    //    }
}

const char* CloseUSB(){
    //closeUSBConnection(myDeviceHandle);
    if(isDeviceOpen == true){
        isDeviceOpen = false;
        closeUSB(hDevice);
        return "closed USB!";
    }
    else{
        return "didn't close USB.";
    }
}

const char* TurnLEDOn(){
    if(isDeviceOpen){
        sendPulse(hDevice, syncChannel);
        // toggleHandleLEDOn(hDevice, syncChannel);
        return "Light On!";
    }
    return "No device to turn on LED.";
    
    
}

const char* TurnLEDOff(){
    if(isDeviceOpen){
        // NO-OP: sync pulse stops automatically after 10 msec with the ftd sync box
        // toggleHandleLEDOff(hDevice, syncChannel);
        return "Light Off!";
    }
    return "No device to turn off LED.";
}

/***
#include <iostream>
int main(int argc, char** argv)  {
  std::cerr << OpenUSB() << std::endl;
  std::cerr << TurnLEDOn() << std::endl;
  std::cerr << TurnLEDOff() << std::endl;
  std::cerr << CloseUSB() << std::endl;    
  return 0;
}
***/
