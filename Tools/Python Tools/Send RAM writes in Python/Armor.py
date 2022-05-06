# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x02E9B1B0, 0x7FE4FB78)
tcp.s.close()
print("You can now see the armor hud like in mini-games!")
