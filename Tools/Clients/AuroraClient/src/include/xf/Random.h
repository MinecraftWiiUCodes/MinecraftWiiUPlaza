#pragma once

#include <inttypes.h>

namespace xf {
    namespace utils {
        uint32_t mLehmer = 0;
            
        class Random {
        public:
            static uint32_t Lehmer32() {
                mLehmer += 0xE120FC15;
                uint64_t tmp;
                tmp = (uint64_t) mLehmer * 0x4A39B70D;
                uint32_t m1 = (tmp >> 32) ^ tmp;
                tmp = (uint64_t) m1 * 0x12FAD5C9;
                uint32_t m2 = (tmp >> 32) ^ tmp;
                return m2;
            }

            static uint32_t next(uint32_t min, uint32_t max) {
                for (uint32_t i = 0; i < 10; i++) Lehmer32();
                return ((Lehmer32() % (max - min + 1)) + min);
            }

            static float nextFloat() {
                for (uint32_t i = 0; i < next(3, 8); i++) Lehmer32();
                int first = next(0,    1000000);
                float result = first / 1000000;
                return result;
            }

            static double nextDouble() {
                for (uint32_t i = 0; i < next(3, 8); i++) Lehmer32();
                int first = next(0,     1000000);
                double result = first / 1000000;
                return result;
            }
        };
    } // utils
} // xf