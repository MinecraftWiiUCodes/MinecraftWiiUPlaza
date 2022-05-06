# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x105DD948, 0x3FF00000)
tcp.s.close()
print("Hitboxes are now enlarged!")
