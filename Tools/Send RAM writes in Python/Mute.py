# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x10997EA8, 0x30000000)
tcp.s.close()
print("Your mic is now muted!")
