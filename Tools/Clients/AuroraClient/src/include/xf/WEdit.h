#pragma once

#include <minecraft/mc/Minecraft.h>
#include <minecraft/mc/ServerboundSetCreativeModeSlotPacket.h>
#include <minecraft/mc/ServerboundPlayerActionPacket.h>
#include <minecraft/mc/ServerboundMovePlayerPacket.h>
#include <minecraft/mc/ServerboundUseItemOnPacket.h>

#include <minecraft/mc/miscellanious.h>
#include <minecraft/mc/ItemInstance.h>
#include <minecraft/mc/Item.h>

#include "PerlinNoise.h"
#include "GUI.h"

namespace xf {
    namespace Code {
        struct blockData {
        public:
            int16_t id;
            int16_t aux;
        };
        
        enum WorldEditTreeEnum {
            OAK, BIRCH, LAST
        };

        class WorldEdit {
        public:
            WorldEdit() {

            }

            void btnHandler() {
                uint32_t btn = getButton();
                mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
                mc::Item* item = getCurrentItem(lPlayer);
                Items itemID = (Items) getItemID(item);

                int* pos = getBlockHit(lPlayer);

                if (btn & Button::ZL) {
                    if (active) return;

                    switch (itemID) {
                    case Items::WOODEN_AXE:
                        if (pos[0] == noBlockHit) return;

                        sendMessageFormated8("%s First position set to (%d, %d, %d)", weText, pos[0], pos[1], pos[2]);
                        pos1 = mc::BlockPos(pos[0], pos[1], pos[2]);
                        break;
                    case Items::STONE_HOE:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        //mc::EndGatewayFeature(0).place(mc::Minecraft::getInstance()->thePlayer->lvl, 0, {pos[0], pos[1], pos[2]});
                        saveConfig();
                        break;
                    case Items::STONE_SHOVEL:
                        if (pos[0] == noBlockHit) return;

                        breakSphere(pos, StoneShovelRadius);
                        we_switchItem(Items::STONE_SHOVEL);
                        break;
                    case Items::STONE_PICKAXE:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        fill();
                        we_switchItem(Items::STONE_PICKAXE);
                        break; 
                    case Items::IRON_HOE:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        copy(); 
                        break; 
                    }

                    switch (itemID) {
                    case DIAMOND_PICKAXE:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        noiseTest();
                        we_switchItem(DIAMOND_PICKAXE);
                        break;
                    }

                    switch (itemID) {
                    case GOLDEN_SHOVEL:
                        if (pos[0] == noBlockHit) {
                            maskedBlock = 0;
                            sendMessageFormated8("%s Masked block to 0:0", weText);
                        } else {
                            maskedBlock = (lPlayer->lvl->getBlockId(pos[0], pos[1], pos[2]) << 16) | lPlayer->lvl->getBlockData(pos[0], pos[1], pos[2]);
                            sendMessageFormated8("%s Masked block to %d:%d", weText, maskedBlock >> 16, maskedBlock & 0xFFFF);
                        }
                
                        break;
                    case Items::IRON_SWORD:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        elevate(1);
                        break;
                    case Items::STONE_AXE:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        wall();
                        we_switchItem(Items::STONE_AXE);

                        break;
                    case Items::DIAMOND_SWORD_ID:
                        if (pos[0] == noBlockHit) return;

                        int id  = lPlayer->lvl->getBlockId(pos[0], pos[1], pos[2]);
                        int aux = lPlayer->lvl->getBlockData(pos[0], pos[1], pos[2]);
                        
                        for (int i = 0; i < (sizeof(BlackListedBlocks) / sizeof(BlackListedBlocks[0])); i++) {
                            if (id == BlackListedBlocks[i]) {
                                id = 1;
                                aux = 0;
                                sendMessageFormated8("%s Selected Block is Black Listed and was Replaced with Stone", weText);
                            }
                        }

                        id = fixID(id);

                        if (!isMultiBlock) {
                            setBlockID = id;
                            setBlockAUX = aux;

                            sendMessageFormated8("%s Switched Set Block to %d:%d", weText, id, aux);
                        } else {
                            bool cont = true;
                            int setIndex = 0;
                            int arrLength = (sizeof(setMultiBlock) / sizeof(setMultiBlock[0]));
                            for (int i = 0; i < arrLength; i++) {
                                if (setMultiBlock[i] == -1 && cont) {
                                    cont = false;
                                    setMultiBlock[i] = (id << 16) | aux;
                                    setIndex = i;
                                } else if (setMultiBlock[arrLength - 1] != -1 && cont) {
                                    for (int j = 0; j < arrLength - 1; j++) {
                                        setMultiBlock[j + 1] = setMultiBlock[j];
                                    }
                                    setMultiBlock[arrLength - 1] = (id << 16) | aux;
                                    cont = false;
                                    setIndex = i;
                                }
                            }

                            sendMessageFormated8("%s Added Block %d:%d to Index %d", weText, id, aux, setIndex);
                        }

                        break;
                    }

                    active = true;
                } else if (btn & Button::ZR) {
                    if (active) return;

                    switch (itemID) {
                    case Items::STONE_HOE:
                        loadConfig(0);
                        break;
                    case Items::WOODEN_AXE:
                        if (pos[0] == noBlockHit) return;

                        sendMessageFormated8("%s Second position set to (%d, %d, %d)", weText, pos[0], pos[1], pos[2]);
                        pos2 = mc::BlockPos(pos[0], pos[1], pos[2]);
                        break;
                    case Items::IRON_HOE:
                        if (pos[0] == noBlockHit) return;

                        paste(pos); 
                        we_switchItem(Items::IRON_HOE);
                        break;
                    case Items::STONE_SHOVEL:
                        StoneShovelRadius++;
                        if (StoneShovelRadius > 10) StoneShovelRadius = 1;

                        sendMessageFormated8("%s Radius set to %d", weText, StoneShovelRadius);
                        break;
                    }

                    switch (itemID) {
                    case DIAMOND_PICKAXE:
                        noiseInvert = !noiseInvert;
                        sendMessageFormated8("%s Noise is now %sInverted", weText, noiseInvert ? "" : "Not ");

                        break;
                    case GOLDEN_SHOVEL:
                        maskedBlock = -1;
                        sendMessageFormated8("%s Disabled Masking", weText);
                
                        break;
                    case Items::IRON_SWORD:
                        if (pos1.x == noBlockHit || pos2.x == noBlockHit) return;

                        elevate(-1);
                        break;
                    case Items::DIAMOND_SWORD_ID:
                        isMultiBlock = !isMultiBlock;
                        sendMessageFormated8("%s Multi Set Block is now %s", weText, isMultiBlock ? "Enabled" : "Disabled");
                        if (!isMultiBlock) {
                            int arrLength = (sizeof(setMultiBlock) / sizeof(setMultiBlock[0]));
                            for (int i = 0; i < arrLength; i++) {
                                setMultiBlock[i] = -1;
                            }
                            sendMessageFormated8("%s Multi Set Block has been Reset", weText);
                        }
                        break;
                    }

                    active = true;
                } else if (btn & Button::DPAD_RIGHT) {
                    if (active) return;

                    switch (itemID) {
                    case Items::DIAMOND_PICKAXE:
                        noiseThreshold += 0.01f;
                        sendMessageFormated8("%s Threshold%s: %s", weText, "++", toCStr(noiseThreshold));
                        break;
                    }

                    active = true;
                } else if (btn & Button::DPAD_LEFT) {
                    if (active) return;

                    switch (itemID) {
                    case Items::DIAMOND_PICKAXE:
                        noiseThreshold -= 0.01f;
                        sendMessageFormated8("%s Threshold%s: %s", weText, "--", toCStr(noiseThreshold));
                        break;
                    }
                    
                    active = true;
                } else {
                    active = false;
                }
            }

            char* readFile(mc::FileInputStream& in) {
                char* t = _new<char>(in.filesize() + 1);
                for (int i = 0; i < in.filesize(); i++) t[i] = in.read();
                t[in.filesize()] = 0;
                in.close();
                return t;
            }

            template<typename... T>
            char* format(char* text, T... t) {
                char16_t* temp = _new<char16_t>(0x60);
                swprintf(temp, 0x60, charToChar16(text), t...);
                return char16ToChar(temp);
            }

            void saveConfig() {
                mc::File file = mc::File(u"WorldEdit.txt");
                if (file.exists()) {
                    mc::FileInputStream in = mc::FileInputStream(file);
                    char* content = readFile(in);

                    for (int i = 0; i < 100; i++) {
                        xf::String n = format("we_structure_%d.txt", i);

                        if (!mc::File(charToChar16(n.c_str())).exists()) {
                            mc::FileOutputStream out = mc::FileOutputStream(mc::File(charToChar16(n.c_str())));
                            
                            int* dataBuffer = (int*) 0x41000000;
                            int16_t* copyBuffer = (int16_t*) 0x42000000;
                            int16_t sizeY = copyBuffer[0];
                            int16_t sizeZ = copyBuffer[1];
                            int16_t sizeX = copyBuffer[2];

                            uint32_t dataBufferSize = 0;
                            uint32_t copyBufferIndex = 3;
                            for         (int y = 0; y < sizeY; y++) {
                                for     (int z = 0; z < sizeZ; z++) {
                                    for (int x = 0; x < sizeX; x++) {
                                        if (copyBuffer[copyBufferIndex++] > dataBufferSize) dataBufferSize++;
                                    }
                                }
                            }

                            for (int j = 0; j < dataBufferSize; j++) {
                                out.write(xf::String::intToStr(dataBuffer[j]));
                                if (j != (dataBufferSize - 1)) out.write(',');
                            }

                            out.write('.');
                            
                            copyBufferIndex = 0;
                            for         (int y = 0; y < sizeY; y++) {
                                for     (int z = 0; z < sizeZ; z++) {
                                    for (int x = 0; x < sizeX; x++) {
                                        out.write(xf::String::intToStr(copyBuffer[copyBufferIndex++]));
                                        out.write(',');
                                    }
                                }
                            }
                            out.write('0');
                            out.close();

                            out = mc::FileOutputStream(file);
                            out.write(content);
                            out.write(n);
                            out.write(',');
                            out.close();
                            return;
                        }
                    }
                } else {
                    mc::FileOutputStream out = mc::FileOutputStream(file);
                    out.write(0);
                    out.close();

                    saveConfig();
                }
            }

            void loadConfig(int index) {
                mc::File file = mc::File(u"WorldEdit.txt");
                if (file.exists()) {
                    mc::FileInputStream in = mc::FileInputStream(file);
                    char* content = readFile(in);
                    xf::String config = xf::String(content).split(',')[index];

                    sendMessageFormated8(config.c_str());

                    in = mc::FileInputStream(mc::File(charToChar16(config.c_str())));
                    content = readFile(in);
                    xf::String blockDataContent   = xf::String(content).split('.')[0];
                    xf::String lookUpTableContent = xf::String(content).split('.')[1];
                    
                    uint32_t* blockDataBuffer = (uint32_t*) 0x41000000;
                    int16_t*  copyBuffer      = (int16_t*)  0x42000000;

                    sendMessageFormated8("%d", blockDataContent.find(','));
                    for (int i = 0; i < blockDataContent.find(','); i++) {
                        blockDataBuffer[i] = xf::String::strToInt(blockDataContent.split(',')[i]);
                    }   

                    sendMessageFormated8("%d", lookUpTableContent.find(','));
                    for (int i = 0; i < lookUpTableContent.find(','); i++) {
                        int16_t n = (int16_t) xf::String::strToInt(lookUpTableContent.split(',')[i]);
                        //sendMessageFormated8("%d", n);
                        copyBuffer[i] = n;
                    }
                } else {
                    mc::FileOutputStream out = mc::FileOutputStream(file);
                    out.write(0);
                    out.close();
                }
            }

            void elevate(int amount) {
                pos1.y = pos1.y + amount;
                pos2.y = pos2.y + amount;

                sendMessageFormated8("%s Elevated Selection Area by %d (%d -> %d) (%d -> %d)", weText, amount, pos1.y - amount, pos1.y, pos2.y - amount, pos2.y);
            }

            void noiseTest() {
                Noise::PerlinNoise perlin(20000);
                float startX = min(toFloat(pos1.x), toFloat(pos2.x));
                float endX   = max(toFloat(pos1.x), toFloat(pos2.x)) + 1;
                float startY = min(toFloat(pos1.y), toFloat(pos2.y));
                float endY   = max(toFloat(pos1.y), toFloat(pos2.y)) + 1;
                float startZ = min(toFloat(pos1.z), toFloat(pos2.z));
                float endZ   = max(toFloat(pos1.z), toFloat(pos2.z)) + 1;

                int lastBlockGiven = -1;
                mc::Level* lvl = mc::Minecraft::getInstance()->thePlayer->lvl;
                
                auto place = [&](int x, int y, int z) {
                    SetSelectedBlock(lastBlockGiven);
                    if (lastBlockGiven != ((lvl->getBlockId(x, y, z) << 16) | (lvl->getBlockData(x, y, z)))) {
                        we_tp(x + 2, y + 2, z + 2);
                        we_placeBlock(x, y, z);
                    }
                };

                for         (int y = startY; y < endY; y++) {
                    for     (int z = startZ; z < endZ; z++) {
                        for (int x = startX; x < endX; x++) {
                            double noise = perlin.noise(toFloat(x), toFloat(y), toFloat(z), 5.0f, 1.0f);
                            
                            if        (noise < noiseThreshold && !noiseInvert) {
                                place(x, y, z);
                            } else if (noise > noiseThreshold &&  noiseInvert) {
                                place(x, y, z);
                            } else {
                                if (lvl->getBlockId(x, y, z) != 0) we_tp(x + 2, y + 2, z + 2);
                                we_breakBlock(x, y, z);
                            }
                        }
                    }
                }
            }

            void wall() {
                float startX = min(toFloat(pos1.x), toFloat(pos2.x));
                float endX   = max(toFloat(pos1.x), toFloat(pos2.x)) + 1;
                float startY = min(toFloat(pos1.y), toFloat(pos2.y));
                float endY   = max(toFloat(pos1.y), toFloat(pos2.y)) + 1;
                float startZ = min(toFloat(pos1.z), toFloat(pos2.z));
                float endZ   = max(toFloat(pos1.z), toFloat(pos2.z)) + 1;
            
                int sizeX = (int) floorf(endX - startX);
                int sizeY = (int) floorf(endY - startY);
                int sizeZ = (int) floorf(endZ - startZ);
                
                int lastBlockGiven = -1;
                for (int y = 0; y < sizeY; y++) {
                    for (int i = 1; i < sizeX; i++) {
                        SetSelectedBlock(lastBlockGiven);
                        we_tp(        i + startX + 2, y + startY + 2, startZ + 2);
                        we_placeBlock(i + startX,     y + startY,     startZ);
                    }

                    for (int i = 1; i < sizeX; i++) {
                        SetSelectedBlock(lastBlockGiven);
                        we_tp(        i + startX + 2, y + startY + 2, endZ - 1 + 2);
                        we_placeBlock(i + startX,     y + startY,     endZ - 1);
                    }

                    for (int i = 0; i < sizeZ; i++) {
                        SetSelectedBlock(lastBlockGiven);
                        we_tp(        startX + 2, y + startY + 2, i + startZ + 2);
                        we_placeBlock(startX,     y + startY,     i + startZ);
                    }

                    for (int i = 1; i < sizeZ - 1; i++) {
                        SetSelectedBlock(lastBlockGiven);
                        we_tp(        endX - 1 + 2, y + startY + 2, i + startZ + 2);
                        we_placeBlock(endX - 1,     y + startY,     i + startZ);
                    }
                }
            }

            void center() {
                float startX = min(toFloat(pos1.x), toFloat(pos2.x));
                float endX   = max(toFloat(pos1.x), toFloat(pos2.x)) + 1;
                float startY = min(toFloat(pos1.y), toFloat(pos2.y));
                float endY   = max(toFloat(pos1.y), toFloat(pos2.y)) + 1;
                float startZ = min(toFloat(pos1.z), toFloat(pos2.z));
                float endZ   = max(toFloat(pos1.z), toFloat(pos2.z)) + 1;
                    
                float sizeX = endX - startX;
                float sizeY = endY - startY;
                float sizeZ = endZ - startZ;
                    
                int x = (int) floorf(startX + (sizeX / 2.0)); 
                int y = (int) floorf(startY + (sizeY / 2.0)); 
                int z = (int) floorf(startZ + (sizeZ / 2.0));
                    
                we_tp(x + 2, y + 2, z + 2);
                we_switchItem((Items) 133);
                we_placeBlock(x, y, z);
            }

            void line(int id) {
                int* posA = _new<int>(3);
                posA[0] = pos1.x;
                posA[1] = pos1.y; 
                posA[2] = pos1.z;
                int* posB = _new<int>(3);
                posB[0] = pos2.x;
                posB[1] = pos2.y;
                posB[2] = pos2.z;

                float yawPitch[2] = { calculateYaw(posA, posB), calculatePitch(posA, posB) };
                float dist = distanceTwoPoint(posA, posB);

                float* lookAtVec = getLookAtVector(yawPitch[0], yawPitch[1]);
                bool contin = true;
                int i = 0;
                while (contin) {
                    int* xyz = _new<int>(3);
                    xyz[0] = floorf(toFloat(posA[0]) + (lookAtVec[0] * i));
                    xyz[1] = floorf(toFloat(posA[1]) + (lookAtVec[1] * i));
                    xyz[2] = floorf(toFloat(posA[2]) + (lookAtVec[2] * i));

                    float newDist = distanceTwoPoint(xyz, posB);
                    if (i >= dist) contin = false;

                    we_tp(xyz[0] + 2, xyz[1] + 2, xyz[2] + 2);
                    //sendMessageFormated8("%d %s %s %s %s", i, xf::utils::convertFloatToText(dist), 
                    //xf::utils::convertFloatToText(newDist), contin ? "true" : "false", xf::utils::convertFloatToText(yawPitch[1]));

                    if (id == 0) we_tp(xyz[0], xyz[1], xyz[2]);
                    i++;
                }
            }

            void copy() {
                float startX = min(toFloat(pos1.x), toFloat(pos2.x));
                float endX   = max(toFloat(pos1.x), toFloat(pos2.x)) + 1;
                float startY = min(toFloat(pos1.y), toFloat(pos2.y));
                float endY   = max(toFloat(pos1.y), toFloat(pos2.y)) + 1;
                float startZ = min(toFloat(pos1.z), toFloat(pos2.z));
                float endZ   = max(toFloat(pos1.z), toFloat(pos2.z)) + 1;

                blockData* blockDataBuffer = (blockData*) 0x41000000;
                int16_t* copyBuffer = (int16_t*) 0x42000000;
                copyBuffer[0] = floorf(endY - startY);
                copyBuffer[1] = floorf(endZ - startZ);
                copyBuffer[2] = floorf(endX - startX);
                
                int copyBufferIndex = 3;
                int blackListCounter = 0;
                int blockDataIndex = 0;

                for         (int y = startY; y < endY; y++) {
                    for     (int z = startZ; z < endZ; z++) {
                        for (int x = startX; x < endX; x++) {
                            mc::Level* level = mc::Minecraft::getInstance()->thePlayer->lvl;
                            int blockID  = level->getBlockId(x, y, z);
                            int blockAUX = level->getBlockData(x, y, z);
                            for (int i = 0; i < (sizeof(BlackListedBlocks) / sizeof(BlackListedBlocks[0])); i++) {
                                if (blockID == BlackListedBlocks[i]) {
                                    blockID = 1;
                                    blockAUX = 0;
                                    blackListCounter++;
                                }
                            }

                            blockID = fixID(blockID);

                            if (blockAUX > 0x7FFF) blockAUX = 0;
                            if (blockID  > 0x7FFF) blockID  = 0;
                            int alreadyExists = -1;
                            for (int i = 0; i < blockDataIndex; i++) {
                                if (blockDataBuffer[i].id == blockID && blockDataBuffer[i].aux == blockAUX) {
                                    alreadyExists = i;
                                }
                            }

                            if (alreadyExists == -1) {
                                blockDataBuffer[blockDataIndex++] = { (int16_t) blockID, (int16_t) blockAUX };
                                alreadyExists = blockDataIndex - 1;
                            }

                            copyBuffer[copyBufferIndex++] = alreadyExists;
                        }
                    }
                }

                isSomethingInsideBuffer = true;
                sendMessageFormated8("%s Copied Structure", weText);
                if (blackListCounter != 0) sendMessageFormated8("%s %d Black Listed Block%s Replaced with Stone", weText, blackListCounter, blackListCounter == 1 ? " was" : "s were");
            }

            void paste(int* pos) {
                if (!isSomethingInsideBuffer) {
                    sendMessageFormated8("%s Nothing Copied", weText);
                    return; 
                }

                int* lastTP = _new<int>(3);
                for (int i = 0; i < 3; i++) lastTP[i] = noBlockHit;
                
                auto tp = [&](int x, int y, int z) {
                    if ((distanceTwoPoint(x, y, z, lastTP[0], lastTP[1], lastTP[2]) >= 5 || y != lastTP[1])) {
                        lastTP[0] = x;
                        lastTP[1] = y;
                        lastTP[2] = z;
                        we_tp(x + 2, y + 2, z + 2);
                    }
                }; 

                blockData* blockDataBuffer = (blockData*) 0x41000000;
                int16_t* copyBuffer = (int16_t*) 0x42000000;
                int16_t sizeY = copyBuffer[0];
                int16_t sizeZ = copyBuffer[1];
                int16_t sizeX = copyBuffer[2];

                int copyBufferIndex = 3;

                uint32_t lastFinal = 0;
                for         (int y = 0; y < sizeY; y++) {
                    for     (int z = 0; z < sizeZ; z++) {
                        for (int x = 0; x < sizeX; x++) {
                            blockData data = blockDataBuffer[copyBuffer[copyBufferIndex++]];

                            Items blockID = (Items) data.id;
                            int blockAUX = data.aux;
                            uint32_t finalOutput = (data.id << 16) | data.aux;
                            if (finalOutput != lastFinal && blockID != 0) {
                                we_switchItem(blockID, blockAUX);
                            }

                            if (blockID == 0) {
                                tp           (x + pos[0] - (sizeX / 2), y + pos[1], z + pos[2] - (sizeZ / 2));
                                we_breakBlock(x + pos[0] - (sizeX / 2), y + pos[1], z + pos[2] - (sizeZ / 2));
                            } else {
                                tp           (x + pos[0] - (sizeX / 2), y + pos[1], z + pos[2] - (sizeZ / 2));
                                we_placeBlock(x + pos[0] - (sizeX / 2), y + pos[1], z + pos[2] - (sizeZ / 2));
                            }
                            lastFinal = finalOutput;
                        }
                    }
                }

                sendMessageFormated8("%s Pasted Structure", weText);
            }

            float calculateYaw(int* posA, int* posB) {
                double diffX = posB[0] - posA[0];
		        double diffZ = posB[2] - posA[2];
                float yaw    = (float) (atan2f(diffZ, diffX) * 180.0 / M_PI) - 90.0f;
                return isnan(yaw) ? 0.0f : yaw;
            }

            float calculatePitch(int* posA, int* posB) {
                double diffX = posB[0] - posA[0];
                double diffY = posB[1] - posA[1];
		        double diffZ = posB[2] - posA[2];
                double dist  = sqrtf(powf(diffX, 2) + powf(diffZ, 2));
		        float pitch  = (float) -(atan2f(diffY, dist) * 180.0 / M_PI);
                return isnan(pitch) ? 0.0f : pitch;
            }
            
            float distanceTwoPoint(int* pointA, int* pointB) {
                return sqrtf(powf(toFloat(pointA[0] - pointB[0]), 2) + powf(toFloat(pointA[1] - pointB[1]), 2) + powf(toFloat(pointA[2] - pointB[2]), 2));
            }

            float distanceTwoPoint(int pointAX, int pointAY, int pointAZ, int pointBX, int pointBY, int pointBZ) {
                int* pointA = _new<int>(3);
                pointA[0] = pointAX;
                pointA[1] = pointAY;
                pointA[2] = pointAZ;

                int* pointB = _new<int>(3);
                pointB[0] = pointBX;
                pointB[1] = pointBY;
                pointB[2] = pointBZ;
                
                return distanceTwoPoint(pointA, pointB);
            }

            static float degreeToRadian(float deg) {
                return (deg * M_PI / 180.0f);
            }

            static float radianToDegree(float rad) {
                return (rad * 180.0f / M_PI);
            }

            int fixID(int id) {
                uint16_t toFix[]    = {  300,  299,  298,  297,  296,  295,  273,  259 };
                uint16_t toReturn[] = { 1068, 1067, 1066, 1065, 1064, 1063, 1041, 1027 };
                for (int i = 0; i < (sizeof(toFix) / sizeof(toFix[0])); i++) {
                    if (toFix[i] == id) return toReturn[i];
                }

                return id;
            }

            void breakSphere(int* center, int radius) {
                int* lastTP = _new<int>(3);
                for (int i = 0; i < 3; i++) lastTP[i] = noBlockHit;
                
                auto tp = [&](int x, int y, int z) {
                    lastTP[0] = x;
                    lastTP[1] = y;
                    lastTP[2] = z;
                    we_tp(x, y, z);
                }; 

                int lastBlockGiven = -1;

                for         (int x = -radius; x < radius + 1; x++) {
                    for     (int y = -radius; y < radius + 1; y++) {
                        for (int z = -radius; z < radius + 1; z++) {
                            int* spherePos = _new<int>(3);
                            spherePos[0] = center[0] + x;
                            spherePos[1] = center[1] + y;
                            spherePos[2] = center[2] + z;

                            float dist = distanceTwoPoint(center, spherePos);
                            int blockId  = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(spherePos[0], spherePos[1], spherePos[2]);
                            int blockAux = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockData(spherePos[0], spherePos[1], spherePos[2]);
                            if (dist <= radius) {
                                if (distanceTwoPoint(spherePos, lastTP) >= 5) tp(spherePos[0] + 2, spherePos[1] + 2, spherePos[2] + 2);
                                SetSelectedBlock(lastBlockGiven);
                                if (maskedBlock != -1) {
                                    if ((maskedBlock >> 16) == blockId && (maskedBlock & 0xFFFF) == blockAux) {
                                        we_placeBlock(spherePos[0], spherePos[1], spherePos[2]);
                                    }
                                } else {
                                    we_placeBlock(spherePos[0], spherePos[1], spherePos[2]);
                                }
                            }
                        }
                    }
                }
            }

            int* getBlockHit(mc::LocalPlayer* lPlayer, int reach = 150) {
                float* lookAt = getLookAtVector(lPlayer);
                int* pos = _new<int>(3);
                
                for (int i = 0; i < 3; i++) {
                    pos[i] = noBlockHit;
                }

                int precision = 10;
                bool hit = false;
                for (int i = 0; i < (reach * precision); i++) {
                    int x = (int) floorf(lPlayer->x +                           ((double) (lookAt[0] * ((float) i) * (1.0f / precision))));
                    int y = (int) floorf(lPlayer->y + lPlayer->getEyeHeight() + ((double) (lookAt[1] * ((float) i) * (1.0f / precision))));
                    int z = (int) floorf(lPlayer->z +                           ((double) (lookAt[2] * ((float) i) * (1.0f / precision))));
                
                    int id = lPlayer->lvl->getBlockId(x, y, z);
                    if (id != 0 && !hit) {
                        hit = true;
                        pos[0] = x;
                        pos[1] = y;
                        pos[2] = z;
                    }
                }

                return pos;
            }

            void fill() {
                float startX = min(toFloat(pos1.x), toFloat(pos2.x));
                float endX   = max(toFloat(pos1.x), toFloat(pos2.x)) + 1;
                float startY = min(toFloat(pos1.y), toFloat(pos2.y));
                float endY   = max(toFloat(pos1.y), toFloat(pos2.y)) + 1;
                float startZ = min(toFloat(pos1.z), toFloat(pos2.z));
                float endZ   = max(toFloat(pos1.z), toFloat(pos2.z)) + 1;

                int lastBlockGiven = -1;

                for         (int y = startY; y < endY; y++) {
                    for     (int z = startZ; z < endZ; z++) {
                        for (int x = startX; x < endX; x++) {
                            we_tp(x + 2, y + 2, z + 2);
                            SetSelectedBlock(lastBlockGiven);
                            if (maskedBlock == -1) {
                                we_placeBlock(x, y, z);
                            } else {
                                int BlockId = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(x, y, z);
                                int BlockAux = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockData(x, y, z);
                                if ((maskedBlock >> 16) == BlockId && (maskedBlock & 0xFFFF) == BlockAux) {
                                    we_placeBlock(x, y, z);
                                }
                            }
                        }
                    }
                }
            }

            void SetSelectedBlock(int &lastBlockGiven) {
                if (isMultiBlock) {
                    int count = 0;
                    int arrLength = (sizeof(setMultiBlock) / sizeof(setMultiBlock[0]));
                    for (int i = 0; i < arrLength; i++) {
                        if (setMultiBlock[i] != -1) count++;
                    }

                    if (count != 0) {
                        int index = xf::utils::Random::next(1, count) - 1;
                        int id = (setMultiBlock[index] >> 16);
                        int aux = (setMultiBlock[index] & 0xFFFF);
                        if (setMultiBlock[index] != lastBlockGiven || lastBlockGiven == -1) we_switchItem((Items) id, aux);
                            lastBlockGiven = setMultiBlock[index];
                        } else {
                            if (((1 << 16) | 0) != lastBlockGiven || lastBlockGiven == -1) we_switchItem(Items::STONE);
                            lastBlockGiven = 1 << 16;
                        }
                    } else {
                        if (((setBlockID << 16) | setBlockAUX) != lastBlockGiven || lastBlockGiven == -1) we_switchItem((Items) setBlockID, setBlockAUX);
                        lastBlockGiven = (setBlockID << 16) | setBlockAUX; 
                    }
            }
            
            static float* getLookAtVector(float yaw, float pitch) {
                float* arr = _new<float>(3);
                arr[0] = fabs(sinf(degreeToRadian(pitch - 90.0f))) * sinf(degreeToRadian(yaw   - 180.0f));
                arr[1] =                                             sinf(degreeToRadian(pitch - 180.0f));
                arr[2] = fabs(sinf(degreeToRadian(pitch - 90.0f))) * cosf(degreeToRadian(yaw));

                return arr;
            }

            static float* getLookAtVector(mc::LocalPlayer* lPlayer) {
                return getLookAtVector(lPlayer->yaw, lPlayer->pitch);
            }

            void onTick() {
                btnHandler();
            }

            void reset() {
                int arrLength = (sizeof(setMultiBlock) / sizeof(setMultiBlock[0]));
                for (int i = 0; i < arrLength; i++) {
                    setMultiBlock[i] = -1;
                }
                active = false;
                pos1 = mc::BlockPos(noBlockHit, noBlockHit, noBlockHit);
                pos2 = mc::BlockPos(noBlockHit, noBlockHit, noBlockHit);
            }

            int getItemID(mc::Item* item) {
                return mc::Item::getId(item);
            }

            static void we_tp(int x, int y, int z) {
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundMovePlayerPacket(toFloat(x), toFloat(y), toFloat(z), true, true)));
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);
            }

            static void we_breakBlock(int x, int y, int z) {
                int id = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(x, y, z);
                if (id == 0) return;
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundPlayerActionPacket(0, {x, y, z})));
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);
            }

            void we_swapHands(mc::LocalPlayer* lPlayer) {
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundPlayerActionPacket(6, lPlayer->pos2)));
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);
            }

            void we_placeBlock(int x, int y, int z) {
                int id = mc::Minecraft::getInstance()->thePlayer->lvl->getBlockId(x, y, z);
                if (id != 0) we_breakBlock(x, y, z);
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundUseItemOnPacket({x, y, z})));
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);
            }

            void we_switchItem(Items itemId, int auxVal = 0) {
                mc::Item* item = mc::Item::byId(itemId);
                mboost::shared_ptr<mc::ItemInstance> itemv2(_new1(mc::ItemInstance(item, 64)));
                itemv2->setAuxValue(auxVal);
                mboost::shared_ptr<mc::Packet> packet(_new1(mc::ServerboundSetCreativeModeSlotPacket(itemv2, InventorySlotId::FIRST_SLOT)));
                mc::Minecraft::getInstance()->getConnection(0)->send(packet);
            }

            mc::Item* getCurrentItem(mc::LocalPlayer* lPlayer) {
                mboost::shared_ptr<mc::ItemInstance> item;
                lPlayer->getCarriedItem(item);
                return item->item;
            }

            template<typename... T>
            static void sendMessageFormated16(const char16_t* msg, T... t) {
                char16_t* tmp = _new<char16_t>(0x60);
                swprintf(tmp, 0x60, msg, t...);
                mc::Minecraft::getInstance()->gui->addMessage(tmp, 0, false, false, false, false);
            }

            template<typename... T>
            static void sendMessageFormated8(const char* msg, T... t) {
                char16_t* tmp = _new<char16_t>(0x60);
                swprintf(tmp, 0x60, charToChar16(msg), t...);
                mc::Minecraft::getInstance()->gui->addMessage(tmp, 0, false, false, false, false);
            }

        private:
            bool active;
            mc::BlockPos pos1;
            mc::BlockPos pos2;

            int noBlockHit = -32767;
            bool isSomethingInsideBuffer = false;
            int setBlockID = 1;
            int setBlockAUX = 0;
            bool isMultiBlock = false;
            int setMultiBlock[64];
            const uint8_t BlackListedBlocks[4] = { 43, 125, 204, 181 };
            int maskedBlock = -1;
            float noiseThreshold = 0.04;
            bool noiseInvert = false;

            int StoneShovelRadius = 5;
            const char* weText = "[WorldEdit]";
            WorldEditTreeEnum DiamondAxeTree = WorldEditTreeEnum::OAK;
        };
    }
}