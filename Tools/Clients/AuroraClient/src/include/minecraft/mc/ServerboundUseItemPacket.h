#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"

namespace mc {
    class ServerboundUseItemPacket : public Packet {
    public:
        ServerboundUseItemPacket(int unk) {
            code::Func<void, 0x28B25B8, ServerboundUseItemPacket*, int>()(this, unk);
        }
    
        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
    };
}