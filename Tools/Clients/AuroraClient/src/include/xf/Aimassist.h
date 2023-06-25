#pragma once

#include "Aimbot.h"
#include "PerlinNoise.h"

namespace xf {
    namespace Code {
        class AimAssist {
        public:
            AimAssist() {

            }

            void setRange(int r)      { range = r;    }
            void setStrength(int t) { strength = t; }

            void onTick() {
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                mc::Player* selectedPlayer = nullptr;
                int playersOnline = 0;
                mc::Player** players = xf::getPlayers(lPlayer, playersOnline);

                if (players == nullptr) return;
                for (uint32_t i = 0; i < 8; i++) {
                    if (players[i] != nullptr && lPlayer != players[i]) {
                        double distanceNewPlayer = Aimbot::getDistance(lPlayer, players[i]);
                        if (distanceNewPlayer <= toFloat(range)) {
                            if (selectedPlayer == nullptr) {
                                selectedPlayer = players[i];
                            } else {
                                double distance = Aimbot::getDistance(lPlayer, selectedPlayer);
                                if (distanceNewPlayer <= distance) {
                                    selectedPlayer = players[i];
                                }
                            }
                        }
                    }
                }

                if (selectedPlayer == nullptr) return;
                
                double targetEyeHeight = selectedPlayer->y + selectedPlayer->getEyeHeight();
                double diffX = selectedPlayer->x - lPlayer->x;
    	        double diffY = targetEyeHeight   - (lPlayer->y + lPlayer->getEyeHeight());
	            double diffZ = selectedPlayer->z - lPlayer->z;

                double dist = sqrt(pow(diffX, 2) + pow(diffZ, 2));
		        float pitch = WorldEdit::radianToDegree((float) -(atan2(diffY, dist)));

                float prevYaw   = lPlayer->yaw;
                float prevPitch = lPlayer->pitch;

                lPlayer->yaw   = Noise::PerlinNoise::lerp(1.0f - (toFloat(strength) / toFloat(100)), prevYaw, Aimbot::calculateYaw(lPlayer, selectedPlayer));
                lPlayer->pitch = Noise::PerlinNoise::lerp(1.0f - (toFloat(strength) / toFloat(100)), prevPitch, pitch);
            }

        private:
            int range = 0;
            int strength = 0;
        };
    }
}