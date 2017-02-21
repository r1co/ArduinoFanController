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
    int speed;
    int maxSpeed;
    int minSpeed;
    int startUpDelay;

    FanRule fanRules[MAX_NUM_FAN_RULES];

    int sizeofRules();

    void setPWMSpeed(int value);

  public:
    Fan();
    Fan(int fanPin);

    void listRules();
    void addRule(int start, int end, int speed);
    void setup();
    void setSpeedPercentage(int value);

    // getter setter
    boolean isNull();

    int getPin();

    void setStartUpDelay(int startUpDelay);
    int getStartUpDelay();

    void setMaxSpeed(int fullSpeed);
    int getMaxSpeed();

    void setMinSpeed(int minSpeed);
    int getMinSpeed();

};

#endif
