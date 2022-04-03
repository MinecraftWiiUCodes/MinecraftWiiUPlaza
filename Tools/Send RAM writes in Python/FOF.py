# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x02D5731C, 0x38600001)
tcp.pokemem(0x02D57320, 0x4E800020)
tcp.s.close()
print("You can now bypass Friends of Friends!")
