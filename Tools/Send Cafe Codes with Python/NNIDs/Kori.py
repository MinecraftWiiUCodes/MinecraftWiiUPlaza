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

cafe='30000000101C9C0810000000500000003100000000000058301000000000000010000000500000000012008C004B006F0012009000720069001200940000000000120098000000000012009C00000000001200A000000000001200A400000000001200A800000000001200AC00000000001200B000000000001200B400000000001200B800000000001200BC00000000001200C000000000001200C400000000001200C800000000001200CC00000000D0000000DEADCAFE' # Cafe Code
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

