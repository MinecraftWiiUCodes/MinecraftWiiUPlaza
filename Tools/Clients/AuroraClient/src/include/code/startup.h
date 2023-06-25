#pragma once

#include <inttypes.h>
#include <cstddef>

void code_main();

extern "C" __attribute__((section(".startup"))) void startup() {
    asm volatile("_startup_main:");
    code_main();
    asm volatile("_startup_main_end:");
}
