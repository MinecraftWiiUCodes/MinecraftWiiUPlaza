#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Packet.h"

namespace mc {
    class ServerboundMovePlayerPacket : public Packet {
    public:
        ServerboundMovePlayerPacket(double x, double y, double z, bool isOnGround, bool isFlying) {
            code::Func<void, 0x028AD6C4, ServerboundMovePlayerPacket*, bool, bool, double, double, double, double>()(this, isOnGround, isFlying, x, y, y, z);
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