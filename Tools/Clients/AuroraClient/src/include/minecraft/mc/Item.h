#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mc {
    class Item {
    public:
        Item() {
            code::Func<void, 0x024B65D8, Item*>()(this);
        }

        void HandEquipped() {
            code::Func<void, 0x024B6904, Item*>()(this);
        }

        void setMaxDamage(uint32_t arg1) {
            code::Func<void, 0x024B6910, Item*, uint32_t>()(this, arg1);
        }

        void setCraftingRemainingItem(uint32_t arg1) {
            code::Func<void, 0x024B6C40, Item*, uint32_t>()(this, arg1);
        }

        void setStackedByData(bool arg1) {
            code::Func<void, 0x024B6C48, Item*, uint8_t>()(this, arg1);
        }

        uint32_t getBaseItemType() {
            return code::Func<uint32_t, 0x024CFD24, Item*>()(this);
        }

        uint32_t getMaxStackSize() {
            return code::Func<uint32_t, 0x024CFFC4, Item*>()(this);
        }

        bool isHandEquipped() {
            return code::Func<uint8_t, 0x024D0030, Item*>()(this);
        }

        static Item* byId(int id) {
            return code::Func<Item*, 0x02486698, int>()(id);
        }

        static int getId(Item* item) {
            return code::Func<int, 0x02492100, Item*>()(item);
        }
    
        uint32_t field_0x0;
        uint32_t field_0x4;
        uint32_t field_0x8;
        uint32_t maxStackSize;
        uint32_t maxDamage;
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
        uint32_t field_0x40;
        uint32_t field_0x44;
        uint32_t field_0x48;
        uint32_t field_0x4C;
        uint32_t field_0x50;
        uint32_t field_0x54;
        uint32_t field_0x58;
        uint32_t field_0x5C;
        uint32_t field_0x60;
        uint32_t field_0x64;
        uint32_t field_0x68;
        uint32_t field_0x6C;
        uint32_t field_0x70;
        uint32_t field_0x74;
    };
}