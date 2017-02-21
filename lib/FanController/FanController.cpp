#include "Arduino.h"
#include "FanController.h"


FanController::FanController(int powerPin)
{
  this->powerPin = powerPin;
  this->deviceMode = OFF;
  this->deviceModePreOff = deviceMode;
}

// *****************************
// **** PUBLIC
// *****************************
void FanController::setup(DeviceMode deviceMode) {
  Serial.println("Setup FanController...");

  // set timer, avoid noise
  TCCR1B = ( TCCR1B & 0b11111000 ) | 0x01;
  TCCR2B = ( TCCR2B & 0b11111000 ) | 0x01;

  // setup fans
  for (int i = 0; i < sizeofFans(); i++) {
    fans[i].setup();
  }

  this->deviceMode = deviceMode;
  this->checkPowerStatus();
}


void FanController::addFan(Fan fan) {
  int i = sizeofFans();
  fans[i] = fan;
}


void FanController::run() {

  //  boolean powerOn = digitalRead(this->powerPin);
  this->checkPowerStatus();

  switch (deviceMode) {
    case OFF:
      Serial.print("Mode: OFF");

      for (int i = 0; i < sizeofFans(); i++) {
        fans[i].setSpeedPercentage(0);
      }
      break;

    case SERIAL_PERCENT:
      Serial.print("Mode: SERIAL_PERCENT");
      if (Serial.available() > 0) {
        // read the incoming byte:
        int newSensorValue = Serial.parseInt();

        if (newSensorValue > 0 ) {
          sensorValue = newSensorValue;
        } else if (newSensorValue < 0) {
          sensorValue = 0;
        }
      }

      for (int i = 0; i < sizeofFans(); i++) {
        fans[i].setSpeedPercentage(sensorValue);
      }

      break;

    default:
      Serial.print("Mode: default");
      for (int i = 0; i < sizeofFans(); i++) {
        fans[i].setSpeedPercentage(30);
      }
      break;
  }

  delay(1000);
}

// *****************************
// **** PRIVATE
// *****************************
int FanController::sizeofFans() {

  int rawSize = (sizeof(fans) / sizeof(Fan));
  int num = 0;

  for (int i = 0; i < rawSize; i++) {
    if (fans[i].isNull()) {
      num++;
    } else {
      break;
    }
  }

  return num;
}


void FanController::checkPowerStatus() {
  int newPowerStatus = digitalRead(this->powerPin);

  if (powerStatus != newPowerStatus) {
    if(newPowerStatus == 0){
      Serial.println("Power: OFF");
      deviceModePreOff = deviceMode;
      deviceMode = OFF;
    }else{
      Serial.println("Power: ON");
      delay(5000);
      deviceMode = deviceModePreOff;
    }

    powerStatus = newPowerStatus;
  }
}

// *****************************
// **** GETTER && SETTER
// *****************************
void FanController::setDeviceMode(DeviceMode mode){
  this->deviceMode = mode;
}
