# -*- coding: cp1252 -*-
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x108E2834, 0x0058002C)
tcp.pokemem(0x108E2838, 0x00200025)
tcp.pokemem(0x108E283C, 0x00640020)
tcp.pokemem(0x108E2840, 0x00200059)
tcp.pokemem(0x108E2844, 0x002C0020)
tcp.pokemem(0x108E2848, 0x00250064)
tcp.pokemem(0x108E284C, 0x00200020)
tcp.pokemem(0x108E2850, 0x0020005A)
tcp.pokemem(0x108E2854, 0x002C0020)
tcp.pokemem(0x108E2858, 0x00250064)
tcp.pokemem(0x108E285C, 0x00200053)
tcp.pokemem(0x108E2860, 0x0069007A)
tcp.pokemem(0x108E2864, 0x00650020)
tcp.pokemem(0x108E286C, 0x0020004E)
tcp.pokemem(0x108E2870, 0x0075006B)
tcp.pokemem(0x108E2874, 0x00650072)
tcp.pokemem(0x108E2878, 0x003A0000)
tcp.pokemem(0x108E287C, 0x00000000)
tcp.pokemem(0x108E2880, 0x00000000)
tcp.pokemem(0x108E2884, 0x0050006F)
tcp.pokemem(0x108E2888, 0x00740069)
tcp.pokemem(0x108E288C, 0x006F006E)
tcp.pokemem(0x108E2890, 0x00200054)
tcp.pokemem(0x108E2894, 0x0069006D)
tcp.pokemem(0x108E2898, 0x00650020)
tcp.pokemem(0x108E289C, 0x007B0074)
tcp.pokemem(0x108E28A0, 0x00610067)
tcp.pokemem(0x108E28A4, 0x0031007D)
tcp.pokemem(0x108E28A8, 0x003D0036)
tcp.pokemem(0x108E28AC, 0x003B0033)
tcp.pokemem(0x108E28B0, 0x00300020)
tcp.pokemem(0x108E28B4, 0x00200057)
tcp.pokemem(0x108E28B8, 0x006F0072)
tcp.pokemem(0x108E28BC, 0x006C0064)
tcp.pokemem(0x108E28C0, 0x00200054)
tcp.pokemem(0x108E28C4, 0x0069006D)
tcp.pokemem(0x108E28C8, 0x00650020)
tcp.pokemem(0x108E28CC, 0x00310030)
tcp.pokemem(0x108E28D0, 0x00300030)
tcp.pokemem(0x108E28D4, 0x00300000)
tcp.pokemem(0x108E28C8, 0x00650020)
tcp.pokemem(0x108E28CC, 0x00310030)
tcp.pokemem(0x108E28D0, 0x00000000)
tcp.pokemem(0x108E28D4, 0x00000000)
tcp.s.close()
print("map nuker size now activated!")
