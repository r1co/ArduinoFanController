import sys, getopt, serial

def sendSerialCmd(port, command):
    # print port+" > "+command

    serial_port = serial.Serial("/dev/cu.usbmodem1411", 9600, timeout=0)

    if serial_port.is_open():
        serial_port.write("status\r\n")
        reading = serial_port.readline().decode()
        print reading

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

   if port is "" or cmd is "":
       usage()

   print 'port file is ', port
   print 'cmd file is', cmd

   if cmd == "status":
       sendSerialCmd(port, cmd)


if __name__ == "__main__":
   main(sys.argv[1:])
