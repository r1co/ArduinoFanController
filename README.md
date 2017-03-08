# ArduinoFanController

This is a really basic framework to build an arduino based fan controller.

## Usage
```
#include <FanController.h>


FanController fanController = FanController(powerPin);

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);

  // create fan on pin 9
  Fan fan1 = Fan(9);

  // Add fan 	
  fanController.addFan(fan1);

  // set controller mode
  fanController.setup(SERIAL_CTRL);
}

void loop() {
  // run fan controller
  fanController.run();

}
```

With this basic code deployed to your arduino you can control pwm fans via serial commands.

## Commands

```
mode#<mode> - mode#1 - set device mode
setFan#<pin>=<speed> - setFan#9=100 - set fan on pin 9 to 100%
set#<speed> - set#55 - set all fans to 55% speed
status - prints fan status
```


### Device Modes

```
OFF=0
SERIAL_CTRL=1
```

## Extend
If you like to extend this framework with more features (using sensors or other stuff) you could start at **FanController.h** and **FanController.cpp**
