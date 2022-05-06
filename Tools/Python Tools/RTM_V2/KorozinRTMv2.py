#===== Imports =====

import tkinter as tk					
from tkinter import ttk
from tkinter import *
import tkinter as tkinter
from TCP.tcpgecko import TCPGecko
from IP import IP_Addr
import sys
import os

#===== Imports End =====


#===== Main Window =====

window = Tk()
window.title('Korozin RTM V2')
window.geometry('655x150')
tabControl = ttk.Notebook(window)

tab1 = ttk.Frame(tabControl)
tab2 = ttk.Frame(tabControl)

tabControl.add(tab1, text ='Main')
tabControl.add(tab2, text ='Aura Codes')
tabControl.pack(expand = 1, fill ="both")

# Print Logo
print("""

   ██╗  ██╗ ██████╗ ██████╗  ██████╗ ███████╗██╗███╗   ██╗    ██████╗ ████████╗███╗   ███╗    ██╗   ██╗██████╗ 
   ██║ ██╔╝██╔═══██╗██╔══██╗██╔═══██╗╚══███╔╝██║████╗  ██║    ██╔══██╗╚══██╔══╝████╗ ████║    ██║   ██║╚════██╗
   █████╔╝ ██║   ██║██████╔╝██║   ██║  ███╔╝ ██║██╔██╗ ██║    ██████╔╝   ██║   ██╔████╔██║    ██║   ██║ █████╔╝
   ██╔═██╗ ██║   ██║██╔══██╗██║   ██║ ███╔╝  ██║██║╚██╗██║    ██╔══██╗   ██║   ██║╚██╔╝██║    ╚██╗ ██╔╝██╔═══╝ 
   ██║  ██╗╚██████╔╝██║  ██║╚██████╔╝███████╗██║██║ ╚████║    ██║  ██║   ██║   ██║ ╚═╝ ██║     ╚████╔╝ ███████╗
   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝╚═╝  ╚═══╝    ╚═╝  ╚═╝   ╚═╝   ╚═╝     ╚═╝      ╚═══╝  ╚══════╝             
""")

#===== Main Window End =====


#===== Section: Defined Functions =====

def armorHud():
    if cb.get() == 2:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02E9B1B0, 0x7FE4FB78)
        tcp.s.close()
        print("You can now see the armor hud like in mini-games!")
        
    elif cb.get() == 3:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02E9B1B0, 0x7FC4F378)
        tcp.s.close()
        print("Armor Hud Disabled")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def megaCode():
    if cb2.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02F88110, 0x39400002)
        tcp.pokemem(0x02FAF4F0, 0x39400002)
        tcp.pokemem(0x02FAF560, 0x39400002)
        tcp.pokemem(0x02FAF5DC, 0x39400002)
        tcp.pokemem(0x02FAF64C, 0x39400002)
        tcp.pokemem(0x030FA014, 0x2C090001)
        tcp.pokemem(0x030F9D38, 0xFC605018)
        tcp.pokemem(0x030F9D3C, 0xFC20A018)
        tcp.pokemem(0x030F9D40, 0xFC005018)
        tcp.pokemem(0x030F9D30, 0xFC405018)
        tcp.pokemem(0x030F9D10, 0xFD69482A)
        tcp.pokemem(0x030F9D20, 0xFD494828)
        tcp.pokemem(0x030F9C50, 0x7F24CB78)
        tcp.pokemem(0x11120008, 0x000006D5)
        tcp.pokemem(0x02D5731C, 0x38600001)
        tcp.pokemem(0x02D57320, 0x4E800020)
        tcp.pokemem(0x02F70970, 0x38600001)
        tcp.pokemem(0x032283CC, 0x38800000)
        tcp.pokemem(0x02F59534, 0x480002E8)
        tcp.pokemem(0x02DEC0B4, 0x38600001)
        tcp.pokemem(0x10997EA8, 0x30000000)
        tcp.pokemem(0x02E9B1B0, 0x7FE4FB78)
        tcp.pokemem(0x030FA0C8, 0x616500FF)
        tcp.pokemem(0x0384CD3C, 0x60000000)
        tcp.pokemem(0x0384CD40, 0x3D804100)
        tcp.pokemem(0x0384CD44, 0x618C0003)
        tcp.pokemem(0x0384CD48, 0x80AC0000)
        tcp.pokemem(0x0384CD4C, 0x4B8AD380)
        tcp.pokemem(0x108C7C2C, 0x60000000)
        tcp.pokemem(0x0205C430, 0x38600000)
        tcp.pokemem(0x031B2B4C, 0x38600001)
        tcp.pokemem(0x02F5C874, 0x38600001)
        tcp.pokemem(0x105DD948, 0x3F100000)
        tcp.s.close()
        print("Mega code is now activated!")
    elif cb2.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.pokemem(0x02F70970, 0x38600000)
        tcp.pokemem(0x032283CC, 0x38800001)
        tcp.pokemem(0x02F59534, 0x7C0802A6)
        tcp.pokemem(0x02DEC0B4, 0x57E3063E)
        tcp.pokemem(0x02F88110, 0x39400003)
        tcp.pokemem(0x02FAF4F0, 0x39400003)
        tcp.pokemem(0x02FAF560, 0x39400003)
        tcp.pokemem(0x02FAF5DC, 0x39400003)
        tcp.pokemem(0x02FAF64C, 0x39400003)
        tcp.pokemem(0x10997EA8, 0x3F000000)
        tcp.pokemem(0x02E9B1B0, 0x7FC4F378)
        tcp.pokemem(0x108C7C2C, 0x3CF5C28F)
        tcp.pokemem(0x0205C430, 0x3860FFFF)
        tcp.pokemem(0x030EA1C8, 0x6CC08000)
        tcp.pokemem(0x02F5C874, 0x38600000)
        tcp.pokemem(0x02D5731C, 0x7C0802A6)
        tcp.pokemem(0x02D57320, 0x9421FFF0)
        tcp.pokemem(0x030F9D38, 0xFC405018)
        tcp.pokemem(0x030F9D3C, 0xFC604018)
        tcp.pokemem(0x030F9D40, 0xFC205818)
        tcp.pokemem(0x030F9D30, 0xFD1D682A)
        tcp.pokemem(0x030F9D10, 0xFD6D682A)
        tcp.pokemem(0x030F9D20, 0xFD49582A)
        tcp.pokemem(0x030FA0C8, 0x38A5FFFF)
        tcp.pokemem(0x030FA014, 0x2C090000)
        tcp.pokemem(0x030F9C50, 0x388000FF)
        tcp.pokemem(0x108C9C20, 0x40080000)
        tcp.pokemem(0x0232F3A0, 0x38800000)
        tcp.s.close()
        print("Mega code is now disabled")
        canv = Tk()
        canv.title('Code Disabled!')
        canv.geometry('200x40')
        btn = Button(canv, text="                           OK!                           ", bd='5', bg="black",                      fg="white",command=canv.destroy)
        btn.pack(side='top')
        canv.mainloop()
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def craftAll():
    if cb3.get() == 3:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02F70970, 0x38600001)
        tcp.pokemem(0x032283CC, 0x38800001)
        tcp.pokemem(0x02F59534, 0x7C0802A6)
        tcp.s.close()
        print("Craft all now active!")
        
    elif cb3.get() == 4:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02F70970, 0x38600000)
        tcp.s.close()
        print("Disabling has not yet been implemented for this code")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def fly():
    if cb4.get() == 5:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0271AA74, 0x38600001)
        tcp.s.close()
        print("You can now fly!")
        
    elif cb4.get() == 6:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0271AA74, 0x38600000)
        tcp.s.close()
        print("Fly disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def FOFbypass():
    if cb5.get() == 7:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02D5731C, 0x38600001)
        tcp.pokemem(0x02D57320, 0x4E800020)
        tcp.s.close()
        print("You can now bypass Friends of Friends!")
        
    elif cb5.get() == 8:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02D5731C, 0x7C0802A6)
        tcp.pokemem(0x02D57320, 0x9421FFF0)
        tcp.s.close()
        print("FOF Disabled")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def host():
    if cb6.get() == 9:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02F17B30, 0x38807D00)
        tcp.s.close()
        print("You can now use host options anywhere!")
        
    elif cb6.get() == 10:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02D5731C, 0x7C0802A6)
        tcp.pokemem(0x02D57320, 0x9421FFF0)
        tcp.s.close()
        print("Host options disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def multiJump():
    if cb7.get() == 11:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0232F3A0, 0x38800001)
        tcp.s.close()
        print("Multi-Jump is now active!")
        
    elif cb7.get() == 12:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0232F3A0, 0x38800000)
        tcp.s.close()
        print("Multi-Jump disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def muteMic():
    if cb8.get() == 13:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x10997EA8, 0x30000000)
        tcp.s.close()
        print("Your mic is now muted!")
        
    elif cb8.get() == 14:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x10997EA8, 0x3F000000)
        tcp.s.close()
        print("Mute mic disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def reach():
    if cb9.get() == 15:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x108C9C20, 0x50090000)
        tcp.s.close()
        print("Reach = 3 blocks")
        
    elif cb9.get() == 16:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x108C9C20, 0x40080000)
        tcp.s.close()
        print("reach disabled")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def keyboard():
    if cb10.get() == 17:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02F88110, 0x39400002)
        tcp.pokemem(0x02FAF4F0, 0x39400002)
        tcp.pokemem(0x02FAF560, 0x39400002)
        tcp.pokemem(0x02FAF5DC, 0x39400002)
        tcp.pokemem(0x02FAF64C, 0x39400002)
        tcp.s.close()
        print("The entire keyboard is now unlocked!")
        
    elif cb10.get() == 18:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02F88110, 0x39400003)
        tcp.pokemem(0x02FAF4F0, 0x39400003)
        tcp.pokemem(0x02FAF560, 0x39400003)
        tcp.pokemem(0x02FAF5DC, 0x39400003)
        tcp.pokemem(0x02FAF64C, 0x39400003)
        tcp.s.close()
        print("Keyboard locked!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def FOV():
    if cb11.get() == 19:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x1088EDA8, 0x3F800000)
        tcp.s.close()
        print("FOV Enhanced")
        
    elif cb11.get() == 20:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x1088EDA8, 0x3F000000)
        tcp.s.close()
        print("FOV Reset")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def Hitbox():
    if cb12.get() == 21:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x030FA0C8, 0xFFFFFFFF)
        tcp.pokemem(0x030FA014, 0x2C090001)
        tcp.pokemem(0x030F9C50, 0xFFFFFFFF)
        tcp.s.close()
        print("Hitbox now shown!")
        
    elif cb12.get() == 22:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x030FA0C8, 0xFFFFFFFF)
        tcp.pokemem(0x030FA014, 0x2C090001)
        tcp.pokemem(0x030F9C50, 0xFFFFFFFF)
        tcp.s.close()
        print("Hitbox now shown!")
        canv = Tk()
        canv.title('Disabling for this code has not yet been added.')
        canv.geometry('200x40')
        btn = Button(canv, text="                           OK!                           ", bd='5', bg="black",                         fg="white",command=canv.destroy)
        btn.pack(side='top')
        canv.mainloop()
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def itemJava():
    if cb13.get() == 23:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0316760C, 0x60000000)
        tcp.pokemem(0x0316762C, 0xFC80F090)
        tcp.pokemem(0x03168DCC, 0xEC4F6BFA)
        tcp.pokemem(0x0384CEBC, 0x3D801002)
        tcp.pokemem(0x0384CEC0, 0x3C40433A)
        tcp.pokemem(0x0384CEC4, 0x904C0110)
        tcp.pokemem(0x0384CEC8, 0xC02C0110)
        tcp.pokemem(0x0384CECC, 0x4B91A770)
        tcp.pokemem(0x03167638, 0x486E5884)
        tcp.s.close()
        print("Item Drop animation has now been modified!")
        
    elif cb13.get() == 24:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0316760C, 0x60000000)
        tcp.pokemem(0x0316762C, 0xFC80F090)
        tcp.pokemem(0x03168DCC, 0xEC4F6BFA)
        tcp.pokemem(0x0384CEBC, 0x3D801002)
        tcp.pokemem(0x0384CEC0, 0x3C40433A)
        tcp.pokemem(0x0384CEC4, 0x904C0110)
        tcp.pokemem(0x0384CEC8, 0xC02C0110)
        tcp.pokemem(0x0384CECC, 0x4B91A770)
        tcp.pokemem(0x03167638, 0x486E5884)
        tcp.s.close()
        print("Disabling for this code has not yet been added.")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def offhand():
    if cb14.get() == 25:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x024FD7F4, 0x38600001)
        tcp.pokemem(0x0207F604, 0x38600001)
        tcp.s.close()
        print("All item slots have been unlocked! (eg offhand and body slots)")
        
    elif cb14.get() == 26:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x024FD7F4, 0x38600000)
        tcp.pokemem(0x0207F604, 0x38600000)
        tcp.s.close()
        print("Item slots back to normal")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def takeAll():
    if cb15.get() == 27:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02DEC0B4, 0x38600001)
        tcp.s.close()
        print("You can now take everything from chests!")
        
    elif cb15.get() == 28:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02DEC0B4, 0x57E3063E)
        tcp.s.close()
        print("You can no longer take everything")
        canv = Tk()
        canv.title('Code Disabled')
        canv.geometry('200x40')
        btn = Button(canv, text="                           OK!                           ", bd='5', bg="black",                         fg="white",command=canv.destroy)
        btn.pack(side='top')
        canv.mainloop()
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def noClip():
    if cb16.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0232E644, 0xFFFFFFFF)
        tcp.s.close()
        print("No-Clip is now active!")
        
    elif cb16.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0232E644, 0xFC20F890)
        tcp.s.close()
        print("No-Clip disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def riptidePunch():
    if cb17.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x031F5484, 0x88630390)
        tcp.s.close()
        print("Riptide Punch is now active!")
        
    elif cb17.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x031F5484, 0x88630A08)
        tcp.s.close()
        print("Riptide Punch disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def riptideAnywhere():
    if cb18.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0232C210, 0x38600001)
        tcp.s.close()
        print("Riptide Punch is now active!")
        
    elif cb18.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0232C210, 0x38600000)
        tcp.s.close()
        print("Riptide Punch disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def rodDMG():
    if cb19.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x10610AB8, 0x3F800000)
        tcp.s.close()
        print("Rods now do damage!")
        
    elif cb19.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x10610AB8, 0x3F800000)
        tcp.s.close()
        print("Rods no longer do damage!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def explosiveArrows():
    if cb20.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x020063F0, 0x487E23A4)
        tcp.s.close()
        print("Arrows and tridnets now explode!")
        
    elif cb20.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x020063F0, 0x7C0802A6)
        tcp.s.close()
        print("Arrows and tridnets no longer explode!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def Speed():
    if cb21.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x1066AAE8, 0x3F76F5C3)
        tcp.pokemem(0x1066879C, 0x3DF5C28F)
        tcp.pokemem(0x1066ACC8, 0x3EB9BD1F)
        tcp.s.close()
        print("You now have speed!")
        
    elif cb21.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x1066AAE8, 0x3F68F5C3)
        tcp.pokemem(0x1066879C, 0x3CA3D70A)
        tcp.pokemem(0x1066ACC8, 0x3E26AD89)
        tcp.s.close()
        print("You no longer have speed!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def antiKB():
    if cb22.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0257D85C, 0x4E800020)
        tcp.s.close()
        print("You no longer take KnockBack!")
        
    elif cb22.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x0257D85C, 0x9421FFA8)
        tcp.s.close()
        print("You now take KnockBack")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def allPerms():
    if cb22.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02C57E94, 0x38600001)
        tcp.pokemem(0x02C57E34, 0x38600001)
        tcp.pokemem(0x02C51C20, 0x38600001)
        tcp.pokemem(0x02C5CC84, 0x38600001)
        tcp.pokemem(0x02C57D74, 0x38600001)
        tcp.pokemem(0x02C57DD4, 0x38600001)
        tcp.s.close()
        print("All perms now activated!")
        
    elif cb22.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02C57E94, 0x57E3063E)
        tcp.pokemem(0x02C57E34, 0x57E3063E)
        tcp.pokemem(0x02C51C20, 0x57E3063E)
        tcp.pokemem(0x02C5CC84, 0x88630124)
        tcp.pokemem(0x02C57D74, 0x57E3063E)
        tcp.pokemem(0x02C57DD4, 0x57E3063E)
        tcp.s.close()
        print("You no longer have all perms!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def fullBright():
    if cb24.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x108C7C2C, 0x60000000)
        tcp.s.close()
        print("You can now see in the dark!")
        
    elif cb24.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x108C7C2C, 0x3CF5C28F)
        tcp.s.close()
        print("Sight set back to normal")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lockServer():
    if cb25.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02D5B28C, 0x3BC00001)
        tcp.s.close()
        print("Your server is now locked!")
        
    elif cb25.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x02D5B28C, 0x3BC00008)
        tcp.s.close()
        print("Server unlocked!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
#===== Section: Defined Functions End =====

#===== Section: Tab 2 Defined Function ====

def lvl1():
    if cg.get() == 1:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F100000)
        tcp.s.close()
        print("Aura Lvl 1 is now active")
        
    elif cg.get() == 0:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl2():
    if cg2.get() == 2:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F200000)
        tcp.s.close()
        print("Aura Lvl 2 is now active")
        
    elif cg2.get() == 3:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl3():
    if cg3.get() == 4:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F300000)
        tcp.s.close()
        print("Aura Lvl 3 is now active")
        
    elif cg3.get() == 5:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl4():
    if cg4.get() == 6:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F400000)
        tcp.s.close()
        print("Aura Lvl 4 is now active")
        
    elif cg4.get() == 7:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl5():
    if cg5.get() == 8:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F500000)
        tcp.s.close()
        print("Aura Lvl 5 is now active")
        
    elif cg5.get() == 9:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl6():
    if cg6.get() == 10:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F600000)
        tcp.s.close()
        print("Aura Lvl 6 is now active")
        
    elif cg6.get() == 11:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl7():
    if cg7.get() == 12:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F700000)
        tcp.s.close()
        print("Aura Lvl 7 is now active")
        
    elif cg7.get() == 13:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl8():
    if cg8.get() == 14:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F800000)
        tcp.s.close()
        print("Aura Lvl 8 is now active")
        
    elif cg8.get() == 15:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl9():
    if cg9.get() == 16:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3FF00000)
        tcp.s.close()
        print("In the words of 9 year olds: You're a hackowr")
        
    elif cg9.get() == 17:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl10():
    if cg10.get() == 18:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x41099999)
        tcp.s.close()
        print("I think you just became god")
        
    elif cg10.get() == 19:
        tcp = TCPGecko(IP_Addr)
        tcp.pokemem(0x105DD948, 0x3F000000)
        tcp.s.close()
        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)

#===== Section Tab 2 Defined Functions End ====


#===== Section: Tab 1 =====

cb = IntVar()
cb2 = IntVar()
cb3 = IntVar()
cb4 = IntVar()
cb5 = IntVar()
cb6 = IntVar()
cb7 = IntVar()
cb8 = IntVar()
cb9 = IntVar()
cb10 = IntVar()
cb11 = IntVar()
cb12 = IntVar()
cb13 = IntVar()
cb14 = IntVar()
cb15 = IntVar()
cb16 = IntVar()
cb17 = IntVar()
cb18 = IntVar()
cb19 = IntVar()
cb20 = IntVar()
cb21 = IntVar()
cb22 = IntVar()
cb23 = IntVar()
cb24 = IntVar()
cb25 = IntVar()

c = Checkbutton(tab1, text="Mega-Code", variable=cb2, onvalue=1, offvalue=0, command=megaCode)
c.grid(column=0, row=0)

c = Checkbutton(tab1, text="Armor Hud", variable=cb, onvalue=2, offvalue=3, command=armorHud)
c.grid(column=1, row=0)

c = Checkbutton(tab1, text="Craft All", variable=cb3, onvalue=3, offvalue=4, command=craftAll)
c.grid(column=2, row=0)

c = Checkbutton(tab1, text="Fly", variable=cb4, onvalue=5, offvalue=6, command=fly)
c.grid(column=3, row=0)

c = Checkbutton(tab1, text="FOF Bypass", variable=cb5, onvalue=7, offvalue=8, command=FOFbypass)
c.grid(column=4, row=0)

c = Checkbutton(tab1, text="Host Options", variable=cb6, onvalue=9, offvalue=10, command=host)
c.grid(column=0, row=1)

c = Checkbutton(tab1, text="Multi-Jump", variable=cb7, onvalue=11, offvalue=12, command=multiJump)
c.grid(column=1, row=1)

c = Checkbutton(tab1, text="Mute Mic", variable=cb8, onvalue=13, offvalue=14, command=muteMic)
c.grid(column=2, row=1)

c = Checkbutton(tab1, text="Reach", variable=cb9, onvalue=15, offvalue=16, command=reach)
c.grid(column=3, row=1)

c = Checkbutton(tab1, text="Unlock Keyboard", variable=cb10, onvalue=17, offvalue=18, command=keyboard)
c.grid(column=4, row=1)

c = Checkbutton(tab1, text="FOV", variable=cb11, onvalue=19, offvalue=20, command=FOV)
c.grid(column=0, row=2)

c = Checkbutton(tab1, text="Draw Hitbox", variable=cb12, onvalue=21, offvalue=22, command=Hitbox)
c.grid(column=1, row=2)

c = Checkbutton(tab1, text="Item Drop", variable=cb13, onvalue=23, offvalue=24, command=itemJava)
c.grid(column=2, row=2)

c = Checkbutton(tab1, text="Unlock Offhand", variable=cb14, onvalue=25, offvalue=26, command=offhand)
c.grid(column=3, row=2)

c = Checkbutton(tab1, text="Take All", variable=cb15, onvalue=27, offvalue=28, command=takeAll)
c.grid(column=4, row=2)

c = Checkbutton(tab1, text="No-Clip", variable=cb16, onvalue=1, offvalue=0, command=noClip)
c.grid(column=0, row=3)

c = Checkbutton(tab1, text="Riptide Punch", variable=cb17, onvalue=1, offvalue=0, command=riptidePunch)
c.grid(column=1, row=3)

c = Checkbutton(tab1, text="Riptide Anywhere", variable=cb18, onvalue=1, offvalue=0, command=riptideAnywhere)
c.grid(column=2, row=3)

c = Checkbutton(tab1, text="Rods Do Damage", variable=cb19, onvalue=1, offvalue=0, command=rodDMG)
c.grid(column=3, row=3)

c = Checkbutton(tab1, text="Arrows Explode", variable=cb20, onvalue=1, offvalue=0, command=explosiveArrows)
c.grid(column=4, row=3)

c = Checkbutton(tab1, text="Speed", variable=cb21, onvalue=1, offvalue=0, command=Speed)
c.grid(column=0, row=4)

c = Checkbutton(tab1, text="Anti-KB", variable=cb22, onvalue=1, offvalue=0, command=antiKB)
c.grid(column=1, row=4)

c = Checkbutton(tab1, text="All Perms", variable=cb23, onvalue=1, offvalue=0, command=allPerms)
c.grid(column=2, row=4)

c = Checkbutton(tab1, text="Full Bright", variable=cb24, onvalue=1, offvalue=0, command=fullBright)
c.grid(column=3, row=4)

c = Checkbutton(tab1, text="Lock Server", variable=cb25, onvalue=1, offvalue=0, command=lockServer)
c.grid(column=4, row=4)

#===== Section: Tab 1 End =====


#===== Section: Tab 2 =====

cg = IntVar()
cg2 = IntVar()
cg3 = IntVar()
cg4 = IntVar()
cg5 = IntVar()
cg6 = IntVar()
cg7 = IntVar()
cg8 = IntVar()
cg9 = IntVar()
cg10 = IntVar()

c = Checkbutton(tab2, text="Level One", variable=cg, onvalue=1, offvalue=0, command=lvl1)
c.grid(column=0, row=0)

c = Checkbutton(tab2, text="Level Two", variable=cg2, onvalue=2, offvalue=3, command=lvl2)
c.grid(column=1, row=0)

c = Checkbutton(tab2, text="Level Three", variable=cg3, onvalue=4, offvalue=5, command=lvl3)
c.grid(column=2, row=0)

c = Checkbutton(tab2, text="Level Four", variable=cg4, onvalue=6, offvalue=7, command=lvl4)
c.grid(column=3, row=0)

c = Checkbutton(tab2, text="Level Five", variable=cg5, onvalue=8, offvalue=9, command=lvl5)
c.grid(column=4, row=0)

c = Checkbutton(tab2, text="Level Six", variable=cg6, onvalue=10, offvalue=11, command=lvl6)
c.grid(column=0, row=1)

c = Checkbutton(tab2, text="Level Seven", variable=cg7, onvalue=12, offvalue=13, command=lvl7)
c.grid(column=1, row=1)

c = Checkbutton(tab2, text="Level Eight", variable=cg8, onvalue=14, offvalue=15, command=lvl8)
c.grid(column=2, row=1)

c = Checkbutton(tab2, text="Blatamt", variable=cg9, onvalue=16, offvalue=17, command=lvl9)
c.grid(column=3, row=1)

c = Checkbutton(tab2, text="Absurdly Blatamt", variable=cg10, onvalue=18, offvalue=19, command=lvl10)
c.grid(column=4, row=1)

#===== Section: Tab 2 End =====

window.mainloop()
