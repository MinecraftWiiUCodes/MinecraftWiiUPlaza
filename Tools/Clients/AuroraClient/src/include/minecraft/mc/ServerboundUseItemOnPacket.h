#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"

namespace mc {
    class ServerboundUseItemOnPacket : public Packet {
    public:
        ServerboundUseItemOnPacket(const mc::BlockPos& pos) {
            uint32_t b = code::Mem(0x104C24A4).as<uint32_t>();
            code::Func<void, 0x28B25B8, ServerboundUseItemOnPacket*, const mc::BlockPos&, uint32_t, uint32_t>()(this, pos, b, 0);
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
    };
}