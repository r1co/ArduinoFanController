#ifndef DeviceCommand_h
#define DeviceCommand_h

#include "Arduino.h"

class DeviceCommand {
private:
  boolean isValid = false;
  String cmd;
  String payload;

  String getStringPart(String str, char seperator, int index);

public:
  DeviceCommand();
  DeviceCommand(String rawCommand);

  String getCommand();
  String getPayload();
  String getPayload(int index);
  boolean isVaild();
};

#endif
