import time
import serial
import argparse
from api import Api

def send_command(ser, command, wait_for_reply=True, wait_for_empty_buffer=False):
    """ Send serial command to arduino  
        Set wait_for_reply to wait and return reply 
        wait_for_empty_buffer to continue reading until the buffer is empty
    """

    retval = []
    ser.write(command+"\n")
    if wait_for_reply:
        retval.append(ser.readline().strip())

    if wait_for_empty_buffer:
        while ser.inWaiting():
            retval.append(ser.readline().strip())

    return retval

def make_message(command, lamp, remote):
    command = int(command[0])
    lamp = int(lamp[0])
    remote = int(remote[0])
    return "c%02d %02d %02d" % (command, lamp, remote)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Control Lightwave devices via an arduino.')
    parser.add_argument('dev', type=str, help='serial device the arduino is connected to')
    args = parser.parse_args()

    ser = serial.Serial(args.dev, 9600, timeout=1)
    api = Api()
    api.demonize(port=8082)

    while True:
        # api events
        for event, kwargs in api.events:
            if event == "set":
                print send_command(ser, make_message(**kwargs), wait_for_reply=False, wait_for_empty_buffer=False)
        while ser.inWaiting():
            print ser.readline().strip()
        api.events = []

        time.sleep(0.01)
