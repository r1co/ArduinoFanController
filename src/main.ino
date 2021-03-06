#include <FanController.h>

const int powerPin = 8;

FanController fanController = FanController(powerPin);

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);


  Fan fan1 = Fan(9);

  Fan fan2 = Fan(10);

  Fan fan3 = Fan(11);
  
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
