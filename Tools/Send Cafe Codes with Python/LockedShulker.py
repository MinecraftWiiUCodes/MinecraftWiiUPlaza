import math
import socket
import os

ip='192.168.1.97' # IP Address
print('\n ** Sending Cafe Code! **\n')
print('  Connecting to: '+ip)
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((ip,7331))
s.send(bytes.fromhex('03'))
s.send(bytes.fromhex('10014CFC00000000'))

cafe='3000000010A0A77010000000500000003100000000000024301000000000000010000000500000000012001812004000D0000000DEADCAFE0002000011FFFFFC1200400000000000D0000000DEADCAFE3000000011FFFFFC1000000050000000001200001058FA540012000400A8A59800120008000000110012000C00000001001200103F800000001200140000001100120018120040400012001C000000003100000000000018301000000000000010000000500000000012000000000000001200040000000000120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C000000000012002000000000001200240000000000120028000000000012003000000000001200340000000000120038120040940012003C000000000012004000000000001200441200420400120048000000000012004C0000000000120050000000000012005400000000D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000000120000000000000012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004210001200200000001200120024000000170012002812004240310000000000001C30100000000000001000000050000000001200000042006C00120004006F006300120008006B00450012000C006E0074001200100069007400120014007900540012001800610067D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC30100000000000001000000050000000001200001058FA540012000400A8A59800120008000000110012000C00000001001200103F800000001200140000001100120018120042700012001C000000003100000000000018301000000000000010000000500000000012000000000000001200040000000000120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C000000000012002000000000001200240000000000120028000000000012003000000000001200340000000000120038120040940012003C000000000012004000000000001200441200435400120048000000000012004C000000000012005000000000001200540000000031000000000000443010000000000000100000005000000000120000000000000012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004360001200200000001200120024000000170012002812004380310000000000001C301000000000000010000000500000000012000000530070001200040061007700120008006E00440012000C0061007400120010006100000012001400000000001200180000000031000000FFFFFFF030100000000000001000000050000000001200001058FA540012000400A8A59800120008000000110012000C00000001001200103F800000001200140000001100120018120043B00012001C000000003100000000000018301000000000000010000000500000000012000000000000001200040000000000120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C000000000012002000000000001200240000000000120028000000000012003000000000001200340000000000120038120040940012003C000000000012004000000000001200441200451400120048000000000012004C0000000000120050000000000012005400000000D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000000120000120045940012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004520001200200000001200120024000000170012002812004420310000000000001C3010000000000000100000005000000000120000004C006F001200040063006B00120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C00000000001200200000000031000000FFFFFFF0301000000000000010000000500000000012000010783144001200040000000000120008000000000012000C000000000012001000000000001200140000000000120018120044400012001C0000001131000000000000183010000000000000100000005000000000120000006D006900120004006E006500120008006300720012000C00610066001200100074003A0012001400730068001200180075006C0012001C006B0065001200200072000000120024000000000012002800000000D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000000120000120046240012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C120045A0001200200000001200120024000000170012002812004560310000000000001C30100000000000001000000050000000001200000053006900120004006C006500120008006E00740012000C000000000012001000000000001200140000000000120018000000000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001056415C0012000401A8A588D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000000120000120046A40012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C120046300012002000000012001200240000001700120028120045F0310000000000001C3010000000000000100000005000000000120000004E006F001200040041004900120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001056415C0012000401A8A588D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF0301000000000000010000000500000003100000000000018301000000000000010000000500000003100000000000044301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000000301000000000000010000000500000000012000012004AE40012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C120046B00012002000000012001200240000001700120028120046F0310000000000001C301000000000000010000000500000000012000000410060001200040074006900120008007600650012000C004500660012001000660065001200140063007400120018007300000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001067DAF0001200040000000000120008120047200012000C120047240012001012004724001200140A000000D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000083010000000000000100000005000000000120000120047700012000400000000310000000000000030100000000000001000000050000000001200001058FA540012000400A8A59800120008000000110012000C00000001001200103F800000001200140000001100120018120047A00012001C000000003100000000000018301000000000000010000000500000000012000000000000001200040000000000120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C000000000012002000000000001200240000000000120028000000000012003000000000001200340000000000120038120040940012003C000000000012004000000000001200441200486400120048000000000012004C000000000012005000000000001200540000000031000000000000443010000000000000100000005000000000120000120048F40012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004870001200200000001200120024000000170012002812004830310000000000001C30100000000000001000000050000000001200000053006800120004006F007700120008005000610012000C00720074001200100069006300120014006C006500120018007300000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001056415C0012000400A8A588D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000083010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000000120000120049940012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C120049000012002000000012001200240000001700120028120048C0310000000000001C30100000000000001000000050000000001200000041006D001200040070006C00120008006900660012000C006900650012001000720000001200140000000000120018000000000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001056415C00120004FFA8A588D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF0301000000000000010000000500000003100000000000008301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000018301000000000000010000000500000003100000000000044301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000000301000000000000010000000500000000012000012004A440012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C120049A0001200200000001200120024000000170012002812004960310000000000001C301000000000000010000000500000000012000000490064001200040000000000120008000000000012000C000000000012001000000000001200140000000000120018000000000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001056415C001200040BA8A588D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000083010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000000120000000000000012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004A50001200200000001200120024000000170012002812004A10310000000000001C301000000000000010000000500000000012000000440075001200040072006100120008007400690012000C006F006E0012001000000000001200140000000000120018000000000012001C00000000001200200000000031000000FFFFFFF030100000000000001000000050000000001200001065993C001200047FFFFFFFD0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF0301000000000000010000000500000003100000000000018301000000000000010000000500000003100000000000044301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000000301000000000000010000000500000003100000000000000301000000000000010000000500000000012000012004BD40012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004AF0001200200000001200120024000000170012002812004AB0310000000000001C30100000000000001000000050000000001200000043006F00120004006C006F00120008007200000012000C0000000000120010006D0065001200140056006900120018007300690012001C0062006C001200200065000031000000FFFFFFF030100000000000001000000050000000001200001056415C0012000404A8A588D0000000DEADCAFE3000000011FFFFFC100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFFC3010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000FFFFFFD4310000000000001C3010000000000000100000005000000031000000FFFFFFF03010000000000000100000005000000031000000000000183010000000000000100000005000000031000000000000443010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000031000000000000003010000000000000100000005000000000120000000000000012000425D93052001200080000003B31000000FFFFFFD4001200000000001800120004000000330012000811B0D4300012000C0000AFB8001200109E3779B900120014000000120012001811B0D4D00012001C12004BE0001200200000001200120024000000170012002812004B30310000000000001C3010000000000000100000005000000000120000004400650012000400610074001200080068004C0012000C006F006F0012001000740054001200140061006200120018006C00650012001C00000000001200200000000031000000FFFFFFF0301000000000000010000000500000000012000010783144001200040000000000120008000000000012000C00000000001200100000000000120014000000000012001812004B500012001C0000002231000000000000183010000000000000100000005000000000120000006D006900120004006E006500120008006300720012000C00610066001200100074003A001200140063006800120018006500730012001C0074007300120020002F006500120024006E006400120028005F00630012002C00690074001200300079005F001200340074007200120038006500610012003C007300750012004000720065D0000000DEADCAFE00020000120042B41200451400000000D0000000DEADCAFE' # Cafe Code
for x in range(math.floor(len(cafe)/8)):
    s.send(bytes.fromhex('03'))
    s.send(bytes.fromhex('0'+format(0x01133000+x*4,'X')+cafe[x*8:x*8+8]))
s.send(bytes.fromhex('03'))
s.send(bytes.fromhex('10014CFC00000001'))

asm='' # Assembly RAM Write
for x in range(math.floor(len(asm)/16)):
    s.send(bytes.fromhex('03'))
    s.send(bytes.fromhex(asm[x*16:x*16+16]))
s.close()
print('\nSent Codes Successfully')
