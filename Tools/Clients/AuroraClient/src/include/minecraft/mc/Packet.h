#pragma once

#include <inttypes.h>

namespace mc {
    class Packet {
	public:
		uint64_t time;
		uint8_t unk_1;
		uint8_t unk_2;
		uint8_t unk_3;
		uint8_t unk_4;
		void* vtbl;    
	};
}