#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "../mstd/WString.h"

namespace mc {
    class Font {
    public:
		int width(mstd::wstring wstr) {
			return code::Func<uint32_t, 0x30E919C, Font*, mstd::wstring>()(this, wstr);
		}

		int width(mstd::wstring* wstr) {
			return code::Func<uint32_t, 0x30E919C, Font*, mstd::wstring*>()(this, wstr);
		}

		void drawShadow(mstd::wstring wstr, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
			code::Func<void, 0x3126B88, Font*, mstd::wstring, uint32_t, uint32_t, uint32_t>()(this, wstr, arg1, arg2, arg3);
		}

		void drawShadow(mstd::wstring* wstr, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
			code::Func<void, 0x3126B88, Font*, mstd::wstring*, uint32_t, uint32_t, uint32_t>()(this, wstr, arg1, arg2, arg3);
		}

		void draw(mstd::wstring wstr, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
			code::Func<void, 0x030E9A10, Font*, mstd::wstring, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>()(this, wstr, arg1, arg2, arg3, 0, 1);
		}

		void draw(mstd::wstring* wstr, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
			code::Func<void, 0x030E9A10, Font*, mstd::wstring*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>()(this, wstr, arg1, arg2, arg3, 0, 1);
		}
    };
}