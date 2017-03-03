#ifndef FanController_h
#define FanController_h

#include "Arduino.h"
#include "Fan.h"
#include "DeviceCommand.h"

const int MAX_NUM_FANS = 10;

enum DeviceMode {
  OFF,            // fans off
  SERIAL_CTRL // fans run on percentage given via serial
  // AUTO            // fans controlled by sensor
};


class FanController
{

  private:
    int powerPin;
    int powerStatus = -1;
    int sensorValue;

    String serialInput;
    String serialReaderInput;

    Fan fans[MAX_NUM_FANS];
    DeviceMode deviceMode;
    DeviceMode deviceModePreOff;

    // *****************************
    // **** PRIVATE
    // *****************************
    int sizeofFans();
    void checkPowerStatus();

    DeviceCommand readDeviceCommand();
    void  readSerialInput();



  public:
    FanController(int powerPin);

    // *****************************
    // **** PUBLIC
    // *****************************
    void setup(DeviceMode mode);
    void addFan(Fan fan);
    void printInfo();

    void run();

    // *****************************
    // **** GETTER && SETTER
    // *****************************
    void setDeviceMode(DeviceMode mode);

};

#endif
