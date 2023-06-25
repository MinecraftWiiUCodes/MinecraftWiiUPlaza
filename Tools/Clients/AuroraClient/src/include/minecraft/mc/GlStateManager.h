#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "BufferedImage.h"

namespace mc {
    class GlStateManager {
    public:
        static void disableDepthTest() {
            code::Func<void, 0x030E9B9C>()();
        }

        static void enableDepthTest() {
            code::Func<void, 0x030E9BB8>()();
        }

        static void pushMatrix() {
            code::Func<void, 0x030E4B3C>()();
        }

        static void scalef(float x, float y, float z) {
            code::Func<void, 0x030E4B6C, float, float, float>()(x, y, z);
        }

        static void translatef(float x, float y, float z) {
            code::Func<void, 0x30E4B54, float, float, float>()(x, y, z);
        }

        static void rotatef(float x, float y, float z) {
            code::Func<void, 0x030E4B84, float, float, float>()(x, y, z);
        }

        static void popMatrix() {
            code::Func<void, 0x030E4BD8>()();
        }

        static void disableTexture() {
            code::Func<void, 0x030E6268>()();
        }

        static int genTexture() {
            return code::Func<int, 0x03124ef8>()();
        }

        static void enableTexture() {
            code::Func<void, 0x030E6294>()();
        }

        static void depthMask(bool arg1) {
            code::Func<void, 0x030E52FC, bool>()(arg1);
        }

        static void ortho(double arg1, double arg2, double arg3, double arg4, double arg5, double arg6) {
            code::Func<void, 0x3110D4C, double, double, double, double, double, double>()(arg1, arg2, arg3, arg4, arg5, arg6);
        }

        static void matrixMode(uint32_t arg1) {
            code::Func<void, 0x3103064, uint32_t>()(arg1);
        }

        static void loadIdentity() {
            code::Func<void, 0x03103080>()();
        }

        static void disableCull() {
            code::Func<void, 0x030F974C>()();
        }

        static void clear(int i) {
            code::Func<void, 0x031078C8, int>()(i);
        }

        static void enableBlend() {
            code::Func<void, 0x30E5284>()();
        }

        static void blendFunc(uint32_t arg1, uint32_t arg2) {
            code::Func<void, 0x30E52A0, uint32_t, uint32_t>()(arg1, arg2);
        }

        static void disableLighting() {
            code::Func<void, 0x030E4B24>()();
        }

        static void color4f(double r, double g, double b, double a) {
            code::Func<void, 0x30E4BA8, double, double, double, double>()(r, g, b, a);
        }

        static void disableFog() {
            code::Func<void, 0x03108C44>()();
        }
    };
}