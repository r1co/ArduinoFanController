import sys, getopt, serial
import threading
import time
import os
import os.path


lines = -2
lastRecivedLine = ""
logEnabled = False
logFile = ""


lockFile = "/tmp/fanController.lock"

def createRunLock():
    outputFileHandler = open(lockFile,"a")
    outputFileHandler.write("lock\r\n")
    outputFileHandler.close()


def rmRunLock():
    os.remove(lockFile)

def checkRunLock():
    if os.path.exists(lockFile):
        sys.exit(0)
    else:
        createRunLock()

def log(data):
    if logEnabled == True:
        outputFileHandler = open(logFile,"a")
        outputFileHandler.write(data+"\r\n")
        outputFileHandler.close()

def closeApplication(run_event, thread):
    print "stop."

    if run_event != None:
        run_event.clear()

    if thread != None:
        thread.join()

    rmRunLock()
    # print "threads successfully closed"

def readSerialData(ser,run_event):

    outputFile = "/tmp/fanController/output"
    global lines
    global lastRecivedLine

    while run_event.is_set():
        # print str(lines)+ " '"+lastRecivedLine+"'"

        reading = ser.readline().decode()

        if len(reading) > 0:
            reading = reading[:-1]
            lastRecivedLine = reading;
            lines = lines + 1
            outputData = "[recv] "+str(reading)

            outputFileHandler = open(outputFile,"a")
            outputFileHandler.write(outputData+"\r\n")
            outputFileHandler.close()

            log(outputData)

            print outputData


        else:
            lastRecivedLine = ""

        time.sleep(1)

def sendSerialData(serial_port,data):
    outputData =  "[send] "+str(data)
    log(outputData)
    print outputData
    serial_port.write(data+"\r\n")

def usage():
    print 'fanCtrl.server.py -p[--port] <port> [-l[--log] <logfile>]'
    sys.exit(2)

def main(argv):

   global logEnabled
   global logFile

   inputFile = "/tmp/fanController/input"
   port = ''

   try:
      opts, args = getopt.getopt(argv,"hp:l:",["port=","log="])
   except getopt.GetoptError:
       usage()

   for opt, arg in opts:
      if opt == '-h':
         usage()
      elif opt in ("-p", "--port"):
         port = arg
      elif opt in ("-l", "--log"):
         logFile = arg

   if port is "":
       usage()

   if len(logFile) > 0:
       logEnabled = True

   # print 'port is ', port

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
            time.sleep(1)
            if os.path.exists(inputFile):
                inputFileHandler = open(inputFile,"r")
                inputData =  inputFileHandler.read()
                inputData = inputData[:-1]

                sendSerialData(serial_port, inputData)
                inputFileHandler.close()
                os.remove(inputFile)


                pass
   except KeyboardInterrupt:
       print "Reviced KeyboardInterrupt"
       closeApplication(run_event, thread)
   except:
       print "Error:"
       print "Unexpected error:", sys.exc_info()
       closeApplication(None, None)

if __name__ == "__main__":
    checkRunLock()

    if not os.path.exists("/tmp/fanController/"):
        os.makedirs("/tmp/fanController/")

    main(sys.argv[1:])
