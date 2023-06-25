#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "miscellanious.h"
#include "ItemInstance.h"
#include "Packet.h"

namespace mc {
    class ServerboundSetCreativeModeSlotPacket : public Packet {
    public:
        ServerboundSetCreativeModeSlotPacket(mboost::shared_ptr<mc::ItemInstance> item, int slot) {
            code::Func<void, 0x028B09F8, ServerboundSetCreativeModeSlotPacket*, int, mboost::shared_ptr<mc::ItemInstance>>()(this, slot, item);
        }

        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
    };
}