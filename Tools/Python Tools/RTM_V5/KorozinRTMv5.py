#===== Imports =====

import tkinter as tk					
from tkinter import ttk
from tkinter import *
import tkinter as tkinter
import sys
import os
import struct
import time
import os.path
import socket, sys
from threading import Thread, RLock
import py_compile
import atexit
import webbrowser
config_exist = os.path.isfile("ip.config") 

#===== Imports End =====


#===== TCP Gecko Module ====

import socket, struct
from binascii import hexlify, unhexlify

def enum(**enums):
    return type('Enum', (), enums)

class TCPGecko:
    def __init__(self, *args):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        print("Connecting to " + str(args[0]) + ":7331")
        self.s.connect((str(args[0]), 7331)) #IP, 1337 reversed, Cafiine uses 7332+
        print("Connected!")

    def readmem(self, address, length): #Number of bytes
        if length == 0: raise BaseException("Reading memory requires a length (# of bytes)")
        if not self.validrange(address, length): raise BaseException("Address range not valid")
        if not self.validaccess(address, length, "read"): raise BaseException("Cannot read from address")
        ret = b""
        if length > 0x400:
            print("Length is greater than 0x400 bytes, need to read in chunks")
            print("Start address:   " + hexstr0(address))
            for i in range(int(length / 0x400)): #Number of blocks, ignores extra
                self.s.send(b"\x04") #cmd_readmem
                request = struct.pack(">II", address, address + 0x400)
                self.s.send(request)
                status = self.s.recv(1)
                if   status == b"\xbd": ret += self.s.recv(0x400)
                elif status == b"\xb0": ret += b"\x00" * 0x400
                else: raise BaseException("Something went terribly wrong")
                address += 0x400;length -= 0x400
                print("Current address: " + hexstr0(address))
            if length != 0: #Now read the last little bit
                self.s.send(b"\x04")
                request = struct.pack(">II", address, address + length)
                self.s.send(request)
                status = self.s.recv(1)
                if   status == b"\xbd": ret += self.s.recv(length)
                elif status == b"\xb0": ret += b"\x00" * length
                else: raise BaseException("Something went terribly wrong")
            print("Finished!")
        else:
            self.s.send(b"\x04")
            request = struct.pack(">II", address, address + length)
            self.s.send(request)
            status = self.s.recv(1)
            if   status == b"\xbd": ret += self.s.recv(length)
            elif status == b"\xb0": ret += b"\x00" * length
            else: raise BaseException("Something went terribly wrong")
        return ret

    def readkern(self, address): #Only takes 4 bytes, may need to run multiple times
        if not self.validrange(address, 4): raise BaseException("Address range not valid")
        if not self.validaccess(address, 4, "write"): raise BaseException("Cannot write to address")
        self.s.send(b"\x0C") #cmd_readkern
        request = struct.pack(">I", int(address))
        self.s.send(request)
        value  = struct.unpack(">I", self.s.recv(4))[0]
        return value

    def writekern(self, address, value): #Only takes 4 bytes, may need to run multiple times
        if not self.validrange(address, 4): raise BaseException("Address range not valid")
        if not self.validaccess(address, 4, "write"): raise BaseException("Cannot write to address")
        self.s.send(b"\x0B") #cmd_readkern
        print(value)
        request = struct.pack(">II", int(address), int(value))
        self.s.send(request)
        return

    def pokemem(self, address, value): #Only takes 4 bytes, may need to run multiple times
        if not self.validrange(address, 4): raise BaseException("Address range not valid")
        if not self.validaccess(address, 4, "write"): raise BaseException("Cannot write to address")
        self.s.send(b"\x03") #cmd_pokemem
        request = struct.pack(">II", int(address), int(value))
        self.s.send(request) #Done, move on
        return
		
    def pokemem8(self, address, value): #Only takes 4 bytes, may need to run multiple times
        if not self.validrange(address, 4): raise BaseException("Address range not valid")
        if not self.validaccess(address, 4, "write"): raise BaseException("Cannot write to address")
        self.s.send(b"\x03") #cmd_pokemem
        request = struct.pack(">IB", int(address), int(value))
        self.s.send(request) #Done, move on
        return

    def search32(self, address, value, size):
        self.s.send(b"\x72") #cmd_search32
        request = struct.pack(">III", address, value, size)
        self.s.send(request)
        reply = self.s.recv(4)
        return struct.unpack(">I", reply)[0]

    def getversion(self):
        self.s.send(b"\x9A") #cmd_os_version
        reply = self.s.recv(4)
        return struct.unpack(">I", reply)[0]

    def writestr(self, address, string):
        if not self.validrange(address, len(string)): raise BaseException("Address range not valid")
        if not self.validaccess(address, len(string), "write"): raise BaseException("Cannot write to address")
        if type(string) != bytes: string = bytes(string, "UTF-8") #Sanitize
        if len(string) % 4: string += bytes((4 - (len(string) % 4)) * b"\x00")
        pos = 0
        for x in range(int(len(string) / 4)):
            self.pokemem(address, struct.unpack(">I", string[pos:pos + 4])[0])
            address += 4;pos += 4
        return
        
    def memalign(self, size, align):
        symbol = self.get_symbol("coreinit.rpl", "MEMAllocFromDefaultHeapEx", True, 1)
        symbol = struct.unpack(">I", symbol.address)[0]
        address = self.readmem(symbol, 4)
        #print("memalign address: " + hexstr0(struct.unpack(">I", address)[0]))
        ret = self.call(address, size, align)
        return ret

    def freemem(self, address):
        symbol = self.get_symbol("coreinit.rpl", "MEMFreeToDefaultHeap", True, 1)
        symbol = struct.unpack(">I", symbol.address)[0]
        addr = self.readmem(symbol, 4)
        #print("freemem address: " + hexstr0(struct.unpack(">I", addr)[0]))
        self.call(addr, address) #void, no return

    def memalloc(self, size, align, noprint=False):
        return self.function("coreinit.rpl", "OSAllocFromSystem", noprint, 0, size, align)

    def freealloc(self, address):
        return self.function("coreinit.rpl", "OSFreeToSystem", True, 0, address)

    def createpath(self, path):
        if not hasattr(self, "pPath"): self.pPath = self.memalloc(len(path), 0x20, True) #It'll auto-pad
        size = len(path) + (32 - (len(path) % 32))
        self.function("coreinit.rpl", "memset", True, 0, self.pPath, 0x00, size)
        self.writestr(self.pPath, path)
        #print("pPath address: " + hexstr0(self.pPath))

    def createstr(self, string):
        address = self.memalloc(len(string), 0x20, True) #It'll auto-pad
        size = len(string) + (32 - (len(string) % 32))
        self.function("coreinit.rpl", "memset", True, 0, address, 0x00, size)
        self.writestr(address, string)
        print("String address: " + hexstr0(address))
        return address

    def FSInitClient(self):
        self.pClient = self.memalign(0x1700, 0x20)
        self.function("coreinit.rpl", "FSAddClient", True, 0, self.pClient)
        #print("pClient address: " + hexstr0(self.pClient))

    def FSInitCmdBlock(self):
        self.pCmd = self.memalign(0xA80, 0x20)
        self.function("coreinit.rpl", "FSInitCmdBlock", True, 0, self.pCmd)
        #print("pCmd address:    " + hexstr0(self.pCmd))

    def FSOpenDir(self, path="/"):
        print("Initializing...")
        self.function("coreinit.rpl",  "FSInit", True)
        if not hasattr(self, "pClient"): self.FSInitClient()
        if not hasattr(self, "pCmd"):    self.FSInitCmdBlock()
        print("Getting memory ready...")
        self.createpath(path)
        self.pDh   = self.memalloc(4, 4, True)
        #print("pDh address: " + hexstr0(self.pDh))
        print("Calling function...")
        ret = self.function("coreinit.rpl", "FSOpenDir", False, 0, self.pClient, self.pCmd, self.pPath, self.pDh, 0xFFFFFFFF)
        self.pDh = int(hexlify(self.readmem(self.pDh, 4)), 16)
        print("Return value: " + hexstr0(ret))

    def SAVEOpenDir(self, path="/", slot=255):
        print("Initializing...")
        self.function("coreinit.rpl",  "FSInit", True, 0)
        self.function("nn_save.rpl", "SAVEInit", True, 0, slot)
        print("Getting memory ready...")
        if not hasattr(self, "pClient"): self.FSInitClient()
        if not hasattr(self, "pCmd"):    self.FSInitCmdBlock()
        self.createpath(path)
        self.pDh   = self.memalloc(4, 4, True)
        #print("pDh address: " + hexstr0(self.pDh))
        print("Calling function...")
        ret = self.function("nn_save.rpl", "SAVEOpenDir", False, 0, self.pClient, self.pCmd, slot, self.pPath, self.pDh, 0xFFFFFFFF)
        self.pDh = int(hexlify(self.readmem(self.pDh, 4)), 16)
        print("Return value: " + hexstr0(ret))

    def FSReadDir(self):
        global printe
        if not hasattr(self, "pBuffer"): self.pBuffer = self.memalign(0x164, 0x20)
        print("pBuffer address: " + hexstr0(self.pBuffer))
        ret = self.function("coreinit.rpl", "FSReadDir", True, 0, self.pClient, self.pCmd, self.pDh, self.pBuffer, 0xFFFFFFFF)
        self.entry = self.readmem(self.pBuffer, 0x164)
        printe = getstr(self.entry, 100) + " "
        self.FileSystem().printflags(uint32(self.entry, 0), self.entry)
        self.FileSystem().printperms(uint32(self.entry, 4))
        print(printe)
        return self.entry, ret

    def SAVEOpenFile(self, path="/", mode="r", slot=255):
        print("Initializing...")
        self.function("coreinit.rpl",  "FSInit", True)
        self.function("nn_save.rpl", "SAVEInit", slot, True)
        print("Getting memory ready...")
        if not hasattr(self, "pClient"): self.FSInitClient()
        if not hasattr(self, "pCmd"):    self.FSInitCmdBlock()
        self.createpath(path)
        self.pMode = self.createstr(mode)
        self.pFh   = self.memalign(4, 4)
        #print("pFh address: " + hexstr0(self.pFh))
        print("Calling function...")
        print("This function may have errors")
        #ret = self.function("nn_save.rpl", "SAVEOpenFile", self.pClient, self.pCmd, slot, self.pPath, self.pMode, self.pFh, 0xFFFFFFFF)
        #self.pFh = int(self.readmem(self.pFh, 4).encode("hex"), 16)
        #print(ret)

    def FSReadFile(self):
        if not hasattr(self, "pBuffer"): self.pBuffer = self.memalign(0x200, 0x20)
        print("pBuffer address: " + hexstr0(self.pBuffer))
        ret = self.function("coreinit.rpl", "FSReadFile", False, 0, self.pClient, self.pCmd, self.pBuffer, 1, 0x200, self.pFh, 0, 0xFFFFFFFF)
        print(ret)
        return tcp.readmem(self.pBuffer, 0x200)

    def get_symbol(self, rplname, symname, noprint=False, data=0):
        self.s.send(b"\x71") #cmd_getsymbol
        request = struct.pack(">II", 8, 8 + len(rplname) + 1) #Pointers
        request += rplname.encode("UTF-8") + b"\x00"
        request += symname.encode("UTF-8") + b"\x00"
        size = struct.pack(">B", len(request))
        data = struct.pack(">B", data)
        self.s.send(size) #Read this many bytes
        self.s.send(request) #Get this symbol
        self.s.send(data) #Is it data?
        address = self.s.recv(4)
        return ExportedSymbol(address, self, rplname, symname, noprint)

    def call(self, address, *args):
        arguments = list(args)
        if len(arguments)>8 and len(arguments)<=16: #Use the big call function
            while len(arguments) != 16:
                arguments.append(0)
            self.s.send(b"\x80")
            address = struct.unpack(">I", address)[0]
            request = struct.pack(">I16I", address, *arguments)
            self.s.send(request)
            reply = self.s.recv(8)
            return struct.unpack(">I", reply[:4])[0]
        elif len(arguments) <= 8: #Use the normal one that dNet client uses
            while len(arguments) != 8:
                arguments.append(0)
            self.s.send(b"\x70")
            address = struct.unpack(">I", address)[0]
            request = struct.pack(">I8I", address, *arguments)
            self.s.send(request)
            reply = self.s.recv(8)
            return struct.unpack(">I", reply[:4])[0]
        else: raise BaseException("Too many arguments!")

    #Data last, only a few functions need it, noprint for the big FS/SAVE ones above, acts as gateway for data arg
    def function(self, rplname, symname, noprint=False, data=0, *args):
        symbol = self.get_symbol(rplname, symname, noprint, data)
        ret = self.call(symbol.address, *args)
        return ret

    def validrange(self, address, length):
        if   0x01000000 <= address and address + length <= 0x01800000: return True
        elif 0x02000000 <= address and address + length <= 0x10000000: return True #Depends on game
        elif 0x10000000 <= address and address + length <= 0x50000000: return True #Doesn't quite go to 5
        elif 0xE0000000 <= address and address + length <= 0xE4000000: return True
        elif 0xE8000000 <= address and address + length <= 0xEA000000: return True
        elif 0xF4000000 <= address and address + length <= 0xF6000000: return True
        elif 0xF6000000 <= address and address + length <= 0xF6800000: return True
        elif 0xF8000000 <= address and address + length <= 0xFB000000: return True
        elif 0xFB000000 <= address and address + length <= 0xFB800000: return True
        elif 0xFFFE0000 <= address and address + length <= 0xFFFFFFFF: return True
        else: return True

    def validaccess(self, address, length, access):
        if   0x01000000 <= address and address + length <= 0x01800000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0x02000000 <= address and address + length <= 0x10000000: #Depends on game, may be EG 0x0E3
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0x10000000 <= address and address + length <= 0x50000000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xE0000000 <= address and address + length <= 0xE4000000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xE8000000 <= address and address + length <= 0xEA000000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xF4000000 <= address and address + length <= 0xF6000000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xF6000000 <= address and address + length <= 0xF6800000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xF8000000 <= address and address + length <= 0xFB000000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xFB000000 <= address and address + length <= 0xFB800000:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        elif 0xFFFE0000 <= address and address + length <= 0xFFFFFFFF:
            if access.lower() == "read":  return True
            if access.lower() == "write": return True
        else: return False
        
    class FileSystem: #TODO: Try to clean this up ????
        Flags = enum(
            IS_DIRECTORY    = 0x80000000,
            IS_QUOTA        = 0x40000000,
            SPRT_QUOTA_SIZE = 0x20000000, #Supports .quota_size field
            SPRT_ENT_ID     = 0x10000000, #Supports .ent_id field
            SPRT_CTIME      = 0x08000000, #Supports .ctime field
            SPRT_MTIME      = 0x04000000, #Supports .mtime field
            SPRT_ATTRIBUTES = 0x02000000, #Supports .attributes field
            SPRT_ALLOC_SIZE = 0x01000000, #Supports .alloc_size field
            IS_RAW_FILE     = 0x00800000, #Entry isn't encrypted
            SPRT_DIR_SIZE   = 0x00100000, #Supports .size field, doesn't apply to files
            UNSUPPORTED_CHR = 0x00080000) #Entry name has an unsupported character
        
        Permissions = enum( #Pretty self explanitory
            OWNER_READ  = 0x00004000,
            OWNER_WRITE = 0x00002000,
            OTHER_READ  = 0x00000400,
            OTHER_WRITE = 0x00000200)

        def printflags(self, flags, data):
            global printe
            if flags & self.Flags.IS_DIRECTORY:    printe += " Directory"
            if flags & self.Flags.IS_QUOTA:        printe += " Quota"
            if flags & self.Flags.SPRT_QUOTA_SIZE: printe += " .quota_size: " + hexstr0(uint32(data, 24))
            if flags & self.Flags.SPRT_ENT_ID:     printe += " .ent_id: " + hexstr0(uint32(data, 32))
            if flags & self.Flags.SPRT_CTIME:      printe += " .ctime: " + hexstr0(uint32(data, 36))
            if flags & self.Flags.SPRT_MTIME:      printe += " .mtime: " + hexstr0(uint32(data, 44))
            if flags & self.Flags.SPRT_ATTRIBUTES: pass #weh
            if flags & self.Flags.SPRT_ALLOC_SIZE: printe += " .alloc_size: " + hexstr0(uint32(data, 20))
            if flags & self.Flags.IS_RAW_FILE:     printe += " Raw (Unencrypted) file"
            if flags & self.Flags.SPRT_DIR_SIZE:   printe += " .dir_size: " + hexstr0(uint64(data, 24))
            if flags & self.Flags.UNSUPPORTED_CHR: printe += " !! UNSUPPORTED CHARACTER IN NAME"

        def printperms(self, perms):
            global printe
            if perms & self.Permissions.OWNER_READ:  printe += " OWNER_READ"
            if perms & self.Permissions.OWNER_WRITE: printe += " OWNER_WRITE"
            if perms & self.Permissions.OTHER_READ:  printe += " OTHER_READ"
            if perms & self.Permissions.OTHER_WRITE: printe += " OTHER_WRITE"
                
def hexstr0(data): #0xFFFFFFFF, uppercase hex string
    return "0x" + hex(data).lstrip("0x").rstrip("L").zfill(8).upper()

class ExportedSymbol(object):
    def __init__(self, address, rpc=None, rplname=None, symname=None, noprint=False):
        self.address = address
        self.rpc     = rpc
        self.rplname = rplname
        self.symname = symname
        if not noprint: #Make command prompt not explode when using FS or SAVE functions
            print(symname + " address: " + hexstr0(struct.unpack(">I", address)[0]))

    def __call__(self, *args):
        return self.rpc.call(self.address, *args) #Pass in arguments, run address
        
class switch(object): #Taken from http://code.activestate.com/recipes/410692/
    def __init__(self, value):
        self.value = value
        self.fall = False

    def __iter__(self):
        """Return the match method once, then stop"""
        yield self.match
        raise StopIteration
    
    def match(self, *args):
        """Indicate whether or not to enter a case suite"""
        if self.fall or not args:
            return True
        elif self.value in args:
            self.fall = True
            return True
        else:
            return False
'''Example Use Case for switch:
for case in switch(variable):
    if case(0):
        #dostuff
    elif case(1):
        #dostuff
    else: #default
        #dodefaultstuff'''

def hexstr(data, length): #Pad hex to value for prettyprint
    return hex(data).lstrip("0x").rstrip("L").zfill(length).upper()
def hexstr0(data): #Uppercase hex to string
    return "0x" + hex(data).lstrip("0x").rstrip("L").upper()
def binr(byte): #Get bits as a string
    return bin(byte).lstrip("0b").zfill(8)
def uint8(data, pos):
    return struct.unpack(">B", data[pos:pos + 1])[0]
def uint16(data, pos):
    return struct.unpack(">H", data[pos:pos + 2])[0]
def uint24(data, pos):
    return struct.unpack(">I", "\00" + data[pos:pos + 3])[0] #HAX
def uint32(data, pos):
    return struct.unpack(">I", data[pos:pos + 4])[0]

def getstr(data, pos): #Keep incrementing till you hit a stop
    string = ""
    while data[pos] != 0:
        if pos != len(data):
            string += chr(data[pos])
            pos += 1
        else: break
    return string


#===== TCP Gecko Module End ====

#===== Main Window =====

s1_ptr = 0x112395AC
s2_ptr = 0x112395B4
s3_ptr = 0x112395B0
name_ptr = 0x121F2C94

freemem = 0x3F000000

def str_end(string, ind):
	for i in range(0, 0xFFFF):
		x = string[ind+i:ind+i+1:1]
		if x == "\x00":
			return string[ind:ind+i:1]

window = Tk()
window.title('Korozin RTM V5')
window.geometry('655x150')
tabControl = ttk.Notebook(window)

tab1 = ttk.Frame(tabControl)
tab2 = ttk.Frame(tabControl)
tab3 = ttk.Frame(tabControl)

tabControl.add(tab1, text ='Connect')
tabControl.add(tab2, text ='Main')
tabControl.add(tab3, text ='Aura Codes')
tabControl.grid(columnspan = 1, rowspan = 1)

# Print Logo
print("""
██╗  ██╗ ██████╗ ██████╗  ██████╗ ███████╗██╗███╗   ██╗    ██████╗ ████████╗███╗   ███╗    ██╗   ██╗███████╗
██║ ██╔╝██╔═══██╗██╔══██╗██╔═══██╗╚══███╔╝██║████╗  ██║    ██╔══██╗╚══██╔══╝████╗ ████║    ██║   ██║██╔════╝
█████╔╝ ██║   ██║██████╔╝██║   ██║  ███╔╝ ██║██╔██╗ ██║    ██████╔╝   ██║   ██╔████╔██║    ██║   ██║███████╗
██╔═██╗ ██║   ██║██╔══██╗██║   ██║ ███╔╝  ██║██║╚██╗██║    ██╔══██╗   ██║   ██║╚██╔╝██║    ╚██╗ ██╔╝╚════██║
██║  ██╗╚██████╔╝██║  ██║╚██████╔╝███████╗██║██║ ╚████║    ██║  ██║   ██║   ██║ ╚═╝ ██║     ╚████╔╝ ███████║
╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝╚═╝  ╚═══╝    ╚═╝  ╚═╝   ╚═╝   ╚═╝     ╚═╝      ╚═══╝  ╚══════╝        
""")

#===== Main Window End =====

#===== Section: Tab 1 =====

verrou = RLock()

def connect():
	ip = nip.get()
	global tcp, f_config
	tcp = TCPGecko(ip)
	x = tcp.readmem(name_ptr, 20)
	x = str_end(x, 0)
	tcp.pokemem(0x1076f7a8, 0x000000FF)
	temp_vars = []
	f_config.seek(0, 0)
	f_config.write(ip)
	f_config.close()
	
def disc():
	with verrou:
		global temp_vars
		temp_vars = []
		tcp.s.close()
		print("Disconnected.")
		
blank = Label(tab1, text="Connection")
blank.grid(row=0, column = 0)

if config_exist == False:
	nip = StringVar()
	nip.set("Wii U IP Addr")
	f_config = open("ip.config", "a+")
else:
	nip = StringVar()
	f_config = open("ip.config", "r+")
	nip.set(f_config.read())

n_ip = Entry(tab1, textvariable=nip)
n_ip.grid(row=1, column=0)

cnn = Button(tab1, text="Connect", command=connect)
cnn.grid(row=1, column=1)

b_disconnect = Button(tab1, text="Disconnect", command=disc)
b_disconnect.grid(row=1, column=2)

#===== Section: Tab 1 End =====

#===== Section: Defined Functions =====

def armorHud():
    if cb.get() == 2:

        tcp.pokemem(0x02E9B1B0, 0x7FE4FB78)

        print("You can now see the armor hud like in mini-games!")
        
    elif cb.get() == 3:

        tcp.pokemem(0x02E9B1B0, 0x7FC4F378)

        print("Armor Hud Disabled")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def megaCode():
    if cb2.get() == 1:

        tcp.pokemem(0x030FA0C8, 0x616500FF)
        tcp.pokemem(0x030FA014, 0x2C090001)
        tcp.pokemem(0x030F9C50, 0x7F24CB78)
        tcp.pokemem(0x030F9D38, 0xFC605018)
        tcp.pokemem(0x030F9D3C, 0xFC20A018)
        tcp.pokemem(0x030F9D40, 0xFC005018)
        tcp.pokemem(0x030F9D30, 0xFC405018)
        tcp.pokemem(0x030F9D10, 0xFD69482A)
        tcp.pokemem(0x030F9D20, 0xFD494828)
        tcp.pokemem(0x030FA0C8, 0x60000000)
        tcp.pokemem(0x030F9C50, 0x7F24CB78)
        tcp.pokemem(0x11120008, 0x000006D5)
        tcp.pokemem(0x02D5731C, 0x38600001)
        tcp.pokemem(0x02D57320, 0x4E800020)
        tcp.pokemem(0x02F70970, 0x38600001)
        tcp.pokemem(0x032283CC, 0x38800000)
        tcp.pokemem(0x02F59534, 0x480002E8)
        tcp.pokemem(0x02DEC0B4, 0x38600001)
        tcp.pokemem(0x02F88110, 0x39400002)
        tcp.pokemem(0x02FAF4F0, 0x39400002)
        tcp.pokemem(0x02FAF560, 0x39400002)
        tcp.pokemem(0x02FAF5DC, 0x39400002)
        tcp.pokemem(0x02FAF64C, 0x39400002)
        tcp.pokemem(0x02E9B1B0, 0x7FE4FB78)
        tcp.pokemem(0x030FA0C8, 0x48752C74)
        tcp.pokemem(0x0384CD3C, 0x60000000)
        tcp.pokemem(0x0384CD40, 0x3D804100)
        tcp.pokemem(0x0384CD44, 0x618C0003)
        tcp.pokemem(0x0384CD48, 0x80AC0000)
        tcp.pokemem(0x0384CD4C, 0x4B8AD380)
        tcp.pokemem(0x108C7C2C, 0x60000000)
        tcp.pokemem(0x0205C430, 0x38600000)
        tcp.pokemem(0x031B2B4C, 0x38600001)
        tcp.pokemem(0x02F5C874, 0x38600001)

        print("Mega code is now activated!")
    elif cb2.get() == 0:

        tcp.pokemem(0x02F70970, 0x38600000)
        tcp.pokemem(0x032283CC, 0x38800001)
        tcp.pokemem(0x02F59534, 0x7C0802A6)
        tcp.pokemem(0x02DEC0B4, 0x57E3063E)
        tcp.pokemem(0x02F88110, 0x39400003)
        tcp.pokemem(0x02FAF4F0, 0x39400003)
        tcp.pokemem(0x02FAF560, 0x39400003)
        tcp.pokemem(0x02FAF5DC, 0x39400003)
        tcp.pokemem(0x02FAF64C, 0x39400003)
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

        print("Mega code is now disabled!")
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

        tcp.pokemem(0x02F70970, 0x38600001)
        tcp.pokemem(0x032283CC, 0x38800001)
        tcp.pokemem(0x02F59534, 0x7C0802A6)

        print("Craft all now active!")
        
    elif cb3.get() == 4:

        tcp.pokemem(0x02F70970, 0x38600000)

        print("Disabling has not yet been implemented for this code")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def fly():
    if cb4.get() == 5:

        tcp.pokemem(0x0271AA74, 0x38600001)

        print("You can now fly!")
        
    elif cb4.get() == 6:

        tcp.pokemem(0x0271AA74, 0x38600000)

        print("Fly disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def FOFbypass():
    if cb5.get() == 7:

        tcp.pokemem(0x02D5731C, 0x38600001)
        tcp.pokemem(0x02D57320, 0x4E800020)

        print("You can now bypass Friends of Friends!")
        
    elif cb5.get() == 8:

        tcp.pokemem(0x02D5731C, 0x7C0802A6)
        tcp.pokemem(0x02D57320, 0x9421FFF0)

        print("FOF Disabled")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def host():
    if cb6.get() == 9:

        tcp.pokemem(0x02F17B30, 0x38807D00)

        print("You can now use host options anywhere!")
        
    elif cb6.get() == 10:

        tcp.pokemem(0x02D5731C, 0x7C0802A6)
        tcp.pokemem(0x02D57320, 0x9421FFF0)

        print("Host options disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def multiJump():
    if cb7.get() == 11:

        tcp.pokemem(0x0232F3A0, 0x38800001)

        print("Multi-Jump is now active!")
        
    elif cb7.get() == 12:

        tcp.pokemem(0x0232F3A0, 0x38800000)

        print("Multi-Jump disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def muteMic():
    if cb8.get() == 13:

        tcp.pokemem(0x10997EA8, 0x30000000)

        print("Your mic is now muted!")
        
    elif cb8.get() == 14:

        tcp.pokemem(0x10997EA8, 0x3F000000)

        print("Mute mic disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def reach():
    if cb9.get() == 15:

        tcp.pokemem(0x108C9C20, 0x50090000)

        print("Reach = 3 blocks")
        
    elif cb9.get() == 16:

        tcp.pokemem(0x108C9C20, 0x40080000)

        print("reach disabled")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def keyboard():
    if cb10.get() == 17:

        tcp.pokemem(0x02F88110, 0x39400002)
        tcp.pokemem(0x02FAF4F0, 0x39400002)
        tcp.pokemem(0x02FAF560, 0x39400002)
        tcp.pokemem(0x02FAF5DC, 0x39400002)
        tcp.pokemem(0x02FAF64C, 0x39400002)

        print("The entire keyboard is now unlocked!")
        
    elif cb10.get() == 18:

        tcp.pokemem(0x02F88110, 0x39400003)
        tcp.pokemem(0x02FAF4F0, 0x39400003)
        tcp.pokemem(0x02FAF560, 0x39400003)
        tcp.pokemem(0x02FAF5DC, 0x39400003)
        tcp.pokemem(0x02FAF64C, 0x39400003)

        print("Keyboard locked!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def FOV():
    if cb11.get() == 19:

        tcp.pokemem(0x1088EDA8, 0x3F800000)

        print("FOV Enhanced")
        
    elif cb11.get() == 20:

        tcp.pokemem(0x1088EDA8, 0x3F000000)

        print("FOV Reset")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def Hitbox():
    if cb12.get() == 21:

        tcp.pokemem(0x030FA0C8, 0xFFFFFFFF)
        tcp.pokemem(0x030FA014, 0x2C090001)
        tcp.pokemem(0x030F9C50, 0xFFFFFFFF)

        print("Hitbox now shown!")
        
    elif cb12.get() == 22:

        tcp.pokemem(0x030FA0C8, 0xFFFFFFFF)
        tcp.pokemem(0x030FA014, 0x2C090001)
        tcp.pokemem(0x030F9C50, 0xFFFFFFFF)

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

        tcp.pokemem(0x0316760C, 0x60000000)
        tcp.pokemem(0x0316762C, 0xFC80F090)
        tcp.pokemem(0x03168DCC, 0xEC4F6BFA)
        tcp.pokemem(0x0384CEBC, 0x3D801002)
        tcp.pokemem(0x0384CEC0, 0x3C40433A)
        tcp.pokemem(0x0384CEC4, 0x904C0110)
        tcp.pokemem(0x0384CEC8, 0xC02C0110)
        tcp.pokemem(0x0384CECC, 0x4B91A770)
        tcp.pokemem(0x03167638, 0x486E5884)

        print("Item Drop animation has now been modified!")
        
    elif cb13.get() == 24:

        tcp.pokemem(0x0316760C, 0x60000000)
        tcp.pokemem(0x0316762C, 0xFC80F090)
        tcp.pokemem(0x03168DCC, 0xEC4F6BFA)
        tcp.pokemem(0x0384CEBC, 0x3D801002)
        tcp.pokemem(0x0384CEC0, 0x3C40433A)
        tcp.pokemem(0x0384CEC4, 0x904C0110)
        tcp.pokemem(0x0384CEC8, 0xC02C0110)
        tcp.pokemem(0x0384CECC, 0x4B91A770)
        tcp.pokemem(0x03167638, 0x486E5884)

        print("Disabling for this code has not yet been added.")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def offhand():
    if cb14.get() == 25:

        tcp.pokemem(0x024FD7F4, 0x38600001)
        tcp.pokemem(0x0207F604, 0x38600001)

        print("All item slots have been unlocked! (eg offhand and body slots)")
        
    elif cb14.get() == 26:

        tcp.pokemem(0x024FD7F4, 0x38600000)
        tcp.pokemem(0x0207F604, 0x38600000)

        print("Item slots back to normal")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def takeAll():
    if cb15.get() == 27:

        tcp.pokemem(0x02DEC0B4, 0x38600001)

        print("You can now take everything from chests!")
        
    elif cb15.get() == 28:

        tcp.pokemem(0x02DEC0B4, 0x57E3063E)

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
        
def noclip():
    if cb16.get() == 1:

        tcp.pokemem(0x0232E644, 0xFFFFFFFF)

        print("No-ClIP_Addr is now active!")
        
    elif cb16.get() == 0:

        tcp.pokemem(0x0232E644, 0xFC20F890)

        print("No-ClIP_Addr disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def riptidePunch():
    if cb17.get() == 1:

        tcp.pokemem(0x031F5484, 0x88630390)

        print("RIP_Addrtide Punch is now active!")
        
    elif cb17.get() == 0:

        tcp.pokemem(0x031F5484, 0x88630A08)

        print("RIP_Addrtide Punch disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def riptideAnywhere():
    if cb18.get() == 1:

        tcp.pokemem(0x0232C210, 0x38600001)

        print("RIP_Addrtide Punch is now active!")
        
    elif cb18.get() == 0:

        tcp.pokemem(0x0232C210, 0x38600000)

        print("RIP_Addrtide Punch disabled!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def rodDMG():
    if cb19.get() == 1:

        tcp.pokemem(0x10610AB8, 0x3F800000)

        print("Rods now do damage!")
        
    elif cb19.get() == 0:

        tcp.pokemem(0x10610AB8, 0x3F800000)

        print("Rods no longer do damage!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def explosiveArrows():
    if cb20.get() == 1:

        tcp.pokemem(0x020063F0, 0x487E23A4)

        print("Arrows and tridnets now explode!")
        
    elif cb20.get() == 0:

        tcp.pokemem(0x020063F0, 0x7C0802A6)

        print("Arrows and tridnets no longer explode!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def Speed():
    if cb21.get() == 1:

        tcp.pokemem(0x1066AAE8, 0x3F76F5C3)
        tcp.pokemem(0x1066879C, 0x3DF5C28F)
        tcp.pokemem(0x1066ACC8, 0x3EB9BD1F)

        print("You now have speed!")
        
    elif cb21.get() == 0:

        tcp.pokemem(0x1066AAE8, 0x3F68F5C3)
        tcp.pokemem(0x1066879C, 0x3CA3D70A)
        tcp.pokemem(0x1066ACC8, 0x3E26AD89)

        print("You no longer have speed!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def antiKB():
    if cb22.get() == 1:

        tcp.pokemem(0x0257D85C, 0x4E800020)

        print("You no longer take KnockBack!")
        
    elif cb22.get() == 0:

        tcp.pokemem(0x0257D85C, 0x9421FFA8)

        print("You now take KnockBack")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def allPerms():
    if cb22.get() == 1:

        tcp.pokemem(0x02C57E94, 0x38600001)
        tcp.pokemem(0x02C57E34, 0x38600001)
        tcp.pokemem(0x02C51C20, 0x38600001)
        tcp.pokemem(0x02C5CC84, 0x38600001)
        tcp.pokemem(0x02C57D74, 0x38600001)
        tcp.pokemem(0x02C57DD4, 0x38600001)

        print("All perms now activated!")
        
    elif cb22.get() == 0:

        tcp.pokemem(0x02C57E94, 0x57E3063E)
        tcp.pokemem(0x02C57E34, 0x57E3063E)
        tcp.pokemem(0x02C51C20, 0x57E3063E)
        tcp.pokemem(0x02C5CC84, 0x88630124)
        tcp.pokemem(0x02C57D74, 0x57E3063E)
        tcp.pokemem(0x02C57DD4, 0x57E3063E)

        print("You no longer have all perms!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def fullBright():
    if cb24.get() == 1:

        tcp.pokemem(0x108C7C2C, 0x60000000)

        print("You can now see in the dark!")
        
    elif cb24.get() == 0:

        tcp.pokemem(0x108C7C2C, 0x3CF5C28F)

        print("Sight set back to normal")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lockServer():
    if cb25.get() == 1:

        tcp.pokemem(0x02D5B28C, 0x3BC00001)

        print("Your server is now locked!")
        
    elif cb25.get() == 0:

        tcp.pokemem(0x02D5B28C, 0x3BC00008)

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

        tcp.pokemem(0x105DD948, 0x3F100000)

        print("Aura Lvl 1 is now active")
        
    elif cg.get() == 0:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl2():
    if cg2.get() == 2:

        tcp.pokemem(0x105DD948, 0x3F200000)

        print("Aura Lvl 2 is now active")
        
    elif cg2.get() == 3:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl3():
    if cg3.get() == 4:

        tcp.pokemem(0x105DD948, 0x3F300000)

        print("Aura Lvl 3 is now active")
        
    elif cg3.get() == 5:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl4():
    if cg4.get() == 6:

        tcp.pokemem(0x105DD948, 0x3F400000)

        print("Aura Lvl 4 is now active")
        
    elif cg4.get() == 7:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl5():
    if cg5.get() == 8:

        tcp.pokemem(0x105DD948, 0x3F500000)

        print("Aura Lvl 5 is now active")
        
    elif cg5.get() == 9:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl6():
    if cg6.get() == 10:

        tcp.pokemem(0x105DD948, 0x3F600000)

        print("Aura Lvl 6 is now active")
        
    elif cg6.get() == 11:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl7():
    if cg7.get() == 12:

        tcp.pokemem(0x105DD948, 0x3F700000)

        print("Aura Lvl 7 is now active")
        
    elif cg7.get() == 13:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl8():
    if cg8.get() == 14:

        tcp.pokemem(0x105DD948, 0x3F800000)

        print("Aura Lvl 8 is now active")
        
    elif cg8.get() == 15:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl9():
    if cg9.get() == 16:

        tcp.pokemem(0x105DD948, 0x3FF00000)

        print("In the words of 9 year olds: You're a hackowr")
        
    elif cg9.get() == 17:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)
        
def lvl10():
    if cg10.get() == 18:

        tcp.pokemem(0x105DD948, 0x41099999)

        print("I think you just became god")
        
    elif cg10.get() == 19:

        tcp.pokemem(0x105DD948, 0x3F000000)

        print("Aura reset!")
        
    else:
        canv = Tk()
        canv.title('Error!')
        canv.geometry('200x40')
        btn = Button(canv, text = "ERROR!", bd='5', bg="black",                      fg="white",command=canv.destroy)

#===== Section Tab 2 Defined Functions End ====

#===== Section: Tab 2 =====

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

c = Checkbutton(tab2, text="Mega-Code", variable=cb2, onvalue=1, offvalue=0, command=megaCode)
c.grid(column=0, row=0)

c = Checkbutton(tab2, text="Armor Hud", variable=cb, onvalue=2, offvalue=3, command=armorHud)
c.grid(column=1, row=0)

c = Checkbutton(tab2, text="Craft All", variable=cb3, onvalue=3, offvalue=4, command=craftAll)
c.grid(column=2, row=0)

c = Checkbutton(tab2, text="Fly", variable=cb4, onvalue=5, offvalue=6, command=fly)
c.grid(column=3, row=0)

c = Checkbutton(tab2, text="FOF Bypass", variable=cb5, onvalue=7, offvalue=8, command=FOFbypass)
c.grid(column=4, row=0)

c = Checkbutton(tab2, text="Host Options", variable=cb6, onvalue=9, offvalue=10, command=host)
c.grid(column=0, row=1)

c = Checkbutton(tab2, text="Multi-Jump", variable=cb7, onvalue=11, offvalue=12, command=multiJump)
c.grid(column=1, row=1)

c = Checkbutton(tab2, text="Mute Mic", variable=cb8, onvalue=13, offvalue=14, command=muteMic)
c.grid(column=2, row=1)

c = Checkbutton(tab2, text="Reach", variable=cb9, onvalue=15, offvalue=16, command=reach)
c.grid(column=3, row=1)

c = Checkbutton(tab2, text="Unlock Keyboard", variable=cb10, onvalue=17, offvalue=18, command=keyboard)
c.grid(column=4, row=1)

c = Checkbutton(tab2, text="FOV", variable=cb11, onvalue=19, offvalue=20, command=FOV)
c.grid(column=0, row=2)

c = Checkbutton(tab2, text="Draw Hitbox", variable=cb12, onvalue=21, offvalue=22, command=Hitbox)
c.grid(column=1, row=2)

c = Checkbutton(tab2, text="Item Drop", variable=cb13, onvalue=23, offvalue=24, command=itemJava)
c.grid(column=2, row=2)

c = Checkbutton(tab2, text="Unlock Offhand", variable=cb14, onvalue=25, offvalue=26, command=offhand)
c.grid(column=3, row=2)

c = Checkbutton(tab2, text="Take All", variable=cb15, onvalue=27, offvalue=28, command=takeAll)
c.grid(column=4, row=2)

c = Checkbutton(tab2, text="No-Clip", variable=cb16, onvalue=1, offvalue=0, command=noclip)
c.grid(column=0, row=3)

c = Checkbutton(tab2, text="Riptide Punch", variable=cb17, onvalue=1, offvalue=0, command=riptidePunch)
c.grid(column=1, row=3)

c = Checkbutton(tab2, text="Rtide Anywhere", variable=cb18, onvalue=1, offvalue=0, command=riptideAnywhere)
c.grid(column=2, row=3)

c = Checkbutton(tab2, text="Rods Do Damage", variable=cb19, onvalue=1, offvalue=0, command=rodDMG)
c.grid(column=3, row=3)

c = Checkbutton(tab2, text="Arrows Explode", variable=cb20, onvalue=1, offvalue=0, command=explosiveArrows)
c.grid(column=4, row=3)

c = Checkbutton(tab2, text="Speed", variable=cb21, onvalue=1, offvalue=0, command=Speed)
c.grid(column=0, row=4)

c = Checkbutton(tab2, text="Anti-KB", variable=cb22, onvalue=1, offvalue=0, command=antiKB)
c.grid(column=1, row=4)

c = Checkbutton(tab2, text="All Perms", variable=cb23, onvalue=1, offvalue=0, command=allPerms)
c.grid(column=2, row=4)

c = Checkbutton(tab2, text="Full Bright", variable=cb24, onvalue=1, offvalue=0, command=fullBright)
c.grid(column=3, row=4)

c = Checkbutton(tab2, text="Lock Server", variable=cb25, onvalue=1, offvalue=0, command=lockServer)
c.grid(column=4, row=4)

#===== Section: Tab 2 End =====

#===== Section: Tab 3 =====

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

c = Checkbutton(tab3, text="Level One", variable=cg, onvalue=1, offvalue=0, command=lvl1)
c.grid(column=0, row=0)

c = Checkbutton(tab3, text="Level Two", variable=cg2, onvalue=2, offvalue=3, command=lvl2)
c.grid(column=1, row=0)

c = Checkbutton(tab3, text="Level Three", variable=cg3, onvalue=4, offvalue=5, command=lvl3)
c.grid(column=2, row=0)

c = Checkbutton(tab3, text="Level Four", variable=cg4, onvalue=6, offvalue=7, command=lvl4)
c.grid(column=3, row=0)

c = Checkbutton(tab3, text="Level Five", variable=cg5, onvalue=8, offvalue=9, command=lvl5)
c.grid(column=4, row=0)

c = Checkbutton(tab3, text="Level Six", variable=cg6, onvalue=10, offvalue=11, command=lvl6)
c.grid(column=0, row=1)

c = Checkbutton(tab3, text="Level Seven", variable=cg7, onvalue=12, offvalue=13, command=lvl7)
c.grid(column=1, row=1)

c = Checkbutton(tab3, text="Level Eight", variable=cg8, onvalue=14, offvalue=15, command=lvl8)
c.grid(column=2, row=1)

c = Checkbutton(tab3, text="Blatamt", variable=cg9, onvalue=16, offvalue=17, command=lvl9)
c.grid(column=3, row=1)

c = Checkbutton(tab3, text="Absurdly Blatamt", variable=cg10, onvalue=18, offvalue=19, command=lvl10)
c.grid(column=4, row=1)

#===== Section: Tab 3 End =====

window.mainloop()
