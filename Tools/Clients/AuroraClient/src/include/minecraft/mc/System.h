#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mc {
    struct time {
    public:
        uint32_t p1;
        uint32_t p2;

        uint32_t operator-(time b) { return p2 - b.p2; }
    };

    class System {
    public:
        static time processTimeInMilliSecs() {
            return code::Func<time, 0x02994408>()();
        }
    };
}