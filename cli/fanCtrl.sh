#!/bin/bash


SERVER_LOCK="/tmp/fanController.lock"
SERVER_RUN_DIR="/tmp/fanController/"

INPUT_FILE="$SERVER_RUN_DIR/input"
OUTPUT_FILE="$SERVER_RUN_DIR/output"


if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi



function usageExit(){
  echo "$0 (status|set|setFan|mode|help)"
  exit 1;
}

function exitServerNotRunning(){
  echo "fanCtrl server not running"
  exit 1;
}

function sendCommand(){
  if [  -f $SERVER_LOCK ]; then
    echo "$1" > $INPUT_FILE
  else
    exitServerNotRunning
  fi
}

if [ -z ${1+x} ]; then
  usageExit
fi


if [ $1 = "status" ]; then
  sendCommand "status"
elif [ $1 = "set" ]; then

  if [ -z ${2+x} ]; then
    echo "$0 set <speed> - value in percents"
    exit 2
  fi

  sendCommand "set#$2"
elif [ $1 = "setFan" ]; then

  if [ -z ${2+x} ] || [ -z ${3+x} ]; then
    echo "$0 setFan <fan> <speed> - pin number, value in percents"
    exit 2
  fi

  sendCommand "setFan#$2=$3"

elif [ $1 = "mode" ]; then

  if [ -z ${2+x} ] || [ -z ${3+x} ]; then
    echo "$0 mode <mode> - mode number"
    exit 2
  fi

  sendCommand "setFan#$2=$3"

elif [ $1 = "help" ]; then
  usageExit
else
  usageExit
fi
