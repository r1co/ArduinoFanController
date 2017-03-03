import serial
import time
import threading
import BaseHTTPServer
import re


HOST_NAME = 'localhost' # !!!REMEMBER TO CHANGE THIS!!!
PORT_NUMBER = 8080 # Maybe set this to 9000.

connected = False
serial_port = serial.Serial("/dev/cu.usbmodem1411", 9600, timeout=0)




class FanController:
    def __init__(self, **kwargs):
        self.__dict__.update(kwargs)
class Fan:
    def __init__(self, **kwargs):
        self.__dict__.update(kwargs)


fans = {}
fanController = {}

def handleSerialData(data):
    data = data[:-1]
    if len(data) > 0:

        try:
            patternStatusController = re.search('\[status\] Controller \[([a-zA-Z0-9:_, ]*)\]', data)

            if patternStatusController != None:

                try:
                    patternMode = re.search('mode:[ ]*([A-Za-z0-9_]*)', patternStatusController.group(1))
                    patternValue = re.search('sensorValue:[ ]*([A-Za-z0-9_]*)', patternStatusController.group(1))
                    patternPowerStatus = re.search('powerStatus:[ ]*([A-Za-z0-9_]*)', patternStatusController.group(1))


                    if patternMode != None and patternValue != None and patternPowerStatus != None:
                        fanController[0] = FanController(mode=patternMode.group(1), sensorValue=patternValue.group(1), powerStatus=patternPowerStatus.group(1))
                    #     if fanController == None:
                    #         print "set"
                    #         fanController = FanController(mode=1, sensorValue=1)
                    #     else:
                    #         print "fan controller is set"
                    # if patternValue != None:
                except:
                    print "Error: patternStatusController"


            else:
                patternStatusFan = re.search('\[status\] Fan \[([a-zA-Z0-9:_, ]*)\]', data)
                if patternStatusFan != None:
                    patternValue = re.search('value:[ ]*([A-Za-z0-9_]*)', patternStatusFan.group(1))
                    patternPin = re.search('pin:[ ]*([A-Za-z0-9_]*)', patternStatusFan.group(1))

                    if patternPin != None and  patternValue != None:

                        try:
                            pin = patternPin.group(1)
                            value = patternValue.group(1)

                            if pin in fans:
                                fans[pin].value = value
                            else:
                                fans[pin] = Fan(pin=pin, value=value)

                        except:
                            print "ERROR 2"






        except:
            print "ERROR"

def readSerialData(ser,run_event):
    while run_event.is_set():
        reading = ser.readline().decode()
        handleSerialData(reading)
        time.sleep(1)

def getDeviceStatus():
    serial_port.write("status\r\n")

def setFanSpeed(fanId, fanValue):
    serial_port.write("setFan#"+fanId+"="+fanValue+"\r\n")

def setDeviceSpeed(fanValue):
    serial_port.write("set#"+fanValue+"\r\n")


def setDeviceMode(mode):
    print "Set DeviceMode", mode
    serial_port.write("mode#"+mode+"\r\n")

def startStatusUpdater():
    threading.Timer(5.0, startStatusUpdater).start()
    serial_port.write("status\r\n")


def sendIndexPage(s):
    # print(fanController)
    if fanController != None and fanController[0] != None:
        s.wfile.write('{')
        s.wfile.write('"FanController":{')
        s.wfile.write('"mode": "'+  str(fanController[0].mode) +'",')
        s.wfile.write('"sensorValue": '+  str(fanController[0].sensorValue)  +',')
        s.wfile.write('"powerStatus": '+str(fanController[0].powerStatus))
        s.wfile.write('},')
        s.wfile.write('"Fans":[')
        for (i, item) in enumerate(fans):

            s.wfile.write('{')
            s.wfile.write('"pin": '+fans[item].pin+",")
            s.wfile.write('"value": '+fans[item].value)
            s.wfile.write('}')

            if i != len(fans)-1:
                s.wfile.write(',')

        s.wfile.write(']')
        s.wfile.write('}')
    else:
        s.wfile.write('{}')

class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(s):
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
    def do_GET(s):
        """Respond to a GET request."""
        s.send_response(200)
        s.send_header("Content-type", "application/json")
        s.end_headers()


        if True:
            setFanPattern = re.match('\/set\/fan\/([0-9]*)\/value\/([0-9]*)', s.path)
            if setFanPattern != None and len( setFanPattern.groups() )  > 0:
                pin = setFanPattern.group(1);
                value = setFanPattern.group(2);

                if pin in fans:
                    s.wfile.write('{')
                    s.wfile.write('"pin": '+fans[pin].pin+",")
                    s.wfile.write('"value": '+value)
                    s.wfile.write('}')
                    setFanSpeed(pin, value)
                else:
                    s.wfile.write('{ "error": "not found" }')
            else:
                getFanPattern = re.search('\/set\/fan\/([0-9]*)([\/]*)', s.path)
                if getFanPattern != None and  len( getFanPattern.groups() )  > 0:
                    pin = getFanPattern.group(1);

                    if pin in fans:
                        s.wfile.write('{')
                        s.wfile.write('"pin": '+fans[pin].pin+",")
                        s.wfile.write('"value": '+fans[pin].value)
                        s.wfile.write('}')
                    else:
                        s.wfile.write('{ "error": "not found" }')


                else:
                    setModePattern = re.search('\/set\/mode\/([0-9]*)([\/]*)', s.path)

                    if setModePattern  != None and len(setModePattern.groups()) > 0:
                        setDeviceMode(setModePattern.group(1))
                        sendIndexPage(s)
                    else:
                        setAllSpeedPattern = re.match('\/set\/([0-9]*)', s.path)

                        if setAllSpeedPattern != None and len(setAllSpeedPattern.groups()) > 0:
                            setDeviceSpeed(setAllSpeedPattern.group(1))
                            sendIndexPage(s)
                        else:
                            sendIndexPage(s)
        else:
            s.wfile.write('{error}')


        # s.wfile.write('{ "ASD": 2}')





def main():

    run_event = threading.Event()
    run_event.set()



    thread = threading.Thread(target=readSerialData, args=(serial_port,run_event,))
    thread.start()

    startStatusUpdater()

    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print "attempting to close threads."
        run_event.clear()
        thread.join()
        print "threads successfully closed"
    httpd.server_close()
    print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)

    # try:
    #     while 1:
    #         time.sleep(.1)
    # except KeyboardInterrupt:
    #     print "attempting to close threads. Max wait ="
    #     run_event.clear()
    #     thread.join()
    #     print "threads successfully closed"


if __name__ == '__main__':
    main()
