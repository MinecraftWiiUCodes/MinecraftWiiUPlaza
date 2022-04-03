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

cafe='09020000102EFA640000018000000000000200001066ACC83E26AD8900000000D0000000DEADCAFE09020000102EFA640000028000000000000200001066ACC8411B8A3D00000000D0000000DEADCAFE09020000102EFA640000048000000000000200001066ACC84000000000000000D0000000DEADCAFE09020000102EFA640000088000000000000200001066ACC83EE475C300000000D0000000DEADCAFE' # Speed
for x in range(math.floor(len(cafe)/8)):
    s.send(bytes.fromhex('03'))
    s.send(bytes.fromhex('0'+format(0x01133000+x*4,'X')+cafe[x*8:x*8+8]))
s.send(bytes.fromhex('03'))
s.send(bytes.fromhex('10014CFC00000001'))

cafe='09020000102EFA6400000008000000003000000010A0A6441000000050000000031000000000009C000000000000000030000000101D1D581000000050000000310000000000008430100000000000001000000050000000101200050000002C1404050000000001111200050000002CD0000000DEADCAFE' # Rejoin
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

