import serial

port0 = '/dev/ttyACM0'
port1 = '/dev/ttyACM1'

brate0 = 9600
brate1 = 57600
cmd0 = 'DDDOT'
cmd1 = 'SNCWW'

ser0 = serial.Serial(port0, baudrate = brate0, timeout = None)
ser1 = serial.Serial(port1, baudrate = brate1, timeout = None)
print('%s and %s' % (ser0.name, ser1.name))
print('Dust1, Dust2, Dust3, Temp, Humi, Ozone, SO2, No2, CO, WindSpeed, WindDirection\n') 

ser0.write(cmd0.encode())
ser1.write(cmd1.encode())

while True:
	if ser0.in_waiting != 0 and ser1.in_waiting != 0 :
		contect0 = ser0.readline()
		contect1 = ser1.readline()
		print('%s, %s\n' %(contect0[:-2].decode(), contect1[:-2].decode()))
