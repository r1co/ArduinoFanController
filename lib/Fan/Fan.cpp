#include "Arduino.h"
#include "Fan.h"

Fan::Fan() {
  this->_isNull = true;
}

Fan::Fan(int fanPin)
{
  this->pin = fanPin;
  this->startUpDelay = 2000;
  this->maxSpeed = 255;
  this->minSpeed = 75;
  this->_isNull = false;
}

void Fan::addRule(int start, int end, int speed) {

  int i = sizeofRules();

  fanRules[i] = FanRule(start, end, speed);
}

void Fan::listRules() {
  for (int i = 0; i < sizeofRules(); i++ ) {
    FanRule r = fanRules[i];
    Serial.print(r.toString());
    Serial.println();
  }
}

int Fan::sizeofRules() {

  int rawSize = (sizeof(fanRules) / sizeof(FanRule));
  int num = 0;

  for (int i = 0; i < rawSize; i++) {
    if (fanRules[i].isRule()) {
      num++;
    } else {
      break;
    }
  }

  return num;
}

void Fan::setup() {
  Serial.println("Setup fan ");
  Serial.print("Rules: ");
  Serial.print(sizeofRules());
  Serial.println();
  listRules();
}

void Fan::setPWMSpeed(int value){

  // if fan was stopped set fullSpeed to ensure rotation
  if(this->speed == 0 && value > 0){
    analogWrite(this->pin , this->maxSpeed);
    delay(1000);
  }

  // check limits
  if(value > this->maxSpeed){
    value = this->maxSpeed;
  }else if(value < this->minSpeed){
    value = this->minSpeed;
  }

  this->speed = value;
  analogWrite(this->pin , this->speed);
}

void Fan::setSpeedPercentage(int value){
  float onePercent = maxSpeed / 100.00;
  int pwmSpeed = onePercent * value;

  this->setPWMSpeed(pwmSpeed);

}

// getter stter
boolean Fan::isNull() {
  return !_isNull;
}

int Fan::getPin() {
  return this->pin ;
}

void Fan::setStartUpDelay(int startUpDelay){
  this->startUpDelay = startUpDelay;
}

void Fan::setMaxSpeed(int maxSpeed){
  this->maxSpeed = maxSpeed;
}

void Fan::setMinSpeed(int minSpeed){
  this->minSpeed = minSpeed;
}

int Fan::getStartUpDelay(){
  return this->startUpDelay;
}

int Fan::getMaxSpeed(){
  return this->maxSpeed;
}

int Fan::getMinSpeed(){
  return this->minSpeed;
}
