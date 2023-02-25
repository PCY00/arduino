import serial

port='/dev/ttyACM1'
brate = 57600
cmd = 'SNCWW'

ser = serial.Serial(port, baudrate = brate, timeout = None)
print(ser.name)

ser.write(cmd.encode())

while True:
	if ser.in_waiting != 0 :
		contect = ser.readline()
		print(contect[:-2].decode())
