#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "BufferedImage.h"

namespace mc {
    class Textures {
    public:
        void loadTexture(BufferedImage* img, int id)  {
            code::Func<void, 0x0338f694, Textures*, BufferedImage*, int>()(this, img, id);
        }

        void bind(int id) {
            code::Func<void, 0x0338ee78, Textures*, int>()(this, id);
        }
    };
}