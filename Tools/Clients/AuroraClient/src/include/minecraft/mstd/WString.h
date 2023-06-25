#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mstd {
    class wstring {
    public:
        wstring(const char16_t* str) {
            code::Func<void, 0x020B08D4, wstring*, const char16_t*>()(this, str);
        }

        wstring(char16_t* str) {
            code::Func<void, 0x020B08D4, wstring*, char16_t*>()(this, str);
        }

        wstring() = default;

        void* unk_0x0;     // 0
        char16_t str[8];   // 4
        char16_t* pointer; // 14
        int trueLength;    // 18
        int length2;       // 1C
    };
}