#pragma once

#include <Minecraft/mc/miscellanious.h>
#include <Minecraft/mc/System.h>

#include "DrawHelper.h"
#include "WEdit.h"

namespace xf {
    namespace Code {
        class KeyStrokes {
        public:
            KeyStrokes() {

            }

            template<typename... T>
            char* formatText(char* text, T... t) {
                char16_t* temp = _new<char16_t>(0x60);
                swprintf(temp, 0x60, charToChar16(text), t...);
                return char16ToChar(temp);
            }

            void draw() { 
                auto f = [&](int input) {
                    return toFloat(input);
                };

                auto c = [&](const char* input) {
                    return charToChar16(input);
                }; 

                uint32_t btn = getButton();
                bool isRGB = true;
                uint32_t clicked; 
                if (!isRGB) clicked = 0xFF0000;
                else        clicked = 0xFFFFFF & xf::GUI::DrawHelper::getRainbowRGB(xf::GUI::frameCount);
                uint32_t notClicked = 0xFFFFFF;

                auto getCPS = [&](int i) {
                    int length;
                    if (i == 0) length = sizeof(lmbCps) / sizeof(lmbCps[0]);
                    else        length = sizeof(rmbCps) / sizeof(rmbCps[0]);
                    
                    mc::time* listToUse = (i == 0) ? lmbCps : rmbCps;
                    mc::time currTime = mc::System::processTimeInMilliSecs();
                    for (int j = 0; j < length; j++) {
                        if ((currTime - listToUse[j]) > 1000) listToUse[j] = { 0, 0 };
                    }

                    int cpsCount = 0;
                    for (int j = 0; j < length; j++) {
                        if (listToUse[j].p1 != 0 && listToUse[j].p2 != 0) cpsCount++;
                    }

                    return cpsCount;
                };

                auto b = [&](int i) {
                    if (i == 0) return ((btn & Button::LEFT_STICK_UP)    ? clicked : notClicked);
                    if (i == 1) return ((btn & Button::LEFT_STICK_DOWN)  ? clicked : notClicked);
                    if (i == 2) return ((btn & Button::LEFT_STICK_LEFT)  ? clicked : notClicked);
                    if (i == 3) return ((btn & Button::LEFT_STICK_RIGHT) ? clicked : notClicked);
                };

                auto b2 = [&](int i) {
                    if (btn & Button::ZL) {
                        if (!wasClickedL) {
                            bool cont = true;
                            int length = sizeof(lmbCps) / sizeof(lmbCps[0]);
                            for (int j = 0; j < length; j++) {
                                if (cont && lmbCps[j].p1 == 0 && lmbCps[j].p2 == 0) {
                                    lmbCps[j] = mc::System::processTimeInMilliSecs();
                                    cont = false;
                                }
                            }

                            wasClickedL = true;
                        }
                    } else {
                        wasClickedL = false;
                    }

                    if (btn & Button::ZR) {
                        if (!wasClickedR) {
                            bool cont = true;
                            int length = sizeof(rmbCps) / sizeof(rmbCps[0]);
                            for (int j = 0; j < length; j++) {
                                if (cont && rmbCps[j].p1 == 0 && rmbCps[j].p2 == 0) {
                                    rmbCps[j] = mc::System::processTimeInMilliSecs();
                                    cont = false;
                                }
                            }

                            wasClickedR = true;
                        }
                    } else {
                        wasClickedR = false;
                    }

                    if (i == 0) return ((btn & Button::ZL) ? clicked : notClicked);
                    if (i == 1) return ((btn & Button::ZR) ? clicked : notClicked); 
                };

                float totalWidth = (boxSize * 3.0f) + (boxDistance * 2.0f);
                float mbWidth = (totalWidth / 2.0f) - (boxDistance / 2.0f);
                
                auto box = [&](float _posX, float _posY, int i, int j) {
                    mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                    const char* text[6] = { "W", "S", "A", "D", "LMB", "RMB" };
                    
                    mc::GlStateManager::disableTexture();
                    if (j > 3) xf::GUI::DrawHelper::DisplayBox2D(_posX, _posY, mbWidth, boxSize, b2(i), 150);
                    else       xf::GUI::DrawHelper::DisplayBox2D(_posX, _posY, boxSize, boxSize, b(i), 150);
                    
                    if (j > 3) {
                        xf::GUI::DrawHelper::DisplayText(font, c(text[j]), 1.0f, _posX + (mbWidth / 2.0f) - (f(font->width(c(text[j]))) / 2.0f), _posY + (boxSize / 2.0f) - 6.20f, 0xFFFFFFFF, true);
                        char* cpsText = formatText("CPS: %d", getCPS(j - 4));
                        xf::GUI::DrawHelper::DisplayText(font, c(cpsText), 0.75f, _posX + (mbWidth / 2.0f) - ((f(font->width(c(cpsText))) * 0.75f) / 2.0f), _posY + (boxSize / 2.0f) - 3.5f + 6.80f, 0xFFFFFFFF, true);
                    } else {
                        xf::GUI::DrawHelper::DisplayText(font, c(text[j]), 1.0f, _posX + (boxSize / 2.0f) - (f(font->width(c(text[j]))) / 2.0f), _posY + (boxSize / 2.0f) - 3.5f, 0xFFFFFFFF, true);
                    }
                };

                box(posX,                         posY,                         0, 0); // W
                box(posX,                         posY + boxDistance + boxSize, 1, 1); // S
                box(posX - boxDistance - boxSize, posY + boxDistance + boxSize, 2, 2); // A
                box(posX + boxDistance + boxSize, posY + boxDistance + boxSize, 3, 3); // D

                float lmbPosX = posX - boxDistance - boxSize;
                float lmbPosY = posY + boxDistance + boxSize + boxDistance + boxSize;
                box(lmbPosX,                         lmbPosY, 0, 4); // LMB
                box(lmbPosX + mbWidth + boxDistance, lmbPosY, 1, 5); // RMB
            }

            void onTick() {
                draw();
            }

        private:
            float posX = 50.0f;
            float posY = 50.0f;

            float boxSize = 20.0f;
            float boxDistance = 3;

            mc::time lmbCps[100];
            bool wasClickedL = false;
            mc::time rmbCps[100];
            bool wasClickedR = false;
        };
    }
}