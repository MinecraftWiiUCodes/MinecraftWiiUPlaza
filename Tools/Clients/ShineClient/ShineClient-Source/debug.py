import socket
import time
import webbrowser,sys,subprocess
import math
import os

ip="192.168.0.185"

address=0x4CC0000
code=""
offset=0

popen = subprocess.Popen('assemble\\build.bat',shell=True)
popen.wait()

with open("assemble\\asm",'rb') as f:
    tmp=f.read()
    f.close()
tmp=tmp.hex().upper()
time.sleep(0.1)

s=[]
for x in range(math.floor(len(tmp)/8)):
    s.append(tmp[x*8:x*8+8])
    
for x in range(len(s)):
    code=code+(format(address+offset,'08X')+" "+s[x][0:8])+"\n"
    offset=offset+4

code=code[:-1]

with open("scripts\\main",'w') as f:
    f.write(code)
    f.close()

print("connecting...")
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((ip,7331))
print("connected"+ip)

sends=[]
with open("scripts\\api-CodeEnablesSetter") as f:
    sends=f.readlines()
    f.close
with open("scripts\\api-Resetter") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\api-GetHackItems") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\api-GetHackItems2") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\api-RainbowArmor") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\api-Nuker") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\main") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\api-ArgSetter") as f:
    sends=sends+f.readlines()
    f.close
with open("scripts\\Run") as f:
    sends=sends+f.readlines()
    f.close

for x in range(len(sends)):
    if(("#" in sends[x])==False):
        s.send(bytes.fromhex('03'))
        s.send(bytes.fromhex(sends[x].replace("\n","")))
        print("send "+sends[x])
s.close()
print("disconnected")
time.sleep(3)
