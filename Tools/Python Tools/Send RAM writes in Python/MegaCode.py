# Created by Korozin
from tcpgecko import TCPGecko
tcp = TCPGecko("192.168.1.97")
tcp.pokemem(0x02F88110, 0x39400002)
tcp.pokemem(0x02FAF4F0, 0x39400002)
tcp.pokemem(0x02FAF560, 0x39400002)
tcp.pokemem(0x02FAF5DC, 0x39400002)
tcp.pokemem(0x02FAF64C, 0x39400002)
tcp.pokemem(0x030FA014, 0x2C090001)
tcp.pokemem(0x030F9D38, 0xFC605018)
tcp.pokemem(0x030F9D3C, 0xFC20A018)
tcp.pokemem(0x030F9D40, 0xFC005018)
tcp.pokemem(0x030F9D30, 0xFC405018)
tcp.pokemem(0x030F9D10, 0xFD69482A)
tcp.pokemem(0x030F9D20, 0xFD494828)
tcp.pokemem(0x030F9C50, 0x7F24CB78)
tcp.pokemem(0x11120008, 0x000006D5)
tcp.pokemem(0x02D5731C, 0x38600001)
tcp.pokemem(0x02D57320, 0x4E800020)
tcp.pokemem(0x02F70970, 0x38600001)
tcp.pokemem(0x032283CC, 0x38800000)
tcp.pokemem(0x02F59534, 0x480002E8)
tcp.pokemem(0x02DEC0B4, 0x38600001)
tcp.pokemem(0x10997EA8, 0x30000000)
tcp.pokemem(0x02E9B1B0, 0x7FE4FB78)
tcp.pokemem(0x030FA0C8, 0x616500FF)
tcp.pokemem(0x0384CD3C, 0x60000000)
tcp.pokemem(0x0384CD40, 0x3D804100)
tcp.pokemem(0x0384CD44, 0x618C0003)
tcp.pokemem(0x0384CD48, 0x80AC0000)
tcp.pokemem(0x0384CD4C, 0x4B8AD380)
tcp.pokemem(0x108C7C2C, 0x60000000)
tcp.pokemem(0x0205C430, 0x38600000)
tcp.pokemem(0x031B2B4C, 0x38600001)
tcp.pokemem(0x02F5C874, 0x38600001)
tcp.pokemem(0x105DD948, 0x3F000000)
tcp.s.close()
print("Mega code is now activated!")
