#pragma once

#include <code/code.h>
#include <inttypes.h>

namespace mc {
    class BlockPos {
	public:
		BlockPos(int _x, int _y, int _z) { 
			code::Func<BlockPos*, 0x020c3ad4, BlockPos*, int, int, int>()(this, _x, _y, _z); 
	  	}

		BlockPos() = default;
	  	
        int x;
        int y;
        int z;
	};
}