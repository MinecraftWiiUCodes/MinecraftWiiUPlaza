# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x0271AA74, 0x38600001)
tcp.s.close()
print("You can now fly!")
