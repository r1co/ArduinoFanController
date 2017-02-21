#ifndef FanController_h
#define FanController_h

#include "Arduino.h"
#include "Fan.h"

const int MAX_NUM_FANS = 10;

enum DeviceMode {
  OFF,            // fans off
  SERIAL_PERCENT, // fans run on percentage given via serial
  AUTO            // fans controlled by sensor
};

class FanController
{

  private:
    int powerPin;
    int powerStatus;
    int sensorValue;

    Fan fans[MAX_NUM_FANS];
    DeviceMode deviceMode;
    DeviceMode deviceModePreOff;

    // *****************************
    // **** PRIVATE
    // *****************************
    int sizeofFans();
    void checkPowerStatus();

  public:
    FanController(int powerPin);

    // *****************************
    // **** PUBLIC
    // *****************************
    void setup(DeviceMode mode);
    void addFan(Fan fan);
    void run();

    // *****************************
    // **** GETTER && SETTER
    // *****************************
    void setDeviceMode(DeviceMode mode);

};

#endif
