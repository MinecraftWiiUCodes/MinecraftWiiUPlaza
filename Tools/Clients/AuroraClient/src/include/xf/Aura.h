#pragma once

#include <minecraft/mc/ServerboundMovePlayerPacket.h>
#include <minecraft/mc/ServerboundInteractPacket.h>
#include <minecraft/mc/ServerboundUseItemPacket.h>
#include <minecraft/mc/miscellanious.h>
#include "DrawHelper.h"
#include "WEdit.h"
#include "Players.h"

namespace xf {
    namespace Code {
        class CAura {
        public:
            CAura() {

            }

            void selectNewPlayer() {
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                auto printSelectedPlayer = [&](mc::Player* player) {
                    mstd::wstring playerName = player->name;
                    WorldEdit::sendMessageFormated8("[Aura] Selected Player: %s", char16ToChar(getStr(playerName)));
                };

                bool found = false;
                int playersOnline = 0;
                mc::Player** players = xf::getPlayers(lPlayer, playersOnline);
                for (int i = 0; i < playersOnline; i++) {
                    if (i >= lastSel && !found && players[i] != nullptr && players[i] != lPlayer) {
                        UUID uuid = players[i]->uuid;
                        uuid1 = uuid.part1;
                        uuid2 = uuid.part2;
                        uuid3 = uuid.part3;
                        uuid4 = uuid.part4;
                        lastSel = i + 1;

                        printSelectedPlayer(players[i]);
                        found = true;
                    } else if (lastSel >= playersOnline && !found && players[i] != nullptr && players[i] != lPlayer) {
                        UUID uuid = players[i]->uuid;
                        uuid1 = uuid.part1;
                        uuid2 = uuid.part2;
                        uuid3 = uuid.part3;
                        uuid4 = uuid.part4;
                        lastSel = 1;

                        printSelectedPlayer(players[i]);
                        found = true;
                    }
                }
            }

            void setAtkSpeed(int _atkSpeed) {
                atkSpeed = _atkSpeed;
            }

            void setRange(int _range) {
                range = _range;
            }

            void btnHandler() {
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                uint32_t btn = getButton();
                if (btn & Button::DPAD_DOWN && btn & Button::X) {
                    if (hasButtonPressed) return;
                    
                    mode++;
                    if (mode == 3) mode = 0;

                    // lPlayer->playSound(SOUND_PLING);
                    
                    hasButtonPressed = true;
                } else if (btn & Button::X && btn & Button::L) {
                    if (hasButtonPressed) return;

                    atkSpeed = 0;
                    range = 0;
                    uuid1 = 0;

                    // lPlayer->playSound(SOUND_HARP);

                    hasButtonPressed = true;
                } else if (btn & Button::DPAD_UP && btn & Button::X) {
                    if (hasButtonPressed) return;

                    atkSpeed++;
                    if (atkSpeed > 10) atkSpeed = 0;

                    // lPlayer->playSound(SOUND_PLING);

                    hasButtonPressed = true;
                } else if (btn & Button::DPAD_LEFT && btn & Button::X) {
                    if (hasButtonPressed) return;

                    if (range != 0) range--;

                    // lPlayer->playSound(SOUND_PLING);

                    hasButtonPressed = true;
                } else if (btn & Button::DPAD_RIGHT && btn & Button::X) {
                    if (hasButtonPressed) return;

                    if (mode == 0) range++;
                    if (range > 50) range = 50;

                    // lPlayer->playSound(SOUND_PLING);

                    hasButtonPressed = true;
                } else if (btn & Button::DPAD_RIGHT && mode == 2) {
                    if (hasButtonPressed) return;

                    selectNewPlayer();
                    // lPlayer->playSound(SOUND_PLING);

                    hasButtonPressed = true;
                } else {
                    hasButtonPressed = false;
                }
            }

            void onTick() {
                btnHandler();

                xf::GUI::DrawHelper::renderSetup();
                mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                char16_t* temp = _new<char16_t>(0x60);
                if      (mode == 0) swprintf(temp, 0x60, charToChar16("[Aura]  |  %s  |  AtkSpeed[%d]  Range[%d]"), "Range Aura", atkSpeed, range);
                else if (mode == 1) swprintf(temp, 0x60, charToChar16("[Aura]  |  %s  |  AtkSpeed[%d]"),           "Full Aura",  atkSpeed);
                else if (mode == 2) {
                    mc::Player* player = nullptr;
                    bool foundPlayer = false;

                    int playersOnline = 0;
                    mc::Player** players = xf::getPlayers(lPlayer, playersOnline);
                    for (int i = 0; i < 8; i++) {
                        if (players != nullptr && players[i] != nullptr && players[i] != lPlayer) {
                            UUID uuid = players[i]->uuid;
                            if (uuid1 == uuid.part1 && uuid2 == uuid.part2 && uuid3 == uuid.part3 && uuid4 == uuid.part4 && !foundPlayer) {
                                player = players[i];
                                foundPlayer = true;
                            }
                        }
                    }

                    char16_t* Health = _new<char16_t>(0x60);
                    char* Health8    = _new<char>(0x30);

                    if (foundPlayer) {
                        swprintf(Health, 0x30, charToChar16(" [%s]"), toCStr(20.0f, 2));
                        Health8 = char16ToChar(Health);
                    } else {
                        Health8 = "";
                    } 

                    const char* playeNameText;
                    if (foundPlayer) {
                        playeNameText = char16ToChar(getStr(player->name));
                    } else {
                        playeNameText = "No target";
                    }

                    swprintf(temp, 0x60, charToChar16("[Aura]  |  %s  |  Target[%s%s]  AtkSpeed[%d]"), "Target Aura", playeNameText, Health8, atkSpeed);
                }

                uint32_t width = font->width(temp);
                xf::GUI::DrawHelper::DisplayText(font, temp, 1.0f, toFloat((WIDTH / 2) - (width / 2)), toFloat(HEIGHT - 80));

                tickCount++;
                if (tickCount >= (11 - atkSpeed) && atkSpeed != 0) {
                    if      (mode == 0) RangeAura(lPlayer);
                    else if (mode == 1) FullAura(lPlayer);
                    else if (mode == 2) TargetAura(lPlayer);
                    tickCount = 0x0;
                }
            }

            void TargetAura(mc::LocalPlayer* lPlayer) {
                int playersOnline = 0;
                mc::Player** players = xf::getPlayers(lPlayer, playersOnline);
                mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
                for (int i = 0; i < playersOnline; i++) {
                    if (players[i] != lPlayer && players[i] != nullptr) {
                        UUID uuid = players[i]->uuid;
                        if (uuid1 == uuid.part1 && uuid2 == uuid.part2 && uuid3 == uuid.part3 && uuid4 == uuid.part4) {
                            hitPlayer(players[i], listener);
                        }
                    }
                }

                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundMovePlayerPacket(lPlayer->x, lPlayer->y, lPlayer->z, true, true)));
                listener->send(packet);
            }

            void FullAura(mc::LocalPlayer* lPlayer) {
                int playersOnline = 0;
                mc::Player** players = xf::getPlayers(lPlayer, playersOnline);
                mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
                for (int i = 0; i < playersOnline; i++) {
                    if (players[i] != lPlayer && players[i] != nullptr) {
                        hitPlayer(players[i], listener);
                    }
                }

                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundMovePlayerPacket(lPlayer->x, lPlayer->y, lPlayer->z, true, true)));
                listener->send(packet);
            }

            void RangeAura(mc::LocalPlayer* lPlayer) {
                int playersOnline = 0;
                mc::Player** players = xf::getPlayers(lPlayer, playersOnline);
                mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
                for (int i = 0; i < playersOnline; i++) {
                    if (players[i] != lPlayer && players[i] != nullptr) {
                        if (range >= sqrt(
                            pow(lPlayer->x - players[i]->x, 2) + 
                            pow(lPlayer->y - players[i]->y, 2) + 
                            pow(lPlayer->z - players[i]->z, 2)
                        )) hitPlayer(players[i], listener);
                    }
                }

                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundMovePlayerPacket(lPlayer->x, lPlayer->y, lPlayer->z, true, true)));
                listener->send(packet);
            }

            void hitPlayer(mc::Player* player, mc::ClientPacketListener* listener) {
                mc::Minecraft::getInstance()->thePlayer->swing();
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundMovePlayerPacket(player->x, player->y, player->z, true, true)));
                listener->send(packet);

                packet = _new1(mc::ServerboundInteractPacket((uint32_t) player));
                listener->send(packet);
            }

            void reset() {
                atkSpeed = 0;
                tickCount = 0;
                range = 0;
                mode = 0;
                hasButtonPressed = false;
            }

        private:   
            uint32_t uuid1 = 0x00000000;
            uint32_t uuid2 = 0x00000000;
            uint32_t uuid3 = 0x00000000;
            uint32_t uuid4 = 0x00000000;
            uint32_t lastSel = 0x00000000;
            uint32_t atkSpeed;
            uint32_t tickCount;
            uint32_t range;
            uint32_t mode;
            bool hasButtonPressed;
        };
    }
}