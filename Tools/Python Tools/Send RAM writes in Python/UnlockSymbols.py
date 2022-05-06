# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x02F88110, 0x39400002)
tcp.pokemem(0x02FAF4F0, 0x39400002)
tcp.pokemem(0x02FAF560, 0x39400002)
tcp.pokemem(0x02FAF5DC, 0x39400002)
tcp.pokemem(0x02FAF64C, 0x39400002)
tcp.s.close()
print("The entire keyboard is now unlocked!")
