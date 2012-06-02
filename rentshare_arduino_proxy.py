#!/usr/bin/env python

"""
An echo server that uses select to handle multiple clients at a time.
Entering any line of input at the terminal will exit the server.
"""

import select
import socket
import sys
import serial

sock_host = 'localhost'
sock_port = 8888
ser_host  = '/dev/ttyACM0'
ser_port  = 9600
backlog = 5
size = 1
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((sock_host,sock_port))
server.listen(backlog)
ser = serial.Serial(ser_host, ser_port)

input = [server,sys.stdin]
running = 1

print 'RentShare Arduino Proxy'
print '    Usage: write message and press enter. (max 7 characters)'
print ''

sys.stdout.write('>>> ')
sys.stdout.flush()

while running:
	inputready,outputready,exceptready = select.select(input,[],[])

	for s in inputready:

		if s == server:
			# handle the server socket
			client, address = server.accept()
			input.append(client)

		elif s == sys.stdin:
			# handle standard input
			data = sys.stdin.readline().strip()
			ser.write(data)
			ser.write(chr(0))
			sys.stdout.write('>>> ')
			sys.stdout.flush()

		else:
			# handle all other sockets
			data = s.recv(size)
			if data:
				ser.write(data)
			else:
				s.close()
				input.remove(s)
server.close()
