#pragma once

#include <minecraft/mc/GlStateManager.h>
#include <minecraft/mc/BufferBuilder.h>
#include <minecraft/mc/miscellanious.h>
#include <minecraft/mc/Tesselator.h>
#include <minecraft/mc/Minecraft.h>
#include <minecraft/mc/Textures.h>

#include <minecraft/mc/Font.h>

namespace xf {
    namespace GUI {
        class Vector3f {
        public:
            Vector3f(float _x, float _y, float _z) {
                x = _x;
                y = _y;
                z = _z;
            }

            Vector3f() = default;

            Vector3f operator*(float mult) {
                return Vector3f(x * mult, y * mult, z * mult);
            }

            Vector3f operator*(Vector3f mult) {
                return Vector3f(x * mult.x, y * mult.y, z * mult.z);
            }

            Vector3f operator+(Vector3f other) {
                return Vector3f(x + other.x, y + other.y, z + other.z);
            }

            float x;
            float y;
            float z;
        };

        uint32_t TextColorIndex = 0;
        uint32_t frameCount = 0;
        
        class DrawHelper {
        public:
            static void renderSetup(bool disableTex = true) {
                mc::GlStateManager::matrixMode(1);
                mc::GlStateManager::loadIdentity();
                mc::GlStateManager::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 3000.0f);
                mc::GlStateManager::matrixMode(0);
                mc::GlStateManager::loadIdentity();
                mc::GlStateManager::translatef(0.0f, 0.0f, 0.0f);
                mc::GlStateManager::scalef(1.0f, 1.0f, 1.0f);
                mc::GlStateManager::disableCull();
                mc::GlStateManager::disableLighting();
                mc::GlStateManager::disableDepthTest();
                if (disableTex) mc::GlStateManager::disableTexture();
                else            mc::GlStateManager::enableTexture();
                mc::GlStateManager::enableBlend();
                mc::GlStateManager::blendFunc(4, 5);
                mc::GlStateManager::color4f(1, 1, 1, 1);
                mc::GlStateManager::disableFog();
            }

            static void DisplayText(mc::Font* font, mstd::wstring text, float size, float x, float y, uint32_t color = 0xFFFFFFFF, bool normal = true) {
                mc::GlStateManager::pushMatrix();
                mc::GlStateManager::translatef(x, y, 0);
                mc::GlStateManager::scalef(size, size, 0);
                if (color == 0) color = getRainbowRGB(TextColorIndex);
                if (normal)  font->drawShadow(text, 0, 0, color);
                else         font->draw      (text, 0, 0, color);
                mc::GlStateManager::popMatrix();
            }

            static void DisplayText(mc::Font* font, mstd::wstring* text, float size, float x, float y, uint32_t color = 0xFFFFFFFF, bool normal = true) {
                mc::GlStateManager::pushMatrix();
                mc::GlStateManager::translatef(x, y, 0);
                mc::GlStateManager::scalef(size, size, 0);
                if (color == 0) color = getRainbowRGB(TextColorIndex);
                if (normal)  font->drawShadow(text, 0, 0, color);
                else         font->draw      (text, 0, 0, color);
                mc::GlStateManager::popMatrix();
            }

            static void DisplayRoundedBox2D(float x, float y, float sX, float sY, int rgb, int a, float cornerRadius = 10.0f) {
                mc::GlStateManager::pushMatrix();
                mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
            
                float radius = cornerRadius;
                if (sX < 2 * radius) radius = sX / 2;
                if (sY < 2 * radius) radius = sY / 2;
            
                builder->begin(2);
                builder->color(rgb, a);
                builder->vertex(x + radius, y, 0.0f);
                builder->vertex(x + sX - radius, y, 0.0f);
                builder->vertex(x + sX, y + radius, 0.0f);
                builder->vertex(x + sX, y + sY - radius, 0.0f);
                builder->vertex(x + sX - radius, y + sY, 0.0f);
                builder->vertex(x + radius, y + sY, 0.0f);
                builder->vertex(x, y + sY - radius, 0.0f);
                builder->vertex(x, y + radius, 0.0f);
            
                int numSegments = 10; 
                for (int i = 0; i <= numSegments / 4; i++) {
                    float angle = i * 2 * M_PI / numSegments;
                    float cx = x + sX - radius + radius * cos(angle); // 4
                    float cy = y + radius + radius * sin(angle);   // 1
                    builder->vertex(cx, cy, 0.0f);
                }
            
                for (int i = numSegments / 4; i <= numSegments / 2; i++) {
                    float angle = i * 2 * M_PI / numSegments;
                    float cx = x + sX - radius + radius * cos(angle); // 4
                    float cy = y + sY - radius + radius * sin(angle); // 3
                    builder->vertex(cx, cy, 0.0f);
                }
            
                for (int i = numSegments / 2; i <= 3 * numSegments / 4; i++) {
                    float angle = i * 2 * M_PI / numSegments;
                    float cx = x + radius + radius * cos(angle);   // 2
                    float cy = y + sY - radius + radius * sin(angle); // 3
                    builder->vertex(cx, cy, 0.0f);
                }
            
                for (int i = 3 * numSegments / 4; i <= numSegments; i++) {
                    float angle = i * 2 * M_PI / numSegments;
                    float cx = x + radius + radius * cos(angle);   // 2
                    float cy = y + radius + radius * sin(angle);   // 1
                    builder->vertex(cx, cy, 0.0f);
                }
            
                builder->end();
                mc::GlStateManager::popMatrix();
            }

            static void DisplayBox2D(float x, float y, float sX, float sY, int rgb, int a) {
                mc::GlStateManager::pushMatrix();
                mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
                builder->begin();
                builder->color(rgb, a);
                builder->vertex(x,      y,      0.0f);
                builder->vertex(x + sX, y,      0.0f);
                builder->vertex(x,      y + sY, 0.0f);
                builder->end();

                builder->begin();
                builder->color(rgb, a);
                builder->vertex(x + sX, y + sY, 0.0f);
                builder->vertex(x + sX, y,      0.0f);
                builder->vertex(x,      y + sY, 0.0f);
                builder->end();
                mc::GlStateManager::popMatrix();
            }

            static void DisplayBox2D(float x, float y, float sX, float sY, int imageID) {
                mc::Minecraft::getInstance()->textures->bind(imageID);
                mc::GlStateManager::pushMatrix();
                mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
                builder->begin();
                builder->vertexUV(x,      y,      0.0f, 0.0f, 0.0f);
                builder->vertexUV(x + sX, y,      0.0f, 1.0f, 0.0f);
                builder->vertexUV(x,      y + sY, 0.0f, 0.0f, 1.0f);
                builder->end();

                builder->begin();
                builder->vertexUV(x + sX, y + sY, 0.0f, 1.0f, 1.0f);
                builder->vertexUV(x + sX, y,      0.0f, 1.0f, 0.0f);
                builder->vertexUV(x,      y + sY, 0.0f, 0.0f, 1.0f);
                builder->end();
                mc::GlStateManager::popMatrix();
            }

            static void DisplayBox3D(float s, int imageID) {
                mc::Minecraft::getInstance()->textures->bind(imageID);
                mc::GlStateManager::pushMatrix();
                mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();

                float n = s;
                s = s*2;

                //float32 Su = texture.getSide().r;    
            	//float32 Sv = texture.getSide().g;
            	//float32 Sm = texture.getSide().b;
            	//float32 Sn = texture.getSide().a;
         
                float width = 1.0f / 3.0f;
                float height = 1.0f / 2.0f;

                auto getUV = [&](int x, int y) {
                    float* f = _new<float>(4);

                    f[0] = width * x;
                    f[1] = f[0] + width;
                    f[3] = height * y;
                    f[2] = f[3] + height;
                    return f;
                };

                auto getUV2 = [&](int x) {
                    if (x == 0) return getUV(0, 0);
                    if (x == 4) return getUV(0, 1);
                    if (x == 1) return getUV(1, 0);
                    if (x == 5) return getUV(1, 1);
                    if (x == 3) return getUV(2, 0);
                    if (x == 2) return getUV(2, 1);
                };

                float* f = getUV2(1);
                builder->begin(); // TOP
                builder->vertexUV(-n,      -n + s,  -n + s,  f[1], f[2]); // Y
			    builder->vertexUV(-n + s,  -n + s,  -n + s,  f[0], f[2]); // Y
			    builder->vertexUV(-n + s,  -n + s,  -n,      f[0], f[3]); // Y
			    builder->vertexUV(-n,      -n + s,  -n,      f[1], f[3]); // Y
                builder->end();

                f = getUV2(0);
                builder->begin(); // BELOW
                builder->vertexUV(-n,      -n,      -n + s,  f[1], f[3]); // Y
			    builder->vertexUV(-n + s,  -n,      -n + s,  f[0], f[3]); // Y
			    builder->vertexUV(-n + s,  -n,      -n,      f[0], f[2]); // Y
			    builder->vertexUV(-n,      -n,      -n,      f[1], f[2]); // Y
                builder->end();

                f = getUV2(2);
                builder->begin(); // SOUTH = THe OTHER THING
			    builder->vertexUV(-n,      -n + s,  -n + s,  f[0], f[3]); // X
			    builder->vertexUV(-n,      -n + s,  -n,      f[1], f[3]); // X
			    builder->vertexUV(-n,      -n,      -n,      f[1], f[2]); // X
                builder->vertexUV(-n,      -n,      -n + s,  f[0], f[2]); // X
                builder->end();

                f = getUV2(3);
                builder->begin(); // WEST = LEFT
                mc::GlStateManager::rotatef(0, toFloat(frameCount), 0);
			    builder->vertexUV(-n,      -n + s,  -n,      f[0], f[3]); // Z
			    builder->vertexUV(-n + s,  -n + s,  -n,      f[1], f[3]); // Z
			    builder->vertexUV(-n + s,  -n,      -n,      f[1], f[2]); // Z
                builder->vertexUV(-n,      -n,      -n,      f[0], f[2]); // Z
                builder->end();

                f = getUV2(4);
                builder->begin(); // NORTH = MIDDLE
			    builder->vertexUV(-n + s,  -n + s,  -n,      f[0], f[3]); // X
			    builder->vertexUV(-n + s,  -n + s,  -n + s,  f[1], f[3]); // X
                builder->vertexUV(-n + s,  -n,      -n + s,  f[1], f[2]); // X
			    builder->vertexUV(-n + s,  -n,      -n,      f[0], f[2]); // X
                builder->end();

                f = getUV2(5);
                builder->begin(); // EAST = RIGHT
                builder->vertexUV(-n + s,  -n + s,  -n + s,  f[0], f[3]); // Z
			    builder->vertexUV(-n,      -n + s,  -n + s,  f[1], f[3]); // Z
			    builder->vertexUV(-n,      -n,      -n + s,  f[1], f[2]); // Z
			    builder->vertexUV(-n + s,  -n,      -n + s,  f[0], f[2]); // Z
                builder->end();

                mc::GlStateManager::popMatrix();
            }

            static uint32_t Color(uint32_t r, uint32_t g, uint32_t b) {
                return 0xFF000000 | (r & 0xFF) << 24 | (g & 0xFF) << 16 | (b & 0xFF) << 8;
            }

            static uint32_t HSVToColor(float H, float S, float V) {
                return code::Func<uint32_t, 0x02624178, float, float, float>()(H, S, V);
            }

            static uint32_t getRainbowRGB(int _index = 0) {
                float jump = 360.0f / (2000.0f);
                for (int i = 0; i < 2000; i++) {
                    if (i == (_index % 2000)) {
                        uint32_t hsv = HSVToColor((jump * i) / 360.0f, 1.0f, 1.0f);
                        return 0xFF000000 | hsv;
                    }
                }
                
                return Color(255, 255, 255);
            }
        };
    } // GUI
} // xf