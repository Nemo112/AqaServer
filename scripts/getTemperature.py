#!/usr/bin/python

import socket

HOST = '10.0.0.173'    # The remote host
PORT = 9011              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall('GT\r\n')
data = s.recv(10)
s.close()
print 'Received'
print data
