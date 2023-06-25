#pragma once

#include "WEdit.h"
#include "DrawHelper.h"
#include "Players.h"
#include <minecraft/mc/miscellanious.h>

namespace xf {
    namespace Code {
        class Aimbot {
        public:
            Aimbot() {

            }

            static double getDistance(mc::Player* player1, mc::Player* player2, bool fromEyePos = false) {
                double x = player1->x - player2->x;
                double y;
                if (fromEyePos) {
                    y = (player1->y + player1->getEyeHeight()) - player2->y;
                } else {
                    y = player1->y - player2->y;
                }
                double z =  player1->z - player2->z;
                return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
            }

            static float calculateYaw(mc::Player* lPlayer, mc::Player* target) {
                double diffX = target->x - lPlayer->x;
                double diffZ = target->z - lPlayer->z;
                float yaw = (float) atan2(diffZ, diffX) * (180.0 / M_PI) - 90.0f;
                if (yaw < 0) {
                    yaw += 360.0f;
                } else if (yaw > 360.0f) {
                    yaw -= 360.0f;
                }
                float yawDelta = yaw - lPlayer->yaw;
                if (yawDelta > 180.0f) {
                    yawDelta -= 360.0f;
                } else if (yawDelta < -180.0f) {
                    yawDelta += 360.0f;
                }
                return lPlayer->yaw + yawDelta;
            }

            void BowAimbot(mc::LocalPlayer* lPlayer, mc::Player* target) {
                // static BowItem::getPowerForTime(int)
                uint32_t n = lPlayer->bowPower;
                double m = code::Func<double, 0x02162F50, uint32_t>()(72000 - n);
                double g = 0.006;
                double diffX  =  target->x - lPlayer->x;
                double diffZ  =  target->z - lPlayer->z;
                double diffY  = (target->y - lPlayer->y) * 2;
                double distZX = sqrt(pow(diffZ, 2) + pow(diffX, 2));
                double pitch  = (atan2(
                    pow(m, 2) - sqrt((pow(distZX, 2) * g  + diffY * pow(m, 2)) * g + pow(m, 4)), 
                    distZX * g
                ) * 180.0 / M_PI);

                float fPitch = ((float) pitch);
                if (fPitch > 90.0f || -90.0f > fPitch || isnan(fPitch)) fPitch = 0.0f;
                lPlayer->yaw   = calculateYaw(lPlayer, target);
                lPlayer->pitch = fPitch;
            }

            void onTick() {
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                mc::Player* selectedPlayer = nullptr;
                int playersOnline = 0;
                mc::Player** players = xf::getPlayers(lPlayer, playersOnline);

                mboost::shared_ptr<mc::ItemInstance> item{};
                lPlayer->getCarriedItem(item);
                uint32_t itemID = mc::Item::getId(item->item);

                if (players == nullptr) return;
                for (uint32_t i = 0; i < 8; i++) {
                    if (players[i] != nullptr && lPlayer != players[i]) {
                        double distanceNewPlayer = getDistance(lPlayer, players[i]);
                        float range = itemID == Items::BOW_ID ? bRange : fRange;
                        if (distanceNewPlayer <= range) {
                            if (selectedPlayer == nullptr) {
                                selectedPlayer = players[i];
                            } else {
                                double distance = getDistance(lPlayer, selectedPlayer);
                                if (distanceNewPlayer <= distance) {
                                    selectedPlayer = players[i];
                                }
                            }
                        }
                    }
                }

                if (selectedPlayer == nullptr) return;
                if (itemID == Items::BOW_ID) {
                    BowAimbot(lPlayer, selectedPlayer);
                } else {
                    double targetEyeHeight = selectedPlayer->y + selectedPlayer->getEyeHeight();
                    double diffX = selectedPlayer->x - lPlayer->x;
		            double diffY = targetEyeHeight   - (lPlayer->y + lPlayer->getEyeHeight());
	                double diffZ = selectedPlayer->z - lPlayer->z;

                    double dist = sqrt(pow(diffX, 2) + pow(diffZ, 2));
		            float pitch = WorldEdit::radianToDegree((float) -(atan2(diffY, dist)));

                    lPlayer->yaw   = calculateYaw(lPlayer, selectedPlayer);
                    lPlayer->pitch = pitch;
                }
            }

            float getRange() {
                return fRange;
            }

        private:
            float fRange = 32.0f;  //     Aimbot Range
            float bRange = 128.0f; // Bow Aimbot Range
        };
    }
}