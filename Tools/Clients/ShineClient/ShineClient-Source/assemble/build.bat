@echo off
cd assemble
powerpc-eabi-as.exe -mregnames -mgekko ../main.txt -o asm
powerpc-eabi-objcopy.exe -O "binary" asm asm