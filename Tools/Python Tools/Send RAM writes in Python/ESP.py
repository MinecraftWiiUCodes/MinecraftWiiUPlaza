# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x030F9D38, 0xFC605018)
tcp.pokemem(0x030F9D3C, 0xFC20A018)
tcp.pokemem(0x030F9D40, 0xFC005018)
tcp.pokemem(0x030F9D30, 0xFC405018)
tcp.pokemem(0x030F9D10, 0xFD69482A)
tcp.pokemem(0x030F9D20, 0xFD494828)
tcp.pokemem(0x030FA0C8, 0x60000000)
tcp.pokemem(0x030F9C50, 0xFFFFFFFF)
tcp.pokemem(0x11120008, 0x000006D5)
tcp.pokemem(0x030FA014, 0x2C090001)
tcp.s.close()
print("ESP is now activated!")
