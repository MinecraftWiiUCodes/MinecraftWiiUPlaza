#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "../mstd/WString.h"

namespace mc {
    class BufferedImage {
    public:
        BufferedImage(uint8_t* buffer, uint32_t size) {
            code::Func<void, 0x03029a40, BufferedImage*, uint8_t*, uint32_t>()(this, buffer, size);
        }

        BufferedImage(const mstd::wstring& path1, bool unk, bool unk2, const mstd::wstring& path2, int unk3) {
            code::Func<void, 0x03028614, BufferedImage*, const mstd::wstring&, bool, bool, const mstd::wstring&, int>()(this, path1, unk, unk2, path2, unk3);
        }

        uint32_t field_0x0;
        uint32_t field_0x4;
        uint32_t field_0x8;
        uint32_t field_0xC;
        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
        uint32_t field_0x28;
        uint32_t field_0x2C;
        uint32_t field_0x30;
    };
}