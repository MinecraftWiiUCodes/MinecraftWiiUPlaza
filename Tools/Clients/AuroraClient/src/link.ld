OUTPUT_ARCH(powerpc)
ENTRY(startup)

MEMORY {
  rom(rxai) : ORIGIN = 0x02000000, LENGTH = 16m
  ram(rwai) : ORIGIN = 0x01800000, LENGTH = 1m
}

SECTIONS {
  .text :
  {
    *(.startup)
    *(.text.*)
  } > rom,

  .data :
  {
    *(.sdata._ZZnwjE3ptr)
    *(.sdata)
    *(.sdata.*)
    *(.rodata)
    *(.rodata.*)
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
  } > ram,
}