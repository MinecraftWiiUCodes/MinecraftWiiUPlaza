#pragma once

#include <code/code.h>
#include <inttypes.h>

#include "Player.h"

namespace mc {
    // 03299F8C
    class ServerPlayer : public Player {
    public:

        void disconnect() {
            code::Func<void, 0x03286798, ServerPlayer*>()(this); 
        }

        bool isSpectator() {
            return code::Func<bool, 0x032d8000, ServerPlayer*>()(this);   
        }
        bool isCreative()  {
            return code::Func<bool, 0x032d8060, ServerPlayer*>()(this);   
        }

        uint32_t field_0x868;
        uint32_t field_0x86C;
        uint32_t field_0x874;
        uint32_t field_0x878;
        uint32_t field_0x87C;
        uint32_t field_0x890;
        uint32_t field_0x894;
        uint32_t field_0x898;
        uint32_t field_0x89C;
        uint32_t field_0x8A0;
        uint32_t field_0x8A4;
        uint32_t field_0x8A8;
        uint32_t field_0x8AC;
        uint32_t field_0x8B0;
        uint32_t field_0x8B4;
        uint32_t field_0x8B8;
        uint32_t field_0x8BC;
        uint32_t field_0x8C0;
        uint32_t field_0x8C4;
        uint32_t field_0x8C8;
        uint32_t field_0x8CC;
        uint32_t field_0x8D0;
        uint32_t field_0x8D4;
        uint32_t field_0x8D8;
        uint32_t field_0x8DC;
        uint32_t field_0x8E0;
        uint32_t field_0x8E4;
        uint32_t field_0x8E8;
        uint32_t field_0x8EC;
        uint32_t field_0x8F0;
        uint32_t field_0x8F4;
        uint32_t field_0x8F8;
        uint32_t field_0x8FC;
        uint32_t field_0x900;
        uint32_t field_0x904;
        uint32_t field_0x908;
        uint32_t field_0x90C;
        uint32_t field_0x910;
        uint32_t field_0x914;
        uint32_t field_0x918;
        uint32_t field_0x91C;
        uint32_t field_0x920;
        uint32_t field_0x924;
        uint32_t field_0x928;
        uint32_t field_0x92C;
        uint32_t field_0x930;
        uint32_t field_0x934;
        uint32_t field_0x938;
        uint32_t field_0x93C;
        uint32_t field_0x940;
        uint32_t field_0x944;
    };
}