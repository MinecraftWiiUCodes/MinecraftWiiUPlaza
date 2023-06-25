#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "../mstd/WString.h"

namespace mc {
    class File {
    public:
        File(mstd::wstring const & path) {
            code::Func<void, 0x023237c0, File*, mstd::wstring const &>()(this, path);
        }

        bool exists() {
            return code::Func<bool, 0x023209b8, File*>()(this);
        }

        void _delete() {
            code::Func<void, 0x023238e4, File*>()(this);
        }

        uint32_t field_0x0;
        uint32_t field_0x4;
        uint32_t field_0x8;
        uint32_t field_0xC;
        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
    };
}