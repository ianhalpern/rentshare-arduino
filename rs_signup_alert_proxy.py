import socket, serial
ser = serial.Serial('/dev/ttyACM0', 9600)

sck = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sck.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sck.bind(('localhost',8888))
sck.listen(1)
while 1:
	conn, addr = sck.accept()
	while 1:
		ser.write( conn.recv(1) )
	conn.close()
