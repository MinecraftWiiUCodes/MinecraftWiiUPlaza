#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "BlockPos.h"

namespace mc {
    class Level {
	public:
		void destroyBlock(BlockPos const& pos, int a) { destroyBlock_(this, pos, a); }
		static code::Func<void, 0x0254b014, Level*, BlockPos const&, int> destroyBlock_;

		uint32_t getBlockId(int x, int y, int z) {
			return code::Func<uint32_t, 0x0254CCAC, Level*, int, int, int>()(this, x, y, z);
		}

		uint32_t getBlockData(int x, int y, int z) {
			return code::Func<uint32_t, 0x0254D800, Level*, int, int, int>()(this, x, y, z);
		}

		int isInWorldBounds(int x, int y, int z) {
			return code::Func<int, 0x02546BB4, Level*, int, int, int>()(this, x, y, z);
		}

	};
}