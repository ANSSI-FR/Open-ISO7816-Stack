#!/usr/bin/python

import serial
import sys

ser = serial.Serial('/dev/ttyUSB0')

n = int(sys.argv[1])

print(repr(ser.read(n)))

quit()
