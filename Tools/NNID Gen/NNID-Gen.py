import os
import math

#---------------
# NNID Hex Conversion
#---------------

text=input('NNID: ')
hexm=text.encode('utf-16be', 'replace').hex().upper()
offset=0x8C
code='30000000 101C9C08\n10000000 50000000\n31000000 00000058\n30100000 00000000\n10000000 50000000'
hexc=len(hexm)
if(len(text)%2)!=0:
    hexc=len(hexm)+8
for x in range(math.floor(hexc/8)):
    code=code+'\n0012'+format(offset+x*4,'04X')+' '+hexm[x*8:x*8+4]
    code=code+hexm[x*8+4:x*8+8]
if(len(text)%2)!=0:
    code=code+'0000'
code=code+'\n0012'+format(offset+x*4+4,'04X')+' 00000000'+'\nD0000000 DEADCAFE'
print(code)
