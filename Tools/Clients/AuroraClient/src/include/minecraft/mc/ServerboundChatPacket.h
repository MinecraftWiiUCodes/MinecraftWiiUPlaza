#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"
#include "../mstd/WString.h"

namespace mc {
    class ServerboundChatPacket : public Packet {
    public:
        ServerboundChatPacket(const mstd::wstring& msg) {
            code::Func<void, 0x028A98DC, ServerboundChatPacket*, const mstd::wstring&>()(this, msg);
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