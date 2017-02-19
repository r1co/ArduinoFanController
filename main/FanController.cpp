#include "Arduino.h"
#include "FanController.h"


FanController::FanController(int _fullSpeed, int _minSpeed, int _startUpDelay, int _noRuleSpeed, int powerPin)
{
  fullSpeed = _fullSpeed;
  minSpeed = _minSpeed;
  startUpDelay = _startUpDelay;
  noRuleSpeed = _noRuleSpeed;
  this->powerPin = powerPin;

  deviceMode = SENSOR_SERIAL;
  deviceModePreOff = deviceMode;
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


void FanController::setup() {
  Serial.println("Setup");

  // set timer, avoid noise
  TCCR1B = TCCR1B & 0b11111000 | 0x01;
  TCCR2B = TCCR2B & 0b11111000 | 0x01;

  for (int i = 0; i < sizeofFans(); i++) {
    fans[i].setup(fullSpeed, minSpeed, startUpDelay);
  }

  Serial.print("\n\n\n");
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
    Fan f = fans[i];
    if (f.isFan()) {
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
        fans[i].runAutoMode(0, 0);
      }
      break;
      
    case SENSOR_SERIAL:

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
        fans[i].runAutoMode(sensorValue, noRuleSpeed);
      }

      break;

    default:
      for (int i = 0; i < sizeofFans(); i++) {
        fans[i].runAutoMode(30, noRuleSpeed);
      }
      break;
  }

  delay(1000);
}

