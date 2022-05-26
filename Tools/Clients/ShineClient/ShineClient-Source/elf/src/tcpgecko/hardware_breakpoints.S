#include "../utils/ppc_asm.h"

.set dabr, 0x3F5

# .section text

.global KernelSetDABR
KernelSetDABR:
    mtspr dabr, r3
    blr

.global SC0x2D_KernelSetDABR
SC0x2D_KernelSetDABR:
    li r0, 0x2D00
    sc
    nop
    blr
