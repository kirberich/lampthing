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

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Control Lightwave devices via an arduino.')
    parser.add_argument('dev', type=str, help='serial device the arduino is connected to')
    args = parser.parse_args()

    ser = serial.Serial(args.dev, 9600, timeout=1)
    api = Api()
    api.demonize()

    while True:
        # api events
        for event, kwargs in api.events:
            if event == "set":
                print send_command(ser, kwargs['command'][0] + " " + kwargs["lamp"][0], wait_for_reply=False)
        api.events = []

        time.sleep(0.01)
