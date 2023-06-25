#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "File.h"

#include "../../xf/String.h"

namespace mc {
    struct test {
    public:
        int length;
        const char* text;
    };  

    class FileOutputStream {
    public:
        FileOutputStream(File const & file) {
            code::Func<void, 0x02320b24, FileOutputStream*, File const &>()(this, file);
        }

        void write(xf::String str) {
            for (int i = 0; i < str.length(); i++) {
                write(str.c_str()[i]);
            }
        }

        void write(char c) {
            code::Func<void, 0x02320CC0, FileOutputStream*, char>()(this, c);
        }

        void close() {
            code::Func<void, 0x02320d8c, FileOutputStream*>()(this);
        }

        uint32_t field_0x0;
        uint32_t field_0x4;
    };
}