/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef FanRule_h
#define FanRule_h

#include "Arduino.h"

class FanRule
{
    boolean _isNull;

    int tempStart;
    int tempEnd;
    int speed;


  public:
    FanRule();
    FanRule(int tStart, int tEnd, int tSpeed);

    boolean isRule();
    int getStart();

    int getEnd();
    int getSpeed();

    String toString();
};

#endif
