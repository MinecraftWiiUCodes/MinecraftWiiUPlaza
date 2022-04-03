# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x10665E20, 0x41000000)
tcp.s.close()
print("Combo code is now active!")
