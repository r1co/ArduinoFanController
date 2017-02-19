#ifndef FanController_h
#define FanController_h

#include "Arduino.h"
#include "Fan.h"

const int MAX_NUM_FANS = 10;
//const int startUpDelay = 500;
//const int minSpeed = 75;
//const int fullSpeed = 255;
//const int noRuleSpeed = 150;


enum DeviceMode {
  OFF,
  POTI,
  SENSOR_MB,
  SENSOR_SERIAL
};

class FanController
{
    int fullSpeed;
    int minSpeed;
    int startUpDelay;
    int noRuleSpeed;
    
    int powerPin;
    int powerStatus;

    int sensorValue;

    Fan fans[MAX_NUM_FANS];
    DeviceMode deviceMode;
    DeviceMode deviceModePreOff;


  public:
    FanController(int fullSpeed, int minSpeed, int startUpDelay, int noRuleSpeed, int powerPin);
    void setup();
    void addFan(Fan fan);
    void run();
    
    void checkPowerStatus();
    int sizeofFans();


};

#endif
