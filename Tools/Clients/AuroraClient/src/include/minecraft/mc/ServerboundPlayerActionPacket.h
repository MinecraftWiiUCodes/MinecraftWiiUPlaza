#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"

namespace mc {
    class ServerboundPlayerActionPacket : public Packet{
    public:
        ServerboundPlayerActionPacket(uint32_t arg2, const BlockPos& pos) {
            uint32_t arg4 = *(uint32_t*) 0x104C24A0;
            code::Func<void, 0x028AEC5C, ServerboundPlayerActionPacket*, uint32_t, const BlockPos&, uint32_t>()(this, arg2, pos, arg4);
        }

        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
        uint32_t field_0x28;
        uint32_t field_0x2C;
    };
}