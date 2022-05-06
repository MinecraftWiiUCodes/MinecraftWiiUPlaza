import math
import socket
import os

ip='192.168.1.97' # IP Address
print('\n ** Sending Cafe Code! **\n')
print('  Connecting to: '+ip)
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((ip,7331))
s.send(bytes.fromhex('03'))
s.send(bytes.fromhex('10014CFC00000000'))

cafe='C00000A7600000003DE000003C8010A061EF00006084A628808400008084009C6000000091E409A83DE000803C8010A961EF000060840E6C8084000091E400183C00010F60006AE07C0903A64E800420' # Cafe Code
for x in range(math.floor(len(cafe)/8)):
    s.send(bytes.fromhex('03'))
    s.send(bytes.fromhex('0'+format(0x01133000+x*4,'X')+cafe[x*8:x*8+8]))
s.send(bytes.fromhex('03'))
s.send(bytes.fromhex('10014CFC00000001'))

asm='' # Assembly RAM Write
for x in range(math.floor(len(asm)/16)):
    s.send(bytes.fromhex('03'))
    s.send(bytes.fromhex(asm[x*16:x*16+16]))
s.close()
print('\nSent Codes Successfully')

