import sys, getopt, serial
import threading
import time

lines = -2
lastRecivedLine = ""

def closeApplication(run_event, thread):
    print "stop."
    run_event.clear()
    thread.join()
    # print "threads successfully closed"

def readSerialData(ser,run_event):
    global lines
    global lastRecivedLine

    while run_event.is_set():
        # print str(lines)+ " '"+lastRecivedLine+"'"

        reading = ser.readline().decode()

        if len(reading) > 0:
            reading = reading[:-1]
            lastRecivedLine = reading;
            lines = lines + 1
            print reading
        else:
            lastRecivedLine = ""

        time.sleep(1)

def usage():
    print 'test.py -p <port> -c <cmd>'
    sys.exit(2)

def main(argv):
   port = ''
   cmd = ''
   try:
      opts, args = getopt.getopt(argv,"hp:c:",["port=","cmd="])
   except getopt.GetoptError:
       usage()

   print opts
   for opt, arg in opts:
      if opt == '-h':
         usage()
      elif opt in ("-p", "--port"):
         port = arg
      elif opt in ("-c", "--cmd"):
         cmd = arg

   if port is "":
       usage()

   print 'port is ', port
   print 'cmd  is', cmd

   try:
        serial_port = serial.Serial(port, 9600, timeout=0)

        run_event = threading.Event()
        run_event.set()

        thread = threading.Thread(target=readSerialData, args=(serial_port,run_event,))
        thread.start()


        serial_port.write("hello\r\n")
        time.sleep(3)

        cmdSent = False

        while True:

            if cmd == "":
                input_var = raw_input("")

                if input_var  == "quit":
                    closeApplication(run_event, thread)
                    sys.exit()
                if input_var == "":
                    pass
                else:
                    print "you entered '" + input_var + "'"
                    serial_port.write(input_var+"\r\n")
            else:
                if cmdSent == False:
                    serial_port.write(cmd+"\r\n")
                    cmdSent = True
                else:
                    if lines > 0 and len(lastRecivedLine) == 0:
                        closeApplication(run_event, thread)
                        sys.exit()
                    time.sleep(1)

   except KeyboardInterrupt:
       closeApplication(run_event, thread)

if __name__ == "__main__":
   main(sys.argv[1:])
