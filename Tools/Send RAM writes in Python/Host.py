# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x02F17B30, 0x38807D00)
tcp.s.close()
print("You can now use host options anywhere!")
