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
  this->speed = this->minSpeed;
  this->_isNull = false;
}

// *****************************
// **** PUBLIC
// *****************************
void Fan::setup() {

}


void Fan::addRule(int start, int end, int speed) {

  int i = sizeofRules();

  fanRules[i] = FanRule(start, end, speed);
}


void Fan::setSpeedPercentage(int value){
  float onePercent = maxSpeed / 100.00;
  int pwmSpeed = onePercent * value;

  this->setPWMSpeed(pwmSpeed);

}

void Fan::run(){
  analogWrite(this->pin , this->speed);
  // this->printInfo();
}

// *****************************
// **** PRIVATE
// *****************************
void Fan::setPWMSpeed(int value){


  // if fan was stopped set fullSpeed to ensure rotation
  if(this->speed == 0 && value > 0){
    analogWrite(this->pin , this->maxSpeed);
    delay(1000);
  }

  // check limits
  if(value > this->maxSpeed && value != 0){
    value = this->maxSpeed;
  }else if(value < this->minSpeed && value != 0){
    value = this->minSpeed;
  }

  // Serial.print("Fan pin ");
  // Serial.print(this->pin);
  // Serial.print(" speed: ");
  // Serial.print(value);
  // Serial.print("pwm");
  // Serial.println();

  this->speed = value;
  analogWrite(this->pin , this->speed);

  // this->printInfo();
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

void Fan::printInfo(){
  Serial.println("[status] Fan [value: "+String(this->speed)+", pin: "+String(this->pin)+"]");
}

// *****************************
// **** GETTER && SETTER
// *****************************
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
