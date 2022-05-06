
# Create mod menu from input

tab=0 # Set which creative menu tab the item is in
slot=0 # Set which slot it's in.
id=166 # Set the item ID
cnt=64 # How many of it? Example: 64 being a stack.
dmg=0 # Durability level.

""" List of tabs
0: Building Blocks
1: Decorations
2: Redstone
3: Materials
4: Food
5: Tools and armor
6: Misc
"""

if(tab==0):
	taba='10A0A6C0'
elif(tab==1):
	taba='10A0A6D0'
elif(tab==2):
	taba='10A0A6F0'
elif(tab==3):
	taba='10A0A700'
elif(tab==4):
	taba='10A0A710'
elif(tab==5):
	taba='10A0A720'
else:
	taba='10A0A770'
code='30000000 109DF84C\n10120000 '+format(id*4,'08X')+'\n30000000 '+taba+'\n10000000 50000000\n31000000 '+format(slot*4,'08X')+'\n30100000 00000000\n10000000 50000000\n11120000 00000014\n0012000C '+format(cnt,'08X')+'\n00120020 '+format(dmg,'08X')+'\nD0000000 DEADCAFE'
print(code)
