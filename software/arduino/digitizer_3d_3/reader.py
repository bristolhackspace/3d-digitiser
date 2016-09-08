#!/usr/bin/python
import time
import math
import serial
import struct

FMT = '<cf'
packet_size = struct.calcsize(FMT)

print_interval = 0.1
serial_port=serial.Serial()
serial_port.port='/dev/ttyACM0'
serial_port.timeout=0.05
serial_port.baudrate=115200
serial_port.open()

pos = { 'x': 0, 'y': 0, 'z': 0, 'butt': 0 }
rot = { 'a': 0, 'b': 0, 'c': 0 }

def communicate():
    serial_port.write(b'a')
    while serial_port.inWaiting():
        response = serial_port.read(packet_size)

        if len(response) == packet_size:
            axis, amount = struct.unpack(FMT, response)
            if axis == b'x':
                pos['x'] = amount
            elif axis == b'y':
                pos['y'] = amount
            elif axis == b'z':
                pos['z'] = amount
            elif axis == b'B':
                pos['butt'] = amount
            elif axis == b'a':
                rot['a'] = math.degrees(amount)
            elif axis == b'b':
                rot['b'] = math.degrees(amount)
            elif axis == b'c':
                rot['c'] = math.degrees(amount)
        else:
            print("no response")

time_now = time.time()
butt = False
print("started")
try:
    while True:
        if time.time() - time_now > print_interval:
            communicate()
            time_now = time.time()
            if butt == False:
                if pos['butt']:
                    butt = True
                    print("x%2.2f y%2.2f z%2.2f" % (pos['x'], pos['y'], pos['z']))
                    #print("button press") 
            elif butt == True:
                if not pos['butt']:
                    butt = False
                    #print("button release") 
           # print("a%2.2f b%2.2f c%2.2f" % (rot['a'], rot['b'], rot['c']))
except KeyboardInterrupt:
    pass
