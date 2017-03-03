#include "Arduino.h"
#include "FanController.h"


FanController::FanController(int powerPin)
{
  this->powerPin = powerPin;
  this->deviceMode = OFF;
  this->deviceModePreOff = deviceMode;
  this->sensorValue = 50;
  this->serialInput = "";
}

// *****************************
// **** PUBLIC
// *****************************
void FanController::setup(DeviceMode deviceMode) {
  Serial.println("Setup FanController...");

  // set timer, avoid noise
  TCCR1B = ( TCCR1B & 0b11111000 ) | 0x01;
  TCCR2B = ( TCCR2B & 0b11111000 ) | 0x01;

  pinMode(this->powerPin, INPUT);      // sets the digital pin as output


  // setup fans
  for (int i = 0; i < sizeofFans(); i++) {
    fans[i].setup();
  }

  this->deviceMode = deviceMode;
  this->deviceModePreOff = deviceMode;

  this->checkPowerStatus();
  Serial.println("started...");
}


void FanController::addFan(Fan fan) {
  int i = sizeofFans();
  fans[i] = fan;
}


void FanController::readSerialInput(){
  if (Serial.available() > 0) {
    String serialInput = Serial.readString();

    // if(serialInput.charAt(serialInput.length()-1) == "#"){
    //   Serial.println("LAST #");
    // }

    if(serialInput.charAt(serialInput.length()-1) == '\n'){
      this->serialReaderInput = this->serialReaderInput + serialInput;
      this->serialReaderInput.remove(this->serialReaderInput.length()-1);
      this->serialInput = serialReaderInput;
      this->serialReaderInput = "";

    }else{
      this->serialReaderInput = this->serialReaderInput + serialInput;
    }
    // Serial.print("X: ");
    // Serial.print(this->getStringPart(serialInput, ":", 0));
    // Serial.println();
  }
}

DeviceCommand FanController::readDeviceCommand(){
  DeviceCommand devCmd = DeviceCommand();

  if(this->serialInput.length() > 0){
    devCmd = (this->serialInput);
    this->serialInput = "";
  }

  return devCmd;
}

void FanController::run() {

  this->readSerialInput();
  this->checkPowerStatus();
  DeviceCommand cmd = this->readDeviceCommand();

  if(cmd.isVaild()){

    if(cmd.getCommand() == "status"){
        this->printInfo();
        for (int i = 0; i < sizeofFans(); i++) {
          fans[i].printInfo();
        }
    }else if(cmd.getCommand() == "set"){
        this->sensorValue = cmd.getPayload().toInt();
        Serial.println("Controller set[sensorValue: "+cmd.getPayload()+"]");
        for (int i = 0; i < sizeofFans(); i++) {
          fans[i].setSpeedPercentage(this->sensorValue);
        }
    }else if(cmd.getCommand() == "setFan"){
        this->sensorValue = cmd.getPayload().toInt();
        Serial.println("Controller setFan[sensorValue: "+cmd.getPayload()+"]");
        // Serial.println(cmd.getPayload(0));
        // Serial.println(cmd.getPayload(1));

        int p = cmd.getPayload(0).toInt();

        for (int i = 0; i < sizeofFans(); i++) {
          if(fans[i].getPin() == p){
            int v = cmd.getPayload(1).toInt();
            fans[i].setSpeedPercentage(v);
          }
        }

    }else if(cmd.getCommand() == "mode"){
        this->deviceMode = cmd.getPayload().toInt();
        Serial.println("Controller set[mode: "+cmd.getPayload()+"]");

        switch (this->deviceMode) {
          case OFF:
            this->sensorValue = 0;
            for (int i = 0; i < sizeofFans(); i++) {
              fans[i].setSpeedPercentage(this->sensorValue);
            }
            break;

          case SERIAL_CTRL:
          // do nothing
            break;

          default:
            this->sensorValue = 30;
            for (int i = 0; i < sizeofFans(); i++) {
              fans[i].setSpeedPercentage(this->sensorValue);
            }
            break;
        }
    }

  }



  for (int i = 0; i < sizeofFans(); i++) {
      fans[i].run();
  }

  // Serial.println();
  delay(1000);
}

void FanController::printInfo(){

  String deviceMode = "unknown";

  switch (this->deviceMode) {
    case OFF:
    deviceMode="OFF";
    break;
    case SERIAL_CTRL:
    deviceMode="SERIAL_CTRL";
    break;
  }

  Serial.println("[status] Controller [mode: "+deviceMode+", sensorValue: "+String(this->sensorValue)+", powerStatus: "+String(this->powerStatus)+"]");

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
      // Serial.println("Power: OFF");
      deviceModePreOff = deviceMode;
      deviceMode = OFF;
    }else{
      // Serial.println("Power: ON");
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
  this->deviceModePreOff = this->deviceMode;

}
