#pragma once

// If ID is less than Zero then its a Normal Module
// If ID is More than Zero then its a Button

#define CONFIG_FILE_NAME u"Aurora.cfg"

#define RANDOM_NNID_ID 1
#define RANDOM_NNID_TEXT u"Random NNID"
#define INSTANT_CRAFT_ID 2
#define INSTANT_CRAFT_TEXT u"Instant Craft"
#define STEP_ID -3
#define STEP_TEXT u"Step <%d>"
#define BHOP_ID -4
#define BHOP_TEXT u"Bunny Hop <%d>"
#define SHOW_POSITION_ID -5
#define SHOW_POSITION_TEXT u"Show Position"
#define ANTI_KB_ID -6
#define ANTI_KB_TEXT u"Anti Knockback"
#define FLY_ID -7
#define FLY_TEXT u"Fly"
#define RUN_AND_EAT_ID -8
#define RUN_AND_EAT_TEXT u"Run & Eat"
#define FULLBRIGHT_ID -9
#define FULLBRIGHT_TEXT u"Fullbright"
#define ALL_GAMEMODE_ID -10
#define ALL_GAMEMODE_TEXT u"All Gamemodes"
#define NO_CLIP_ID -11
#define NO_CLIP_TEXT u"No Clip"
#define SWAP_HANDS_ID 12
#define SWAP_HANDS_TEXT u"Swap Hands"
#define AURA_ID -13
#define AURA_TEXT u"Aura"
#define NUKER_ID -14
#define NUKER_TEXT u"Nuker"
#define AUTOCLICKER_ID -15
#define AUTOCLICKER_TEXT u"Auto Clicker"
#define BOW_SPAM_ID -16
#define BOW_SPAM_TEXT u"Bow Spam"
#define AUTO_SPRINT_ID -17
#define AUTO_SPRINT_TEXT u"Auto Sprint"
#define SPEED_ID -18
#define SPEED_TEXT u"Speed <%d>"
#define WORLDEDIT_ID -20
#define WORLDEDIT_TEXT u"World Edit"
#define AIMBOT_ID -21
#define AIMBOT_TEXT u"Aimbot"
#define NIGHT_VISION_ID -22
#define NIGHT_VISION_TEXT u"Night Vision"
#define BOOST_PLAYER_ID 25
#define BOOST_PLAYER_TEXT u"Boost Player 7x"
#define AIMASSIST_ID 26
#define AIMASSIST_TEXT u"Aimassist"
#define RANDOM_XUID_ID 27
#define RANDOM_XUID_TEXT u"Random XUID"
#define KEYSTROKES_ID -28
#define KEYSTROKES_TEXT u"Key Strokes"
#define SAVE_CONFIG_ID 29
#define SAVE_CONFIG_TEXT u"Save Config"
#define CLIENT_NAME_ID 30
#define CLIENT_NAME_TEXT u"Client Name"

#define ARRAY_LIST_ID -99
#define ARRAY_LIST_TEXT u"Array List"
#define CREDITS_ID 100
#define CREDITS_TEXT u"Credits"

#define POSITION_X_ID -200
#define POSITION_X_TEXT u"Position X <%d>"
#define POSITION_Y_ID -201
#define POSITION_Y_TEXT u"Position Y <%d>"
#define RGB_SPEED_ID -202
#define RGB_SPEED_TEXT u"RGB Speed <%d>"

#define PLAYER_TEXT u"%s"
#define PLAYER_ALL_TEXT u"Everyone"
#define PLAYER_1_ID   101 
#define PLAYER_2_ID   102
#define PLAYER_3_ID   103
#define PLAYER_4_ID   104
#define PLAYER_5_ID   105
#define PLAYER_6_ID   106
#define PLAYER_7_ID   107
#define PLAYER_8_ID   108
#define PLAYER_ALL_ID 109

#define AIMASSIST_STRENGTH_ID 120
#define AIMASSIST_STRENGTH_TEXT u"Strength <%d>"
#define AIMASSIST_RANGE_ID 121
#define AIMASSIST_RANGE_TEXT u"Range <%d>"
#define AIMASSIST_ENABLE_ID 122
#define AIMASSIST_ENABLE_TEXT u"Enable"

#define ID_ID -30
#define ID_TEXT u"ID <%d>"
#define AMOUNT_ID -31
#define AMOUNT_TEXT u"Amount <%d>"
#define AUX_ID -32
#define AUX_TEXT u"Aux <%d>"
#define GIVE_ID 33
#define GIVE_TEXT u"Give Item"
#define GIVE_ALL_ID 34
#define GIVE_ALL_TEXT u"Give All"
#define TELEPORT_TO_ID 35
#define TELEPORT_TO_TEXT u"Teleport to Player"
#define HIT_PLAYER_ID 37
#define HIT_PLAYER_TEXT u"Hit Player"

#include <minecraft/mc/CraftItemPacket.h>
#include <minecraft/mc/miscellanious.h>

#include <minecraft/mc/FileOutputStream.h>
#include <minecraft/mc/FileInputStream.h>
#include <minecraft/mc/System.h>

#include "DrawHelper.h"
#include "Keystrokes.h"
#include "PathWalker.h"
#include "Aimassist.h"
#include "Aimbot.h"
#include "WEdit.h"
#include "Nuker.h"
#include "Aura.h"

namespace xf {
    namespace GUI {
        class MessageNotif {
        public:
            MessageNotif(const char16_t* title_, const char16_t* message_) {
                mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                id = 0;
                
                title = _new2(mstd::wstring(title_));
                message = _new2(mstd::wstring(message_));
            }

            mstd::wstring* getTitle()   { return title;   }
            mstd::wstring* getMessage() { return message; }
            int getID()                 { return id;      }
        
        private:
            mstd::wstring* title;
            mstd::wstring* message;
            int id; 
        };

        class Module {
        public:
            Module(const char16_t* newTitle, int newID) {
                title = _new2(mstd::wstring(newTitle));
                isActive = false;
                id = newID;
            }

            bool isEnabled()         { return isActive;      }
            mstd::wstring* getName() { return title;         }
            int getID()              { return id;            }
            void toggle()            { isActive = !isActive; }
            void setID(int i)        { id = i;               }

        private:
            mstd::wstring* title;
            bool isActive;
            int id;
        };

        class Page {
        public:
            Page(const char16_t* newTitle, int newID) {
                title = _new2(mstd::wstring(newTitle));
                id = newID;
            }

            mstd::wstring* getName()      { return title;          }
            int getID()                   { return id;             }
            int getModuleCount()          { return moduleCount;    }
            int getSelectedModule()       { return selectedModule; }
            void setSelectedModule(int i) { selectedModule = i;    }
            void goUP() {
                if (selectedModule == moduleCount - 1) {
                    selectedModule = 0;
                } else {
                    selectedModule++;
                }
            }

            void goDown() {
                if (selectedModule == 0) {
                    selectedModule = moduleCount - 1;
                } else {
                    selectedModule--;
                }
            }

            void setModules(Module* newModules, int _moduleCount) {
                modules = newModules;
                moduleCount = _moduleCount;
                selectedModule = -1;
            }

            Module  operator[](int i) const { return modules[i]; }
            Module &operator[](int i)       { return modules[i]; }

        private:
            mstd::wstring* title;
            int id;
            Module* modules;
            int moduleCount;
            int selectedModule;
        };

        class GUI {
        public:
            GUI(mstd::wstring* newTitle) : title(newTitle) {
                hide();
                setPosX(0.0f);
                setPosY(0.0f);
                setSizeX(10.0f);
                setSizeY(10.0f);
                setOutLineSize(1.0f);
                title = newTitle;
            }

            GUI() = default;

            void onBtn() {
                uint32_t btn = getButton();
                mc::time currentTime = mc::System::processTimeInMilliSecs();
                uint32_t minTime = 700;
                uint32_t clickAmount = 20;
                uint32_t clickTime = 1000 / clickAmount;

                if (btn & Button::L && btn & Button::R && btn & Button::DPAD_UP) {
                    if (wasButtonPressed) return;
    
                    isShowing = !isShowing;
                    isShowing ? show() : hide();

                    wasButtonPressed = true;
                } else if (btn & Button::DPAD_UP && isShowing) {
                    if (minTime   > (currentTime - lastTime)      && wasButtonPressed) return;
                    if (clickTime > (currentTime - lastClickTime) && wasButtonPressed) return;

                    if (currentPageOpened == -1) {
                        if (selectedPage == 0) {
                            selectedPage = pageCount - 4;
                        } else {
                            selectedPage--;
                        }
                    } else {
                        int index = pages[currentPageOpened].getSelectedModule();
                        Module* module = &pages[currentPageOpened][index];

                        pages[currentPageOpened].goDown();
                    }

                    wasButtonPressed = true;
                    lastClickTime = currentTime;
                } else if (btn & Button::DPAD_DOWN && isShowing) {
                    if (minTime   > (currentTime - lastTime)      && wasButtonPressed) return;
                    if (clickTime > (currentTime - lastClickTime) && wasButtonPressed) return;

                    if (currentPageOpened == -1) {
                        if (selectedPage == pageCount - 4) {
                            selectedPage = 0;
                        } else {
                            selectedPage++;
                        }
                    } else {
                        int index = pages[currentPageOpened].getSelectedModule();
                        Module* module = &pages[currentPageOpened][index];

                        pages[currentPageOpened].goUP();
                    }

                    wasButtonPressed = true;
                    lastClickTime = currentTime;
                } else if (btn & Button::DPAD_RIGHT && isShowing) {
                    if (minTime   > (currentTime - lastTime)      && wasButtonPressed) return;
                    if (clickTime > (currentTime - lastClickTime) && wasButtonPressed) return;

                    if (currentPageOpened == -1) {
                        currentPageOpened = selectedPage;
                    } else {
                        int index = pages[currentPageOpened].getSelectedModule();
                        Module* module = &pages[currentPageOpened][index];
                        
                        if (module->getID() == SPEED_ID) {
                            speedMultiplier++;
                            if (speedMultiplier > 20) speedMultiplier = 20;
                        } else if (module->getID() == BHOP_ID) {
                            bhopMultiplier++;
                            if (bhopMultiplier > 10) bhopMultiplier = 10;
                        } else if (module->getID() == STEP_ID) {
                            stepMultiplier++;
                            if (stepMultiplier > 100) stepMultiplier = 100;
                        } else if (module->getID() == POSITION_X_ID) {
                            x += 5;
                            if (x > WIDTH - sizeX) x = WIDTH - sizeX;
                        } else if (module->getID() == POSITION_Y_ID) {
                            y += 5;
                            if (y > HEIGHT - sizeY) y = HEIGHT - sizeY;
                        } else if (module->getID() == RGB_SPEED_ID) {
                            rgbSpeed++;
                            if (rgbSpeed > 6) rgbSpeed = 6;
                        } else if (module->getID() == ID_ID) {
                            giveID++;
                        } else if (module->getID() == AMOUNT_ID) {
                            giveAmount++;
                        } else if (module->getID() == AUX_ID) {
                            giveAux++;
                        } else if (module->getID() == AIMASSIST_STRENGTH_ID) {
                            if (aimassistStrength != 100) aimassistStrength++;
                        } else if (module->getID() == AIMASSIST_RANGE_ID) {
                            aimassistRange++;
                        } else {
                            if (currentPageOpened == 7 && module->getID() >= PLAYER_1_ID && module->getID() < PLAYER_ALL_ID) {
                                currentPageOpened = 8; 
                                selectedPlayer = module->getID() - PLAYER_1_ID;
                            } else if (currentPageOpened == 7 && module->getID() == PLAYER_ALL_ID) {
                                currentPageOpened = 9;
                                selectedPlayer = 10;
                            } else if (currentPageOpened == 0 && module->getID() == AIMASSIST_ID) {
                                currentPageOpened = 10;
                            } else {
                                module->toggle();
                            } 
                        }
                    }

                    wasButtonPressed = true;
                    lastClickTime = currentTime;
                } else if (btn & Button::DPAD_LEFT && isShowing) {
                    if (minTime   > (currentTime - lastTime)      && wasButtonPressed) return;
                    if (clickTime > (currentTime - lastClickTime) && wasButtonPressed) return;

                    if (currentPageOpened != -1) {
                        int index = pages[currentPageOpened].getSelectedModule();
                        Module* module = &pages[currentPageOpened][index];
                        if (module->getID() == SPEED_ID) {
                            if (speedMultiplier == 1) {
                                currentPageOpened = -1;
                            } else if (speedMultiplier != 1) {
                                speedMultiplier--;
                            }
                        } else if (module->getID() == BHOP_ID) {
                            if (bhopMultiplier == 1) {
                                currentPageOpened = -1;
                            } else if (bhopMultiplier != 1) {
                                bhopMultiplier--;
                            }
                        } else if (module->getID() == STEP_ID) {
                            if (stepMultiplier == 1) {
                                currentPageOpened = -1;
                            } else if (stepMultiplier != 1) {
                                stepMultiplier--;
                            }
                        } else if (module->getID() == POSITION_X_ID) {
                            if (x == 0) {
                                currentPageOpened = -1;
                            } else if (x != 0) {
                                x -= 5;
                            }
                        } else if (module->getID() == POSITION_Y_ID) {
                            if (y == 0) {
                                currentPageOpened = -1;
                            } else if (y != 0) {
                                y -= 5;
                            }
                        } else if (module->getID() == RGB_SPEED_ID) {
                            if (rgbSpeed == 1) {
                                currentPageOpened = -1;
                            } else if (rgbSpeed != 1) {
                                rgbSpeed--;
                            }
                        } else if (module->getID() == ID_ID) {
                            if (giveID > 0) giveID--;
                        } else if (module->getID() == AMOUNT_ID) {
                            if (giveAmount > 0) giveAmount--;
                        } else if (module->getID() == AUX_ID) {
                            if (giveAux > 0) giveAux--;
                        } else if (module->getID() == AIMASSIST_STRENGTH_ID) {
                            if (aimassistStrength > 0) aimassistStrength--;
                        } else if (module->getID() == AIMASSIST_RANGE_ID) {
                            if (aimassistRange > 0) aimassistRange--;
                        } else {
                            if (currentPageOpened == 8 || currentPageOpened == 9) currentPageOpened = 7;
                            else if (currentPageOpened == 10)                     currentPageOpened = 0;
                            else                                                  currentPageOpened = -1;
                        }
                    } else {
                        hide();
                    }

                    wasButtonPressed = true;
                    lastClickTime = currentTime;
                } else {
                    wasButtonPressed = false; 
                    lastTime = currentTime;
                }
            }

            void saveConfig(bool notific = true) {
                mc::File file = mc::File(CONFIG_FILE_NAME);
                mc::FileOutputStream* ostr = _new1(mc::FileOutputStream(file));
                ostr->write(xf::String::intToStr(toInt(x)));
                ostr->write(',');
                ostr->write(xf::String::intToStr(toInt(y)));
                ostr->write(',');
                ostr->write(xf::String::intToStr(rgbSpeed));
                ostr->write(',');

                int m = 0;
                Page page = pages[3];
                for (int i = 0; i < page.getModuleCount(); i++) {
                    Module* module = &page[i];
                    int n = module->isEnabled();
                    m |= n << i;
                }
                ostr->write(xf::String::intToStr(m));
                ostr->write(',');

                page = pages[6];
                for (int i = 0; i < page.getModuleCount(); i++) {
                    xf::GUI::Module* module = &page[i];
                    if (module->getID() == CLIENT_NAME_ID) ostr->write(xf::String::intToStr(module->isEnabled()));
                }

                ostr->close();

                if (notific) setNotification(u"Config", u"Saved Config...");
            }

            void execCommand(mc::Player* player, mc::LocalPlayer* lPlayer) {
                if (player == nullptr) return;
                code::Func<int, 0x02460E54, uint32_t, mc::Player*, int, int, int, mboost::shared_ptr<uint32_t>&>()(0x10303000, player, giveID, giveAmount, giveAux, lPlayer->field_0x740);
                mc::Minecraft::getInstance()->getConnection(0)->sendCommand(0x10303000);
            }

            void drawPlayerInfo() {
                mc::Minecraft* minecraft = mc::Minecraft::getInstance();
                mc::Font* font = minecraft->defaultFonts;
                mc::LocalPlayer* lPlayer = minecraft->thePlayer;
                int onlinePlayers = 0;
                mc::Player** players = xf::getPlayers(lPlayer, onlinePlayers);
                mc::Player* player = players[selectedPlayer];
                if (player == nullptr) return;

                auto getHealth = [&]() {
                    uint32_t a = code::Mem((uint32_t) player + 0x1F8).as<uint32_t>();
                    if (a == 0) return 0.0f;
                    uint32_t b = code::Mem(a + 0x30).as<uint32_t>();
                    if (b == 0) return 0.0f;
                    return code::Mem(b + 0xC).as<float>();
                };

                auto getHealthColor = [&](float h) {
                    if (h >= toFloat(10)) return 0xFF55FF55;
                    if (h >= toFloat(5))  return 0xFFFFFF55;
                    if (h >= toFloat(0))  return 0xFFAA0000;
                    
                    return 0xFFFFFFFF;
                };

                float dist = sqrtf(powf(player->x - lPlayer->x, 2) + powf(player->y - lPlayer->y, 2) + powf(player->z - lPlayer->z, 2));
                float hp = getHealth();
                
                char16_t* posText   = formatText(u"(%d, %d, %d)", toInt(player->x), toInt(player->y), toInt(player->z));
                char16_t* nameText  = u"Name: ";
                char16_t* nameText2 = getStr(player->name);
                char16_t* distText  = formatText(u"Distance: %s", toCStr(dist, 3));
                char16_t* hpText    = u"HP: ";
                char16_t* hpText1   = formatText(u"%s", toCStr(hp, 2));
                char16_t* hpText2   = u"/";
                char16_t* hpText3   = formatText(u"%s", toCStr(toFloat(20), 2));
                int posTextWidth = font->width(posText);
                int longestWidth = 45;
                char16_t* texts[4] = { posText, nameText, distText, hpText };
                for (int i = 0; i < 4; i++) if (font->width(texts[i]) > longestWidth) longestWidth = font->width(texts[i]);
                int _temp = font->width(nameText) + font->width(nameText2);
                if (_temp > longestWidth) longestWidth = _temp;
                _temp = font->width(hpText) + font->width(hpText1) + font->width(hpText2) + font->width(hpText3);
                if (_temp > longestWidth) longestWidth = _temp;

                float _size = 56.0f;
                float _sizeX = 14.0f + toFloat(longestWidth);
                mc::GlStateManager::disableTexture();
                mc::GlStateManager::color4f(1, 1, 1, 1);
                xf::GUI::DrawHelper::DisplayBox2D(x + sizeX, y, _sizeX, _size, xf::GUI::DrawHelper::getRainbowRGB(xf::GUI::TextColorIndex) & 0xFFFFFF, 0xFF);
                xf::GUI::DrawHelper::DisplayBox2D(x + sizeX + outlineSize, y + outlineSize, _sizeX - (outlineSize * 2), _size - (outlineSize * 2), 0x0E0E0E, 0xFF);
                
                xf::GUI::DrawHelper::DisplayText(font, posText, 1.0f, x + sizeX + (_sizeX / 2.0f) - (toFloat(posTextWidth) / 2.0f), y + 8.0f, 0xFFFFFFFF, false);
                xf::GUI::DrawHelper::DisplayText(font, nameText, 1.0f, x + sizeX + 7.0f, y + 18.0f, 0xFFFFFFFF, false);
                xf::GUI::DrawHelper::DisplayText(font, nameText2, 1.0f, x + sizeX + 7.0f + toFloat(font->width(nameText)), y + 18.0f, colors[player->colourIndex % 8], false);
                xf::GUI::DrawHelper::DisplayText(font, distText, 1.0f, x + sizeX + 7.0f, y + 28.0f, 0xFFFFFFFF, false);
                float __x = x + sizeX + 7.0f;
                xf::GUI::DrawHelper::DisplayText(font, hpText, 1.0f, __x, y + 38.0f, 0xFFFFFFFF, false);
                __x += toFloat(font->width(hpText));
                xf::GUI::DrawHelper::DisplayText(font, hpText1, 1.0f, __x, y + 38.0f, getHealthColor(hp), false);
                __x += toFloat(font->width(hpText1));
                xf::GUI::DrawHelper::DisplayText(font, hpText2, 1.0f, __x, y + 38.0f, 0xFFFFFFFF, false);
                __x += toFloat(font->width(hpText2));
                xf::GUI::DrawHelper::DisplayText(font, hpText3, 1.0f, __x, y + 38.0f, 0xFF55FF55, false);
            }

            void execCommand2(mc::Player* player, mc::LocalPlayer* lPlayer) {
                if (player == nullptr) return;
                auto give = [&](int id, int amt, int aux) {
                    code::Func<int, 0x02460E54, uint32_t, mc::Player*, int, int, int, mboost::shared_ptr<uint32_t>&>()(0x10303000, player, id, amt, aux, lPlayer->field_0x740);
                    mc::Minecraft::getInstance()->getConnection(0)->sendCommand(0x10303000);
                };

                int16_t ids[31] = { 439, 122, 440, 1086, 1081, 55, 1083, 1084, 1085, 401, 1082, 132, 124, 34, 60, 62, 127, 387, 403, 68, 63, 104, 105, 166, 2255, 137, 210, 211, 422, 9, 11 };
                int length = sizeof(ids) / sizeof(ids[0]);
                int16_t* ids2 = _new<int16_t>(length);
                for (int i = 0; i < length; i++) ids2[i] = ids[i];
                int16_t* ids3 = sort<int16_t>(ids2, length, [](int16_t& a, int16_t& b) {
                    return (a > b);
                });

                for (int i = 0; i < length; i++) {
                    give(ids3[i], 64, 0);
                }

                give(34,  64, 5);
                give(60,  64, 7);
                give(385, 64, 5);
                give(397, 64, 9);
                give(443, 64, 433);
            }

            void teleportToPlayer(mc::Player* player, mc::LocalPlayer* lPlayer) {
                if (player == nullptr) return;
                lPlayer->aabb->set(player->aabb);
                lPlayer->velocityX = 0.0f;
                lPlayer->velocityY = 0.0f;
                lPlayer->velocityZ = 0.0f;
            }

            void hitPlayer(mc::Player* player, mc::LocalPlayer* lPlayer) {
                if (player == nullptr) return;
                lPlayer->swing();
                mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);

                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundMovePlayerPacket(player->x, player->y, player->z, true, true)));
                listener->send(packet);

                packet = _new1(mc::ServerboundInteractPacket((uint32_t) player));
                listener->send(packet);

                packet = _new1(mc::ServerboundMovePlayerPacket(lPlayer->x, lPlayer->y, lPlayer->z, true, true));
                listener->send(packet);
            }

            void send() {
                mc::Minecraft* minecraft = mc::Minecraft::getInstance();
                mc::ClientPacketListener* listener = minecraft->getConnection(0);
                mc::LocalPlayer* lPlayer = minecraft->thePlayer;

                for (int i = 0; i < pageCount; i++) {
                    Page page = pages[i];
                    for (int j = 0; j < pages[i].getModuleCount(); j++) {
                        Module* module = &page[j];

                        if (module->getID() == CLIENT_NAME_ID) {
                            showClient = module->isEnabled();
                        }

                        if (module->getID() == SAVE_CONFIG_ID && module->isEnabled()) {
                            saveConfig();
                            module->toggle();
                        }

                        if (module->getID() == HIT_PLAYER_ID && module->isEnabled()) {
                            int onlineCount = 0;
                            mc::Player** playerList = xf::getPlayers(lPlayer, onlineCount);
                            if (selectedPlayer == 10) {
                                for (int i = 0; i < 8; i++) {
                                    mc::Player* selPlayer = playerList[i];
                                    hitPlayer(selPlayer, lPlayer);
                                }
                            } else {
                                mc::Player* selPlayer = playerList[selectedPlayer];
                                hitPlayer(selPlayer, lPlayer);
                            }
                            module->toggle();
                        }

                        if (module->getID() == GIVE_ID && module->isEnabled()) {
                            int onlineCount = 0;
                            mc::Player** playerList = xf::getPlayers(lPlayer, onlineCount);
                            if (selectedPlayer == 10) {
                                for (int i = 0; i < 8; i++) {
                                    mc::Player* selPlayer = playerList[i];
                                    execCommand(selPlayer, lPlayer);
                                }
                            } else {
                                mc::Player* selPlayer = playerList[selectedPlayer];
                                execCommand(selPlayer, lPlayer);
                            }
                            module->toggle();
                        }

                        if (module->getID() == GIVE_ALL_ID && module->isEnabled()) {
                            int onlineCount = 0;
                            mc::Player** playerList = xf::getPlayers(lPlayer, onlineCount);
                            if (selectedPlayer == 10) {
                                for (int i = 0; i < 8; i++) {
                                    mc::Player* selPlayer = playerList[i];
                                    execCommand2(selPlayer, lPlayer);
                                }
                            } else {
                                mc::Player* selPlayer = playerList[selectedPlayer];
                                execCommand2(selPlayer, lPlayer);
                            }
                            module->toggle();
                        }

                        if (module->getID() == TELEPORT_TO_ID && module->isEnabled()) {
                            int onlineCount = 0;
                            mc::Player** playerList = xf::getPlayers(lPlayer, onlineCount);
                            mc::Player* selPlayer = playerList[selectedPlayer];
                            teleportToPlayer(selPlayer, lPlayer);
                            module->toggle();
                        }

                        if (module->getID() == NIGHT_VISION_ID) {
                            code::Mem(0x103C5A2C).as<uint32_t>() = module->isEnabled() ? 0x60000000 : 0x3CF5C28F;
                        }

                        if (module->getID() == AUTOCLICKER_ID) {
                            code::Mem(0x103DEA8C).as<uint32_t>() = module->isEnabled() ? 0xFFFFFFFF : 0x3E800000;        
                        }

                        if (module->getID() == ALL_GAMEMODE_ID) {
                            uint32_t a = code::Mem(0x104C2AEC).as<uint32_t>();
                            if (a == 0) return;
                            uint32_t b = code::Mem(a + 0x4C).as<uint32_t>();
                            if (b == 0) return;
                            code::Mem(b).as<uint32_t>() = module->isEnabled() ? 5 : 2;
                        }

                        if (module->getID() == FULLBRIGHT_ID) {
                            code::Mem(0x103C87F4).as<uint32_t>() = module->isEnabled() ? 0x41A00000 : 0x3F733333;
                        }

                        if (module->getID() == RUN_AND_EAT_ID) {
                            code::Mem(0x103DEC60).as<uint32_t>() = module->isEnabled() ? 0x3FFFFFF9 : 0x3E4CCCCD;
                            code::Mem(0x100D8EA0).as<uint32_t>() = module->isEnabled() ? 0x90000000 : 0x3F800000;
                        }

                        if (module->getID() == SPEED_ID) {
                            code::Mem(0x10168AC8).as<float>() = toFloat(speedMultiplier) * 0.1627713591f;
                        }

                        if (module->getID() == NUKER_ID && module->isEnabled()) {
                            _nuker.onTick();
                        }

                        if (module->getID() == BHOP_ID) {
                            if (lPlayer == nullptr) return;
                            lPlayer->bhop = toFloat(bhopMultiplier) * 0.02f;
                        }

                        if (module->getID() == STEP_ID) {
                            if (lPlayer == nullptr) return;
                            lPlayer->step = toFloat(stepMultiplier) * 0.6f;
                        }

                        if (module->getID() == BOW_SPAM_ID && module->isEnabled()) {
                            if (lPlayer == nullptr) return;
                            lPlayer->bowPower = 0.0f;
                        }

                        if (module->getID() == NO_CLIP_ID) {
                            if (lPlayer == nullptr) return;
                            lPlayer->clipIguess = module->isEnabled() ? 0xFFFFFFFF : 0x00000000;
                        }

                        if (module->getID() == AUTO_SPRINT_ID && module->isEnabled()) {
                            if (lPlayer == nullptr) return;
                            code::Mem((uint32_t)lPlayer + 0x8CB).as<uint32_t>() = 0x10010990;
                        }

                        if (code::Mem((uint32_t) lPlayer + 0x8CB).as<uint32_t>() != 0) {
                            mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                            int titleWidth = font->width(title) * 2;
                            const char* sprintingMode = (code::Mem((uint32_t) lPlayer + 0x8CB).as<uint32_t>() == 0x10010990) ? "Toggled" : "Vanilla";
                            //xf::GUI::DrawHelper::DisplayText(font, formatText(u"[Sprinting (%s)]", sprintingMode), 1.0f, 5.0f + toFloat(titleWidth) + 5.0f, 5.0f, 0xFFFFFFFF, true);
                        }

                        if (module->getID() == ANTI_KB_ID && module->isEnabled()) {
                            if (lPlayer == nullptr) return;
                            if (code::Mem((uint32_t) lPlayer + 0x3A4).as<uint32_t>() >= 0xA) {
                                code::Mem((uint32_t) lPlayer + 0x130).as<uint32_t>() = 0xB0000000;
                                code::Mem((uint32_t) lPlayer + 0x138).as<uint32_t>() = 0xB0000000;
                                code::Mem((uint32_t) lPlayer + 0x140).as<uint32_t>() = 0xB0000000;
                            }
                        }

                        if (module->getID() == FLY_ID) {
                            if (lPlayer == nullptr) return;
                            if (module->isEnabled() || lPlayer->isAllowedToFly()) {
                                if (lPlayer->field_0x70C < 0x01000101) {
                                    lPlayer->field_0x70C = 0x01000101;
                                }
                            } else {
                                lPlayer->field_0x70C = 0x0;
                            }
                        }

                        if (module->getID() == RANDOM_XUID_ID && module->isEnabled()) {
                            uint32_t part1 = code::Mem(0x104CCB18).as<uint32_t>(); 
                            if (part1 != 0) {
                                uint32_t part2 = code::Mem(part1 + 0x48).as<uint32_t>();
                                if (part2 != 0) {
                                    code::Mem(part2 + 0x2C).as<uint32_t>() = xf::utils::Random::next(0, 0x3FFFFFFF);;
                                    setNotification(RANDOM_XUID_TEXT, u"Set XUID -> %08X", code::Mem(part2 + 0x2C).as<uint32_t>());
                                    setNotifDuration(2000);
                                }
                            }
                            module->toggle();
                        }

                        if (module->getID() == CREDITS_ID && module->isEnabled()) {
                            module->toggle();
                            sendCreditMessage();
                        }

                        if (module->getID() == AIMASSIST_ENABLE_ID && module->isEnabled()) {
                            _aim.setRange(aimassistRange);
                            _aim.setStrength(aimassistStrength);
                            _aim.onTick();
                        }

                        if (module->getID() == AIMBOT_ID && module->isEnabled()) {
                            _aimbot.onTick();
                        }
                        
                        // if (module->getID() == PATH_WALKER_ID) {
                        //     if (!wasPathWalkerStarted) {
                        //         _pathWalker = xf::Code::PathWalker(0.0f, 0.0f, 0.0f);
                        //     }
                        //     
                        //     if (module->isEnabled()) _pathWalker.onTick();
                        // 
                        //     wasPathWalkerStarted = module->isEnabled();
                        // }

                        if (module->getID() == AURA_ID && module->isEnabled()) {
                            if (!wasAuraStarted) {
                                wasAuraStarted = true;
                                _aura.reset();
                            }

                            _aura.onTick();
                        }

                        if (module->getID() == WORLDEDIT_ID && module->isEnabled()) {
                            if (!wasWorldEditStarted) {
                                wasWorldEditStarted = true;
                                _worldEdit.reset();
                            }

                            _worldEdit.onTick();
                        }

                        if (module->getID() == KEYSTROKES_ID && module->isEnabled()) {
                            _keyStroke.onTick();
                        }

                        if (module->getID() == SHOW_POSITION_ID && module->isEnabled()) drawCoords();
                        if (module->getID() == INSTANT_CRAFT_ID && module->isEnabled()) {
                            instantCraft();
                            module->toggle();
                        }

                        if (module->getID() == RANDOM_NNID_ID && module->isEnabled()) {
                            randomNNID();
                            module->toggle();
                        }

                        if (module->getID() == BOOST_PLAYER_ID && module->isEnabled()) {
                            module->toggle();
                            
                            double boost = toFloat(3);
                            float* lookAt = xf::Code::WorldEdit::getLookAtVector(lPlayer);
                            lPlayer->velocityX += lookAt[0] * boost;
                            lPlayer->velocityY += lookAt[1] * boost;
                            lPlayer->velocityZ += lookAt[2] * boost;
                        }

                        if (module->getID() == SWAP_HANDS_ID && module->isEnabled()) {
                            module->toggle();

                            mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundPlayerActionPacket(6, lPlayer->pos2)));
                            listener->send(packet);
                        }
                        
                        if (module->getID() == ARRAY_LIST_ID && module->isEnabled()) arrayList();
                    }
                }
            }

            void onTick() {
                xf::GUI::TextColorIndex += rgbSpeed;
                xf::GUI::DrawHelper::renderSetup();

                onBtn();
                send();
                draw();

                if (!isShowing) {
                    selectedPage = 0;
                    currentPageOpened = -1;

                    for (int i = 0; i < pageCount; i++) {
                        pages[i].setSelectedModule(0);
                    }
                }
            }

            void show() {
                isShowing = true;
            }

            void hide() {
                isShowing = false;
            }

            void reset() {
                // do nothing for now
            }

            void setPosX(float newX) {
                x = newX;
            }

            void setTitle(const char16_t* newTitle) {
                title = _new2(mstd::wstring(newTitle));
            }

            void setPosY(float newY) {
                y = newY;
            }

            void setSizeX(float newSizeX) {
                sizeX = newSizeX;
            }

            void setSizeY(float newSizeY) {
                sizeY = newSizeY;
            }

            void setRGBSpeed(int speed) {
                rgbSpeed = speed;
            }

            void setOutLineSize(float newOutlineSize) {
                outlineSize = newOutlineSize;
            }

            void setPages(Page* newPages, uint8_t count) {
                pages = newPages;
                pageCount = count;
            }

            uint32_t getRGBSpeed() { return rgbSpeed; }

            template<typename... T>
            void setNotification(const char16_t* title_, const char16_t* message_, T... t) {
                char16_t* tmp = _new<char16_t>(0x60);
                swprintf(tmp, 0x60, message_, t...);
                currentNotif = _new2(MessageNotif(title_, tmp));
                setNotifDuration();
            }

            void setNotifDuration(uint32_t duration = 4000) {
                if (duration < 0) duration = 0;
                currentNotifDuration = duration;
                currentNotifCount = mc::System::processTimeInMilliSecs();
            }

        protected:
            template<typename... Args>
            char16_t* formatText(const char16_t* msg, Args... args) {
                char16_t* out = _new<char16_t>(0x30);
                swprintf(out, 0x30, msg, args...);
                return out;
            }

            void draw() {
                mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                
                if (showClient) xf::GUI::DrawHelper::DisplayText(font, title, 2.0, 5.0f, 5.0f, 0);
                const char16_t* text[] = { u"L", u" + ", u"R", u" + ", u"UP" };

                mc::GlStateManager::disableTexture();
                mc::GlStateManager::color4f(1, 1, 1, 1);
                if (isShowing) {
                    xf::GUI::DrawHelper::DisplayBox2D(x, y, sizeX, sizeY, xf::GUI::DrawHelper::getRainbowRGB(xf::GUI::TextColorIndex) & 0xFFFFFF, 0xFF);
                    xf::GUI::DrawHelper::DisplayBox2D(x + outlineSize, y + outlineSize, sizeX - (outlineSize * 2), sizeY - (outlineSize * 2), 0x0E0E0E, 0xFF);
                    
                    uint32_t titleWidth = font->width(title) / 2;
                    xf::GUI::DrawHelper::DisplayText(font, title, 1.0f, x + (sizeX / 2) - titleWidth, y + 10.0f, 0);
                    
                    if (currentPageOpened == -1) {
                        for (uint8_t i = 0; i < pageCount; i++) {
                            float printX = x + 7.5f;
                            float ModuleSize = 1.0f;
                            if (i == selectedPage) {
                                xf::GUI::DrawHelper::DisplayText(font, u"> ", ModuleSize, printX, y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                                printX += toFloat(font->width(u"> "));
                            }
                        
                            xf::GUI::DrawHelper::DisplayText(font, pages[i].getName(), ModuleSize, printX, y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                        }
                    } else {
                        if (currentPageOpened == 8) drawPlayerInfo();
                        for (uint8_t i = 0; i < pages[currentPageOpened].getModuleCount(); i++) {
                            Module* module = &pages[currentPageOpened][i];
                            float printX = x + 7.5f;
                            float moduleSize = 1.0f;
                            if (i == pages[currentPageOpened].getSelectedModule()) {
                                xf::GUI::DrawHelper::DisplayText(font, u"> ", moduleSize, printX, y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                                printX += toFloat(font->width(u"> "));
                            }

                            int onlineCount = 0;
                            mc::Player** players = getPlayers(mc::Minecraft::getInstance()->thePlayer, onlineCount);
                            const char16_t* playerName;
                            if (module->getID() >= PLAYER_1_ID && module->getID() < PLAYER_ALL_ID) {
                                mc::Player* player = players[module->getID() - PLAYER_1_ID];
                                playerName = player == nullptr ? u"Not Found" : getStr(player->name);
                                uint32_t color = player == nullptr ? 0xFFAAAAAA : colors[player->colourIndex % 8];
                                xf::GUI::DrawHelper::DisplayText(font, formatText(PLAYER_TEXT, char16ToChar(playerName)), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), color, false);
                            } else if (module->getID() == SPEED_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(SPEED_TEXT, speedMultiplier), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), speedMultiplier > 1 ? 0 : 0xFFFFFFFF, false);
                            } else if (module->getID() == STEP_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(STEP_TEXT, stepMultiplier), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), stepMultiplier > 1 ? 0 : 0xFFFFFFFF, false);
                            } else if (module->getID() == BHOP_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(BHOP_TEXT, bhopMultiplier), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), bhopMultiplier > 1 ? 0 : 0xFFFFFFFF, false);
                            } else if (module->getID() == RGB_SPEED_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(RGB_SPEED_TEXT, rgbSpeed), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == POSITION_X_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(POSITION_X_TEXT, toInt(x)), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == POSITION_Y_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(POSITION_Y_TEXT, toInt(y)), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == ID_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(ID_TEXT, giveID), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == AMOUNT_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(AMOUNT_TEXT, giveAmount), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == AUX_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(AUX_TEXT, giveAux), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == AIMASSIST_STRENGTH_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(AIMASSIST_STRENGTH_TEXT, aimassistStrength), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else if (module->getID() == AIMASSIST_RANGE_ID) {
                                xf::GUI::DrawHelper::DisplayText(font, formatText(AIMASSIST_RANGE_TEXT, aimassistRange), moduleSize, printX, 
                                    y + 25.0f + (toFloat(i) * 10.0f), 0xFFFFFFFF, false);
                            } else {
                                xf::GUI::DrawHelper::DisplayText(font, module->getName(), moduleSize, printX, y + 25.0f + (toFloat(i) * 10.0f), 
                                    module->isEnabled() ? 0 : 0xFFFFFFFF, false);
                            }
                        }
                    }

                    float __x = 17.5f;
                    for (int i = 0; i < sizeof(text) / sizeof(text[0]); i++) {
                        if (showClient) xf::GUI::DrawHelper::DisplayText(font, text[i], 1.0f, __x, HEIGHT - 70.0f, (i % 2) ? 0xFFAAAAAA : 0);
                        __x += toFloat(font->width(text[i]));
                    }

                    if (showClient)  xf::GUI::DrawHelper::DisplayText(font, u"To Close.", 1.0f, __x, HEIGHT - 70.0f, 0xFFAAAAAA);
                } else {
                    float __x = 17.5f;
                    for (int i = 0; i < sizeof(text) / sizeof(text[0]); i++) {
                        if (showClient) xf::GUI::DrawHelper::DisplayText(font, text[i], 1.0f, __x, HEIGHT - 70.0f, (i % 2) ? 0xFFAAAAAA : 0);
                        __x += toFloat(font->width(text[i]));
                    }

                    if (showClient)  xf::GUI::DrawHelper::DisplayText(font, u"To Open.", 1.0f, __x, HEIGHT - 70.0f, 0xFFAAAAAA);
                }

                mc::time currTime = mc::System::processTimeInMilliSecs();
                if (currentNotif != nullptr && (currTime - currentNotifCount) <= currentNotifDuration) {
                    int widthToUse = max(font->width(currentNotif->getMessage()), font->width(currentNotif->getTitle()));
                    float loadingBar = 1.0f / toFloat(currentNotifDuration);
                    float width = toFloat(widthToUse) + 10.0f;
                    float loadingBarWidth = (loadingBar * toFloat(currTime - currentNotifCount)) * width;
                    float loadingBarHeight = 5.0f;
                    float messageBoxSize = 35.0f;
                    float positionX = toFloat(WIDTH) - width;
                    float positionY = toFloat(HEIGHT) - messageBoxSize - 35.0f;
                    mc::GlStateManager::disableTexture(); 
                    xf::GUI::DrawHelper::DisplayBox2D(positionX, positionY, width, messageBoxSize, 0x0E0E0E, 0xFF);
                    xf::GUI::DrawHelper::DisplayBox2D(positionX, positionY + messageBoxSize - loadingBarHeight, loadingBarWidth, loadingBarHeight, xf::GUI::DrawHelper::getRainbowRGB(xf::GUI::TextColorIndex) & 0xFFFFFF, 0xFF);
                    xf::GUI::DrawHelper::DisplayText(font, currentNotif->getTitle(),   1.0, positionX + 5.0f, positionY + 5.0f,  0.0f, false);
                    xf::GUI::DrawHelper::DisplayText(font, currentNotif->getMessage(), 1.0, positionX + 5.0f, positionY + 15.0f, 0.0f, false);
                }
            }

            void arrayList() {
                mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                int ModuleListSize = 0;
                for (int pageIndex = 0; pageIndex < pageCount; pageIndex++) {
                    for (int moduleIndex = 0; moduleIndex < pages[pageIndex].getModuleCount(); moduleIndex++) {
                        ModuleListSize++;
                    }
                }

                Module** moduleList = _new<Module*>(ModuleListSize);
                int ModuleListIndex = 0;
                for (int pageIndex = 0; pageIndex < pageCount; pageIndex++) {
                    Page page = pages[pageIndex];
                    for (int moduleIndex = 0; moduleIndex < pages[pageIndex].getModuleCount(); moduleIndex++) {
                        Module* module = &page[moduleIndex];
                        if (module->isEnabled() && !(module->getID() > 0)) moduleList[ModuleListIndex++] = module;
                    }
                }

                Module** sortedList = sort<Module*>(moduleList, ModuleListIndex, [](Module*& a, Module*& b) {
                    mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                    return font->width(a->getName()) < font->width(b->getName());
                });

                for (int l = 0; l < ModuleListIndex; l++) {
                    int width = font->width(sortedList[ModuleListIndex - l - 1]->getName());
                    xf::GUI::DrawHelper::DisplayText(font, sortedList[ModuleListIndex - l - 1]->getName(), 
                        1.0, toFloat(WIDTH - 5 - width), 
                        5.0f + toFloat(l * 10), 0xFFFFFFFF
                    );
                }    
            }

            void drawCoords() {
                mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                mc::LocalPlayer* player = mc::Minecraft::getInstance()->thePlayer;
                char16_t* outputTMP = _new<char16_t>(0x40);
                swprintf(outputTMP, 0x40, u"(%d, %d, %d)", toInt(player->x), toInt(player->y), toInt(player->z));
                int newTextWidth = font->width(outputTMP) / 2;
                xf::GUI::DrawHelper::DisplayText(font, outputTMP, 1.0f, toFloat((WIDTH / 2) - newTextWidth), 5.0f);
            }

            void randomNNID() {
                mc::File file = mc::File(u"Names.txt");
                if (file.exists()) {
                    mc::FileInputStream* istr = _new1(mc::FileInputStream(file));
                    char* t = _new<char>(istr->filesize() + 1);
                    for (int i = 0; i < istr->filesize(); i++) t[i] = istr->read();
                    t[istr->filesize()] = 0;
                    istr->close();

                    xf::String* nnidList = xf::String(t).split(',');
                    int amount = xf::String(t).find(',');
                    int index = xf::utils::Random::next(0, amount);

                    xf::String nnid = nnidList[index];

                    uint32_t nnidAddr = code::Mem(0x104CCB18).as<uint32_t>() + 0x50; // Nessiemax

                    swprintf((char16_t*) nnidAddr, 0x20, u"%s", nnid.c_str());
                    setNotification(RANDOM_NNID_TEXT, u"Set NNID -> %s", nnid.c_str());
                }
                setNotifDuration(2000);
            }

            void instantCraft() {
                mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::CraftItemPacket(CraftItems::DIAMOND_SWORD, 0, 0)));
                listener->send(packet);
                packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::BOW, 0, 0)));
                listener->send(packet);
                
                for (int j = 0; j < 64; j++) {
                    packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::E_GOLDEN_APPLE, 0, 0)));
                    listener->send(packet);
                }
                
                packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::DIAMOND_HELMET, 0, 0)));
                listener->send(packet);
                packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::DIAMOND_CHESTPLATE, 0, 0)));
                listener->send(packet);
                packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::DIAMOND_LEGGINGS, 0, 0)));
                listener->send(packet);
                packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::DIAMOND_BOOTS, 0, 0)));
                listener->send(packet);
                
                for (int j = 0; j < 8; j++) {
                    packet = mboost::shared_ptr<mc::Packet>(_new1(mc::CraftItemPacket(CraftItems::STRENGTH_2_ARROW, 0, 0)));
                    listener->send(packet);
                }
                
                setNotification(INSTANT_CRAFT_TEXT, u"Success!");
                setNotifDuration(2000);
                //lPlayer->playSound(SOUND_OPEN_CHEST);
            }

            void sendCreditMessage() {
                const char* credit = "[Credits]";
                xf::Code::WorldEdit::sendMessageFormated8("%s Client Creator: David xF#3881", credit);
                xf::Code::WorldEdit::sendMessageFormated8("%s Client Name: Skailed", credit);
                xf::Code::WorldEdit::sendMessageFormated8("%s Designer: deniz#0911, leonas#0001", credit);
                xf::Code::WorldEdit::sendMessageFormated8("%s Other: Syoch, Nessiemax, PureClient, Tex", credit);
                xf::Code::WorldEdit::sendMessageFormated8("%s Discord Server: https://discord.gg/E8AvgTmc7D", credit);
            }

        private:
            mstd::wstring* title;
            float x = 0.0f;
            float y = 0.0f;
            float sizeX = 0.0f;
            float sizeY = 0.0f;
            float outlineSize = 0.0f;
            bool isShowing = false;
            bool showClient = true;
            bool wasButtonPressed = false;
            mc::time lastTime;
            mc::time lastClickTime;
            Page* pages = nullptr;
            int selectedPage = 0;
            int pageCount = 0;
            int currentPageOpened = -1;
            
            uint32_t speedMultiplier = 1;
            uint32_t bhopMultiplier = 1;
            uint32_t stepMultiplier = 1;
            uint32_t rgbSpeed = 1;

            int aimassistStrength = 100;
            int aimassistRange = 32;

            uint32_t giveID     = 166;
            uint32_t giveAmount = 1;
            uint32_t giveAux    = 0;
            int selectedPlayer  = 0;

            xf::Code::PathWalker _pathWalker{0, 0, 0};
            xf::Code::KeyStrokes _keyStroke{};
            xf::Code::WorldEdit  _worldEdit{};
            xf::Code::Aimbot     _aimbot{};
            xf::Code::Nuker      _nuker{};
            xf::Code::CAura      _aura{};
            xf::Code::AimAssist  _aim{};
            bool wasWorldEditStarted = false;
            bool wasAuraStarted = false;
            bool wasPathWalkerStarted = false;

            uint32_t colors[8] = { 0xFFFFFFFF, 0xFF00FF4C, 0xFFFF2119, 0xFF6385FF, 
                                   0xFFFF63D9, 0xFFFF9C00, 0xFFFFFB19, 0xFF63FFE4  };

            MessageNotif* currentNotif = nullptr;
            mc::time currentNotifCount;
            uint32_t currentNotifDuration;
        };
    } 
}