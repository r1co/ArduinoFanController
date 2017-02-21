#include "Arduino.h"
#include "FanRule.h"

FanRule::FanRule() {
  _isNull = true;
}

FanRule::FanRule(int tStart, int tEnd, int tSpeed) {
  tempStart = tStart;
  tempEnd = tEnd;
  speed = tSpeed;
  _isNull = false;

}

boolean FanRule::isRule() {
  return !_isNull;
}

int FanRule::getStart() {
  return tempStart;
}

int FanRule::getEnd() {
  return tempEnd;
}


int FanRule::getSpeed() {
  return speed;
}

String FanRule::toString() {
  return "Rule: " + String(tempStart) + " - " + String(tempEnd) + " : " + String(speed);
}
