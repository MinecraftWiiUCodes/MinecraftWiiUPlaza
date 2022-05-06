# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x0232F3A0, 0x38800001)
tcp.s.close()
print("Multi-Jump is now active!")
