import time, serial
#from serial import Serial 

SERIAL_PORT = "COM8"

ser = serial.Serial(SERIAL_PORT, 9600)

while True: 
	ser.write((0xff,))
	time.sleep(0.5)
	ser.write((0,))
	time.sleep(0.5)


ser.close() 
