
#include "Arduino.h"
#include "Fan.h"

Fan::Fan() {
  _isNull = true;
  currentSpeed = 0; 
}

Fan::Fan(String fanName, int fanPin)
{
  pin = fanPin;
  name = fanName;
  currentSpeed = 0; 
  
  _isNull = false;
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
    FanRule f = fanRules[i];
    if (f.isRule()) {
      num++;
    } else {
      break;
    }
  }

  return num;
}

void Fan::setSpeed(int newSpeed) {
//  Serial.println("\nSet speeed: ");
//  Serial.println(newSpeed);
//  Serial.println(this->currentSpeed);
  float onePercent = fullSpeed / 100.00;
  int pwmSpeed = onePercent * newSpeed;

  if(this->currentSpeed == 0 && newSpeed > 0){
    analogWrite(pin, fullSpeed);
    delay(1000);
  }
  
  this->currentSpeed = newSpeed;

  analogWrite(pin, pwmSpeed);

  delay(100);
}



int Fan::getPin() {
  return pin;
}

String Fan::getName() {
  return name;
}

void Fan::setup(int _fullSpeed, int _minSpeed, int _startUpDelay) {
  Serial.println("Setup fan " + name);
  Serial.print("Rules: ");
  Serial.print(sizeofRules());
  Serial.println();
  listRules();

  fullSpeed = _fullSpeed;
  minSpeed = _minSpeed;
  startUpDelay = _startUpDelay;

}

void Fan::runAutoMode(int sensorValue, int noRuleSpeed) {

  // find matching rule
  bool ruleFound  = false;
  FanRule rule;

  for (int i = 0; i < sizeofRules(); i++) {
    FanRule r = fanRules[i];
    if (sensorValue >= r.getStart() && sensorValue <= r.getEnd()) {
      ruleFound = true;
      rule = r;
    }

  }

  if (ruleFound) {
    Serial.print(rule.toString());
    Serial.print(" < " + name);

    this->setSpeed(rule.getSpeed());
  } else {
    Serial.print("Rule: default : ");
    Serial.print(noRuleSpeed);
    Serial.print(" < " + name);

    this->setSpeed(noRuleSpeed);
  }
  Serial.println();

}

boolean Fan::isFan() {
  return !_isNull;
}
