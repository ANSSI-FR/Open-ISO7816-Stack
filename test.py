#!/usr/bin/python

import sys
import serial

ser = serial.Serial('/dev/ttyUSB1')

print(repr(ser.read(int(sys.argv[1]))))
