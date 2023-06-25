#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mc {
    class AABB {
    public:
        AABB(double x1, double y1, double z1, double x2, double y2, double z2) {
            code::Func<void, 0x0200c920, AABB*, double, double, double, double, double, double>()(this, x1, y1, z1, x2, y2, z2);
        }

        AABB(AABB const & aabb) {
            code::Func<void, 0x02ca58a8, AABB*, AABB const &>()(this, aabb);
        }

        void set(AABB const * aabb) {
            code::Func<void, 0x0200cf7c, AABB*, AABB const *>()(this, aabb);
        }

        AABB move(double x, double y, double z) {
            return code::Func<AABB, 0x0200d260, AABB*, double, double, double>()(this, x, y, z);
        }

        double minX;
        double minY;
        double minZ;
        double maxX;
        double maxY;
        double maxZ;
    };
}