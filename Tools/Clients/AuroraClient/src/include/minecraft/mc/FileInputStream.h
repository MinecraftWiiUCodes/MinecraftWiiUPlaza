#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "File.h"

namespace mc {
    class FileInputStream {
    public:
        FileInputStream(File const & file) {
            code::Func<void, 0x023204a4, FileInputStream*, File const &>()(this, file);
        }

        char read() {
            return code::Func<char, 0x023206ac, FileInputStream*>()(this);
        }

        int filesize() {
            return code::Func<int, 0x02320990, FileInputStream*>()(this);
        }

        void close() {
            code::Func<void, 0x02320938, FileInputStream*>()(this);
        }

        uint32_t field_0x0;
        uint32_t field_0x4;
        uint32_t field_0x8;
    };
}