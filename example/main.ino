#include <FanController.h>

const int fullSpeed = 255; // pwm value - default
const int minSpeed = 75;   // pwm value - default
const int powerPin = 8;

FanController fanController = FanController(powerPin);

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);


  Fan fan1 = Fan(9);

  Fan fan2 = Fan(10);

  // set custom min/max speed
  fan2.setMinSpeed(minSpeed+20);
  fan2.setMaxSpeed(fullSpeed-20);

  Fan fan3 = Fan(11);
  fan3.setStartUpDelay(5000); // fan needs to run longer at full speed befor slowing down


  fanController.addFan(fan1);
  fanController.addFan(fan2);
  fanController.addFan(fan3);

  // setup fan controller
  fanController.setup(SERIAL_CTRL);

}

void loop() {
  // run fan controller
  fanController.run();

}
