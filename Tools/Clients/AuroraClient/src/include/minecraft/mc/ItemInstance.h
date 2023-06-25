#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "CompoundTag.h"
#include "Item.h"

namespace mc {
    class ItemInstance {
    public:
        ItemInstance(Item* item, int amount) {
            code::Func<void, 0x024861BC, ItemInstance*, Item*, int>()(this, item, amount);
        }

        void setAuxValue(int val) {
            code::Func<void, 0x02488B2C, ItemInstance*, int>()(this, val);
        }

        uint32_t field_0x0;
        uint32_t field_0x4;
        uint32_t field_0x8;
        uint32_t field_0xC;
        Item* item;
        CompoundTag* tag;
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
        uint32_t field_0x40;
    };
}