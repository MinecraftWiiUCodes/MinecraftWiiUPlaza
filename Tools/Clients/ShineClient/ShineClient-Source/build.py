import webbrowser,sys,subprocess
import math
import os
import time

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

time.sleep(0.1)

sends=[]
with open("scripts\\api-CodeEnablesSetter") as f:
    sends=f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\api-Resetter") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\api-GetHackItems") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\api-GetHackItems2") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\api-RainbowArmor") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\api-Nuker") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\main") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\api-ArgSetter") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("scripts\\Run") as f:
    sends=sends+f.readlines()
    f.close

code=''
for x in range(len(sends)):
    if(("#" in sends[x])==False):
        if(sends[x]!="\n"):
            if(sends[x][:2]!="16"):
                code=code+"writeKernelMemory((void *)0x"+sends[x][:8]+",0x"+sends[x][9:17].replace("1600","1003")+"L);\n"
            else:
                code=code+"int value"+str(x)+"=0x"+sends[x][9:17]+";\n"
                code=code+"kernelCopyData((unsigned char *) 0x"+sends[x][:8].replace("1600","1003")+",(unsigned char *)&value"+str(x)+",sizeof(value"+str(x)+"));\n"

with open("elf\\src\\tcpgecko\\tcp_gecko.cpp") as f:
    s=f.read()
    f.close()

sa=s[s.index("// Start ShineClient")+21:s.index("// End ShineClient")]
s=s.replace(sa,code)

with open("elf\\src\\tcpgecko\\tcp_gecko.cpp",'w') as f:
    f.write(s)
    f.close()

popen = subprocess.Popen('elf\\build.bat',shell=True)
popen.wait()

print("Elf Builded!")
time.sleep(3)
