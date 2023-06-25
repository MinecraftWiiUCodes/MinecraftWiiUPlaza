#pragma once

#include <code/code.h>
#include "BufferBuilder.h"

namespace mc {
    class Tesselator {
    public:
        static Tesselator* getInstance() {
            return code::Func<Tesselator*, 0x3337EF0>()();
        }

        BufferBuilder* getBuilder() {
            return code::Func<BufferBuilder*, 0x3337EE8, Tesselator*>()(this);
        }
    };
}