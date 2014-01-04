import serial

def send_command(ser, command):
    """ Send serial command to arduino """

    retval = []
    ser.write(command)
    retval.append(ser.readline().strip())
    while ser.inWaiting():
        retval.append(ser.readline().strip())

    return retval

ser = serial.Serial('/dev/tty.usbmodem1411', 9600, timeout=1)