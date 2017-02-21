#include "Arduino.h"
#include "FanController.h"


FanController::FanController(int powerPin)
{
  this->powerPin = powerPin;

  this->deviceMode = OFF;
  this->deviceModePreOff = deviceMode;
}

void FanController::checkPowerStatus() {
  int newPowerStatus = digitalRead(this->powerPin);

  if (powerStatus != newPowerStatus) {
    Serial.println("POWER CHANGE");
    Serial.println(newPowerStatus);

    if(newPowerStatus == 0){
      deviceModePreOff = deviceMode;
      deviceMode = OFF;
    }else{
      delay(5000);
      deviceMode = deviceModePreOff;
    }

    powerStatus = newPowerStatus;

  }



}


void FanController::setup(DeviceMode deviceMode) {
  Serial.println("Setup");

  // set timer, avoid noise
  TCCR1B = TCCR1B & 0b11111000 | 0x01;
  TCCR2B = TCCR2B & 0b11111000 | 0x01;

  for (int i = 0; i < sizeofFans(); i++) {
    fans[i].setup();
  }

  Serial.print("\n\n\n");

  this->deviceMode = deviceMode;

  this->checkPowerStatus();
}

void FanController::addFan(Fan fan) {
  int i = sizeofFans();
  fans[i] = fan;
}

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


void FanController::run() {

  //  boolean powerOn = digitalRead(this->powerPin);
  this->checkPowerStatus();

  Serial.println();
  Serial.print("MODE: ");
  Serial.print(deviceMode);
  //  Serial.print(" Power: ");
  //  Serial.print(powerOn);
  Serial.println();


  switch (deviceMode) {
    case OFF:
      for (int i = 0; i < sizeofFans(); i++) {
        fans[i].setSpeedPercentage(0);
      }
      break;

    case SERIAL_PERCENT:

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
      for (int i = 0; i < sizeofFans(); i++) {
        fans[i].setSpeedPercentage(30);
      }
      break;
  }

  delay(1000);
}

void FanController::setDeviceMode(DeviceMode mode){
  this->deviceMode = mode;
}
