
import os
import math
import socket

# Create mod menu from input

IP=input('Wii U IP: ')

tab=4 # Set which creative menu tab the item is in
slot=1 # Set which slot it's in.
id=276 # Set the item ID
cnt=64 # How many of it? Example: 64 being a stack.
dmg=0 # Durability level.

""" List of tabs
0: Building Blocks
1: Decorations
2: Redstone
3: Materials
4: Food
5: Tools and armor
6: Misc
"""

if(tab==0):
	taba='10A0A6C0'
elif(tab==1):
	taba='10A0A6D0'
elif(tab==2):
	taba='10A0A6F0'
elif(tab==3):
	taba='10A0A700'
elif(tab==4):
	taba='10A0A710'
elif(tab==5):
	taba='10A0A720'
else:
	taba='10A0A770'

code='30000000109DF84C100000005000000010120001'+format(id*4,'08X')+'30000000'+taba+'100000005000000031000000'+format(slot*4,'08X')+'3010000000000000100000005000000011120001000000140012000C'+format(cnt,'08X')+'00120020'+format(dmg,'08X')+'0012001812000000D0000000DEADCAFE0100001C120000001058FA5400A8A59800000011000000013F8000000000001112000030000000FF3000000012000018100000005000000000120044120000D401000044120000A01056415C00000000000000000000000011B0D4300000AFB8000000000000000011B0D430120000E00000000000000017120000A012000134000000000000003B00000000000000FF01000018120000E00055006E0062007200650061006B00610062006C0065000000000000000000FF010000301200011011B0D4300000AFB8000000000000000011B0D4301200014000000000000000171200100012000184000000000000003B00000000000000FF01000014120001400064006900730070006C00610079000000000000000000FF0100001C120010001058FA5400A8A59800000011000000013F8000000000001112001030000000FF3000000012001018100000005000000000120044120010D401000044120010A01065993C00000000000000000000000011B0D4300000AFB8000000000000000011B0D430120010E00000000000000017120010A000000000000000000000003B00000000000000FF01000016120010E00000000000000000000000000000000000000000000000FF010000301200016011B0D4300000AFB8000000000000000011B0D43012000190000000000000001712002000120001D4000000000000003B00000000000000FF01000014120001900065006E00630068000000000000000000000000000000FF01000030120001B011B0D4300000AFB8000000000000000011B0D430120001E000000000000000171200500012000254000000000000003B00000000000000FF01000026120001E0004100740074007200690062007500740065004D006F0064006900660069006500720073000000FF010000301200023011B0D4300000AFB8000000000000000011B0D4301200026000000000000000171200A00000000000000000000000003B00000000000000FF0100002C120002600043007500730074006F006D0050006F00740069006F006E0045006600660065006300740073000000000000000000FF0100000C120020001065993C0000000000000000000000FF0100000C120050001065993C0000000000000000000000FF0100000C1200A0001065993C0000000000000000000000FF'
print(code)

def sendCode():
	ip=IP # IP Address
	print('\n ** Sending Menu.. **\n')
	print('  Connecting to: '+ip)
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
	print('\nSent Code! Menu should now be modified!')

sendCode()
