#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "miscellanious.h"
#include "Player.h"

#include "../mstd/WString.h"

namespace mc {
    class GiveItemCommand {
    public:
        static int preparePacket(mboost::shared_ptr<Player> player, int id, int aux, int amount, const mstd::wstring& unk) {
            return code::Func<int, 0x02460e54, int, mboost::shared_ptr<Player>, int, int, int, const mstd::wstring&>()(0, player, id, aux, amount, unk);
        }
        
    };
}