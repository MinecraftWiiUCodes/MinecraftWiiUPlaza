import os
import math
import socket
from time import sleep

#---------------
# NNID Hex Conversion
#---------------
text=input('NNID: ')
IP=input('Wii U IP: ')
hexm=text.encode('utf-16be', 'replace').hex().upper()
offset=0x00
code='30000000109CED1810000000500000003100000000000050'
hexc=len(hexm)
if(len(text)%2)!=0:
    hexc=len(hexm)+8
for x in range(math.floor(hexc/8)):
    code=code+'0012'+format(offset+x*4,'04X')+hexm[x*8:x*8+4]
    code=code+hexm[x*8+4:x*8+8]
if(len(text)%2)!=0:
    code=code+'0000'
code=code+'0012'+format(offset+x*4+4,'04X')+'00000000'+'D0000000DEADCAFE'
print('Converting to Hex..')
print(' ')
sleep(0.4)
print('Converted! Now sending code..')

sleep(0.7)

def sendCode():
	ip=IP # IP Address
	print('\n ** Changing Name.. **\n')
	print('  Connecting to: '+ip)
	sleep(0.8)
	s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect((ip,7331))
	s.send(bytes.fromhex('03'))
	s.send(bytes.fromhex('10014CFC00000000'))
	
	cafe=code # Cafe Code
	for x in range(math.floor(len(cafe)/8)):
	    s.send(bytes.fromhex('03'))
	    s.send(bytes.fromhex('0'+format(0x01133000+x*4,'X')+cafe[x*8:x*8+8]))
	s.send(bytes.fromhex('03'))
	s.send(bytes.fromhex('10014CFC00000001'))
	s.close()
	print('\nSent Code! Name should now be changed!')

sendCode()

