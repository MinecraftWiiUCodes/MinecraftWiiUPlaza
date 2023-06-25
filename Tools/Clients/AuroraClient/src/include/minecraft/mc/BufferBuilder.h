#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mc {
    class BufferBuilder {
    public:
        void begin(int mode = 3) {
            code::Func<void, 0x02FD1FB4, BufferBuilder*, int>()(this, mode);
        }

        void color(int color, int alpha) {
            code::Func<void, 0x2FD36C4, BufferBuilder*, int, int>()(this, color, alpha);
        }

        void vertexUV(float x, float y, float z, float u, float v) {
            code::Func<void, 0x02FD2AF4, BufferBuilder*, float, float, float, float, float>()(this, x, y, z, u, v);
        }

        void vertex(float x, float y, float z) {
            code::Func<void, 0x02FD2A34, BufferBuilder*, float, float, float>()(this, x, y, z);
        }

        void end() {
            code::Func<void, 0x02FD2288, BufferBuilder*>()(this);
        }
    };
}