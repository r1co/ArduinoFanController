#include "Arduino.h"
#include "DeviceCommand.h"


DeviceCommand::DeviceCommand(){}

DeviceCommand::DeviceCommand(String rawCommand){

    // Serial.println("new DeviceCommand");
    // Serial.println(rawCommand);

    this->cmd =     this->getStringPart(rawCommand, '#', 0);
    this->payload = this->getStringPart(rawCommand, '#', 1);

    if(this->cmd.length() > 0){
      if(this->cmd == "status"){
        this->isValid = true;
      }else if(this->cmd == "set"){
        this->isValid = true;
      }else if(this->cmd == "mode"){
        this->isValid = true;
      }else if(this->cmd == "setFan"){
        this->isValid = true;
      }else if(this->cmd == "help"){
        this->isValid = true;
      }


    }
    // Serial.println(this->cmd);
    // Serial.println(this->payload);
    // Serial.println(this->isValid);
}


// 1:2:3:4:5:6:7:8;9:10
// 100 200 300 400 500 600 7 008 9
String DeviceCommand::getStringPart(String str, char seperator, int index){

  // Serial.println(str);
  // Serial.println(seperator);
  // Serial.println(index);
  // status#test 123
  // setFan#9=8812

  int start = 0;
  int rounds = 0;
  int indexOfSeperator = -1;
  String result = "";

  do {
      indexOfSeperator = str.indexOf(seperator);

      if(indexOfSeperator > -1){
          result = str.substring(start, indexOfSeperator);
      }else{
        if(index == 0){
          result = str.substring(start, str.length()-1);
        }else if(rounds == index){
          result = str.substring(start, str.length());
        }

      }
      rounds++;

      str = str.substring(indexOfSeperator+1);

  }while(indexOfSeperator > -1 && rounds <=index);

  // not found return empty string
  return result;
}

String DeviceCommand::getPayload(int index){
  return this->getStringPart(this->payload, '=', index);
}

String DeviceCommand::getCommand(){
  return this->cmd;
}

String DeviceCommand::getPayload(){
  return this->payload;
}

boolean DeviceCommand::isVaild(){
  return this->isValid;
}
