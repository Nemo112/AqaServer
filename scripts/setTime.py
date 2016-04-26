#!/usr/bin/python
from __future__ import print_function
import re
import socket

HOST = '10.0.0.173'    # The remote host
PORT = 9011              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall('GN\r\n')
data = "";
tmp=s.recv(1)
while (tmp):
	data = data + tmp
	tmp=s.recv(1)
s.close()
print('Time: ' + str(data),end='')
# Time: 12:33:31 26/4/16 Day of week: Tuesday
#m = re.search('Time: [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9]/[0-9][0-9]/[0-9][0-9] Day of week: \W', data) 
tm = re.match("\d\d:\d\d:\d\d", data)
if tm.group(0) == "00:00:00":
	s.connect((HOST, PORT))
	s.sendall('SN\r\n')


