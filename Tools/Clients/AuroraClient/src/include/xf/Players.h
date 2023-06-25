#pragma once

#include <minecraft/mc/miscellanious.h>
#include <minecraft/mc/LocalPlayer.h>
#include <minecraft/mc/Player.h>

namespace xf {
    static mc::Player** getPlayers(mc::LocalPlayer* lPlayer, int& onlineCount) {
        mc::Player* players[8] = {};
        int playersOnline = 0;
        for (int i = 0; i < 8; i++) {
            players[i] = nullptr;
        }

        auto check = [&](uint32_t ptr) {
            if (ptr < 0x10000000) return true;
            if (ptr > 0x40000000) return true;

            return false;
        };

        uint32_t a = code::Mem((uint32_t) lPlayer + 0x70).as<uint32_t>();
        if (check(a)) return nullptr;
        mboost::shared_ptr<mc::Player>* b = code::Mem(a + 0xC8).as<mboost::shared_ptr<mc::Player>*>();
        if (check((uint32_t) b)) return nullptr;
        for (int i = 0; i < 8; i++) {
            mc::Player* player = b[i].ptr;
            bool continue_ = !check((uint32_t) player);

            if (continue_ && (player->uuid.part1 == 0x0 && player->uuid.part2 == 0x0)) continue_ = false; 
            if (continue_ && player->colourIndex > 7)                                  continue_ = false;

        	if (continue_ && (code::Mem((uint32_t) player + 0x718).as<uint32_t>() == 0x3DCCCCCD)) {
                players[playersOnline++] = player;
            }
        }

        for (int i = 0; i < 8; i++) {
            bool isDouble = false;
            for (int j = 0; j < 8; j++) {
                if (players[i] == players[j] && i != j && players[i] != nullptr) isDouble = true;
            }

            if (isDouble) {
                players[i] = nullptr;
                playersOnline--;
            }
        }

        onlineCount = playersOnline;

        auto sort = [&](mc::Player** list, int size) {
            mc::Player** playerList = _new<mc::Player*>(0x8);
            for (int i = 0; i < 8; i++)    playerList[i] = nullptr;
            for (int i = 0; i < size; i++) playerList[i] = list[i];

            mc::Player* temp;
            for (int i = 0; i < size; i++) {
                for (int j = i + 1; j < size; j++) {
                    int iLength = playerList[i]->colourIndex;
                    int jLength = playerList[j]->colourIndex;

                    if (jLength < iLength) {
                        temp = playerList[i];
                        playerList[i] = playerList[j];
                        playerList[j] = temp;
                    }
                }
            }

            return playerList;
        };

        mc::Player** playersList = _new<mc::Player*>(0x8);
        for (int i = 0; i < 8; i++) playersList[i] = nullptr;

        int index = 0;
        for (int i = 0; i < 8; i++) {
            if (players[i] != nullptr) playersList[index++] = players[i];
        }

        return sort(playersList, index);
    }
}