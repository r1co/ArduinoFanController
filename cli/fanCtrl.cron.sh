#!/bin/bash
SESSION=fanCtrl

if [ -z "$1" ]; then
  echo "usage: $0 <port>"
  exit 1
fi

SCRIPT=`realpath $0`
SCRIPTPATH=`dirname $SCRIPT`

tmux="/usr/local/bin/tmux -2 "
clientCmd="python2.7 $SCRIPTPATH/fanCtrl.client.py -p $1"

# if the session is already running, just attach to it.
$tmux has-session -t $SESSION
if [ $? -eq 0 ]; then
       echo "Session $SESSION already exists."
       exit 0;
fi

# create a new session, named $SESSION, and detach from it
$tmux new-session -d -s $SESSION
$tmux new-window    -t $SESSION:0
#$tmux split-window  -v -t $SESSION:0
$tmux select-window -t $SESSION:0
tmux send-keys -t $SESSION:0 C-z "$clientCmd" Enter

echo "Session $SESSION created."
