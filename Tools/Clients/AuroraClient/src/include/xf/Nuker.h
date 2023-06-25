#pragma once

#include <code/code.h>
#include <inttypes.h>

#include <minecraft/mc/Minecraft.h>
#include <minecraft/mc/ServerboundSetCreativeModeSlotPacket.h>
#include <minecraft/mc/ServerboundPlayerActionPacket.h>
#include <minecraft/mc/ServerboundMovePlayerPacket.h>
#include <minecraft/mc/ServerboundUseItemOnPacket.h>

#include <minecraft/mc/miscellanious.h>
#include <minecraft/mc/ItemInstance.h>
#include <minecraft/mc/Item.h>

#include "WEdit.h"

namespace xf {
    namespace Code {
        class Nuker {
        public:
            Nuker() {

            }

            void breakBlocks() {
                for (int x = -r; x < r + 1; x++) {
                    for (int y = -r; y < r + 1; y++) {
                        for (int z = -r; z < r + 1; z++) {
                            mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                            int* t = _new<int>(3);
                            t[0] = toInt(lPlayer->x) + x;
                            t[1] = toInt(lPlayer->y) + y;
                            t[2] = toInt(lPlayer->z) + z;
                            if (toFloat(r) > sqrtf(powf(toFloat(t[0]) - lPlayer->x, 2) + powf(toFloat(t[1]) - lPlayer->y, 2) + powf(toFloat(t[2]) - lPlayer->z, 2))) {
                                if (lPlayer->lvl->getBlockId(t[0], t[1], t[2]) != 0) xf::Code::WorldEdit::we_tp        (t[0], t[1], t[2]);
                                xf::Code::WorldEdit::we_breakBlock(t[0], t[1], t[2]);
                            }
                        }
                    }
                }
            }

            void onBtn() {
                uint32_t btn = getButton();
                if (btn & Button::DPAD_UP) {
                    if (active) return;

                    active = true;
                } else if (btn & Button::DPAD_LEFT) {
                    if (active) return;

                    r--;
                    if (0 > r) r = 0;

                    xf::Code::WorldEdit::sendMessageFormated8("[Nuker] Set Radius to %d -> %d", r+1, r);
                    
                    active = true;
                } else if (btn & Button::DPAD_RIGHT) {
                    if (active) return;

                    r++;

                    xf::Code::WorldEdit::sendMessageFormated8("[Nuker] Set Radius to %d -> %d", r-1, r);

                    active = true;
                } else {
                    active = false;
                }
            }

            void onTick() {
                onBtn();
            
            
                breakBlocks();
            }


        private:
            int r = 5;
            bool active = false;
        };
    }
}