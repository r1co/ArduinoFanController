/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Fan_h
#define Fan_h

#include "Arduino.h"
#include "FanRule.h"

const int MAX_NUM_FAN_RULES = 10;



class Fan
{
  private:
    boolean _isNull;
    int pin;
    int currentSpeed;
    String name;

    int fullSpeed;
    int minSpeed;
    int startUpDelay;

    FanRule fanRules[MAX_NUM_FAN_RULES];

  public:
    Fan();
    Fan(String fanName, int fanPin);

    void listRules();
    void addRule(int start, int end, int speed);
    void setup(int fullSpeed, int minSpeed, int startUpDelay);
    void runAutoMode(int sensorValue, int noRuleSpeed);
    void setSpeed(int newSpeed);

    int getPin();
    int sizeofRules();

    String getName();

    boolean isFan();

};

#endif
