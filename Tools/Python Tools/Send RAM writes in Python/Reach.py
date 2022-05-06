# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x108C9C20, 0x50090000)
tcp.s.close()
print("You now have 3 block reach!")
