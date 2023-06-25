#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"

namespace mc {
    class ServerboundInteractPacket : public Packet {
    public:
        ServerboundInteractPacket(uint32_t entity) {
            code::Func<void, 0x028AC244, ServerboundInteractPacket*, uint32_t>()(this, entity);
        }

        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
    };
}