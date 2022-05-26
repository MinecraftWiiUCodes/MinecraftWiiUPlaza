import time
import pyperclip

sends=[]
with open("api-CodeEnablesSetter") as f:
    sends=f.readlines()
    sends.append('\n')
    f.close
with open("api-Resetter") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("api-GetHackItems") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("api-GetHackItems2") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("api-RainbowArmor") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("api-Nuker") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("main") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("api-ArgSetter") as f:
    sends=sends+f.readlines()
    sends.append('\n')
    f.close
with open("Run") as f:
    sends=sends+f.readlines()
    f.close

code=''
for x in range(len(sends)):
    code=code+sends[x]
pyperclip.copy(code)
print('Copied!')
time.sleep(3)
