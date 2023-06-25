#pragma once

#include <inttypes.h>

namespace code {
    class Mem {
    private:
        uint32_t base;

    public:
        explicit Mem(uint32_t addr) { base = addr; }

        Mem operator[](int offset) {
            return Mem(*reinterpret_cast<uint8_t*>(base) + offset);
        }

        template<typename T>
        T& as() {
            return *reinterpret_cast<T*>(base);
        }
    };
}