#include "FanController.h"

const int fullSpeed = 255; // pwm value
const int minSpeed = 75;   // pwm value
const int startUpDelay = 1000; // milis
const int powerPin = 8;

const int noRuleSpeed = 50; // percentage

FanController fanController = FanController(fullSpeed, minSpeed, startUpDelay, noRuleSpeed, powerPin);

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);


  Fan fan1 = Fan("back", 9);
  fan1.addRule(0, 15, 0);
  fan1.addRule(16, 30, 30);
  fan1.addRule(31, 40, 50);
  fan1.addRule(41, 50, 75);
  fan1.addRule(51, 10000, 100);

  Fan fan2 = Fan("front_left", 10);
  fan2.addRule(0, 34, 0);
  fan2.addRule(35, 40, 30);
  fan2.addRule(41, 50, 75);
  fan2.addRule(51, 100, 100);
 
  Fan fan3 = Fan("front_right", 11);
  fan3.addRule(0, 49, 0);
  fan3.addRule(50, 100, 100);

  fanController.addFan(fan1);
  fanController.addFan(fan2);
  fanController.addFan(fan3);

  // setup fan controller
  fanController.setup();
}

void loop() {
  // run fan controller
  fanController.run();

}


