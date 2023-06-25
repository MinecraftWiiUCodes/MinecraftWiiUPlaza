#pragma once

#include <code/code.h>
#include <inttypes.h>

#define X_MAX 1000
#define X_STEP 20
#define Y_MAX 500
#define Y_STEP 20

#include <float.h>
#include "Keystrokes.h"

namespace xf {
    namespace Code {
        template<typename _Tp, uint32_t _Nm>
        struct array {
            typedef _Tp                value_type;
            typedef _Tp*               pointer;
            typedef const _Tp*         const_pointer;
            typedef value_type&        reference;
            typedef const value_type&  const_reference;
            typedef value_type*        iterator;
            typedef const value_type*  const_iterator;

            value_type _M_instance[_Nm ? _Nm : 1];

            reference operator[](uint32_t __n) { 
                return _M_instance[__n]; 
            }

            const_reference operator[](uint32_t __n) const { 
                return _M_instance[__n]; 
            }
        };

        class PathWalker {
        public:
            PathWalker(float _x, float _y, float _z) : nodePath{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}} {
                x = _x;
                y = _y;
                z = _z;
            }

            static float degreeToRadian(float deg) {
                return (deg * M_PI / 180.0f);
            }

            static float radianToDegree(float rad) {
                return (rad * 180.0f / M_PI);
            }

            static float* getLookAtVector(float yaw, float pitch) {
                float* arr = _new<float>(3);
                arr[0] = sinf(degreeToRadian(yaw   - 180.0f));
                arr[1] = sinf(degreeToRadian(pitch - 180.0f));
                arr[2] = cosf(degreeToRadian(yaw));

                return arr;
            }

            static float* getLookAtVector(mc::LocalPlayer* lPlayer) {
                return getLookAtVector(lPlayer->yaw, lPlayer->pitch);
            }

            void setup() {
                if (lastTime.p2 == 0) {
                    mc::LocalPlayer* lP = mc::Minecraft::getInstance()->thePlayer;
                    nodePath[5] = xf::GUI::Vector3f( -350,     0,  -350);
                    nodePath[4] = xf::GUI::Vector3f(  350,     0,  -350);
                    nodePath[3] = xf::GUI::Vector3f( -350,     0,   350);
                    nodePath[2] = xf::GUI::Vector3f(  350,     0,   350);
                    nodePath[1] = xf::GUI::Vector3f(    0,     0,     0);
                    nodePath[0] = xf::GUI::Vector3f(lP->x, lP->y, lP->z);
                }
            }

            void onTick() {
                setup();
                if (nodePathIndex == -1) return;
                mc::time currentTime = mc::System::processTimeInMilliSecs();
                deltaTime = toFloat(currentTime - lastTime) / 1000.0f;
                lastTime = currentTime;
                if (deltaTime > 2000.0f || 0.0f > deltaTime) deltaTime = 0;

                char16_t* temp = _new<char16_t>(0x40);
                
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                float walkSpeed = 8.0f * code::Mem(0x10168AC8).as<float>();
                float* lookAt = getLookAtVector(lPlayer);
                xf::GUI::Vector3f vec = xf::GUI::Vector3f(lookAt[0], lookAt[1], lookAt[2]);
                vec = vec * walkSpeed;
                vec = vec * deltaTime;
                vec = vec * xf::GUI::Vector3f(1.0f, 0.0f, 1.0f);

                mc::AABB newAABB = mc::AABB(lPlayer->aabb->minX + vec.x, lPlayer->aabb->minY + vec.y, lPlayer->aabb->minZ + vec.z, 
                                            lPlayer->aabb->maxX + vec.x, lPlayer->aabb->maxY + vec.y, lPlayer->aabb->maxZ + vec.z);
                lPlayer->aabb->set(_new2(newAABB));

                double diffX = nodePath[nodePathIndex].x - lPlayer->x;
                double diffY = nodePath[nodePathIndex].y - lPlayer->y;
                double diffZ = nodePath[nodePathIndex].z - lPlayer->z;
                float yaw = (float) atan2(diffZ, diffX) * (180.0 / M_PI) - 90.0f;
                if (!isnan(yaw)) lPlayer->yaw = yaw;
                lPlayer->pitch = 0.0f;

                if (sqrt(pow(diffX, 2) + pow(diffZ, 2)) < 0.25f) {
                    nodePathIndex--;
                    if (nodePathIndex == -1) {
                        xf::Code::WorldEdit::sendMessageFormated8("[PathFinder] Reached Destination!");
                    }
                }

                swprintf(temp, 0x40, u"%s | %s %s %s | %d", toCStr(deltaTime, 2), toCStr(vec.x, 2), toCStr(vec.y, 2), toCStr(vec.z, 2), 0);
                xf::GUI::DrawHelper::DisplayText(mc::Minecraft::getInstance()->defaultFonts, temp, 1.0f, 5, HEIGHT - 100, 0xFFFFFFFF, true);
            }

        private:
            mc::time lastTime {0, 0};
            float deltaTime;

            xf::GUI::Vector3f nodePath[6];
            int nodePathIndex = 5;

            float x;
            float y;
            float z;
        };
    }
}