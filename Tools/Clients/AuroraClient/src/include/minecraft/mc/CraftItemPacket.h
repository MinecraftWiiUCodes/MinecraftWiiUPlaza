#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"

namespace mc {
    class CraftItemPacket : public Packet {
    public:
        CraftItemPacket(int a, int b, int c) {
            code::Func<void, 0x0222DD24, CraftItemPacket*, int, int, int>()(this, a, b, c);
        }

        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
        uint32_t field_0x28;
        uint32_t field_0x2C;
        uint32_t field_0x30;
        uint32_t field_0x34;
        uint32_t field_0x38;
        uint32_t field_0x3C;
    };
}