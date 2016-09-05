#!/usr/bin/python
import time
import math
import serial
import struct

FMT = '<cf'

print_interval = 0.1
serial_port=serial.Serial()
serial_port.port='/dev/ttyACM0'
serial_port.timeout=0.10
serial_port.baudrate=9600
serial_port.open()

pos = { 'x': 0, 'y': 0, 'z': 0 }
rot = { 'a': 0, 'b': 0, 'c': 0 }

def communicate():
    packet_size = struct.calcsize(FMT)
    response = serial_port.read(packet_size)
    if len(response) == packet_size:
        axis, amount = struct.unpack(FMT, response)
        if axis == 'x':
            pos['x'] = amount
        elif axis == 'y':
            pos['y'] = amount
        elif axis == 'z':
            pos['z'] = amount
        elif axis == 'a':
            rot['a'] = math.degrees(amount)
        elif axis == 'b':
            rot['b'] = math.degrees(amount)
        elif axis == 'c':
            rot['c'] = math.degrees(amount)

time_now = time.time()
try:
    while True:
        communicate()
        if time.time() - time_now > print_interval:
            time_now = time.time()
            print("x%2.2f y%2.2f z%2.2f" % (pos['x'], pos['y'], pos['z']))
           # print("a%2.2f b%2.2f c%2.2f" % (rot['a'], rot['b'], rot['c']))
except KeyboardInterrupt:
    pass
