#include <code/code.h>

#include <minecraft/mc/ServerboundMovePlayerPacket.h>
#include <minecraft/mc/ServerboundChatPacket.h>
#include <minecraft/mc/GiveItemCommand.h>
#include <minecraft/mc/miscellanious.h>
#include <minecraft/mc/ServerPlayer.h>
#include <minecraft/mc/Minecraft.h>
#include <minecraft/mc/Font.h>
#include <minecraft/mc/Gui.h>

#include <xf/PlayerUtils.h>
#include <xf/DrawHelper.h>
#include <xf/Players.h>
#include <xf/Random.h>
#include <xf/GUI.h>

#include <minecraft/mc/BufferedImage.h>
#include <minecraft/mstd/WString.h>
#include <minecraft/mc/Textures.h>

#include <minecraft/mc/FileOutputStream.h>
#include <minecraft/mc/FileInputStream.h>

xf::GUI::GUI* gui = nullptr;
bool isGuiInit = false;
int frameCounter = 0;
bool isActive = false;

void code_main() {
    if (!isGuiInit) {
        gui = _new2(xf::GUI::GUI(_new2(mstd::wstring(u"AuroraClient"))));
        isGuiInit = true;
        gui->setOutLineSize(2.5f);
        gui->setSizeX(120.0f);
        gui->setSizeY(185.0f);
        gui->setPosX(30.0f);
        gui->setPosY(35.0f);
        gui->setRGBSpeed(1);

        int pageCount = 11;
        xf::GUI::Page* pages = _new3<xf::GUI::Page>(pageCount);
        for (int i = 0; i < pageCount; i++) {
            pages[i] = xf::GUI::Page(u"Page", i);
            if (i == 0) pages[i]  = xf::GUI::Page(u"Combat", i);
            if (i == 1) pages[i]  = xf::GUI::Page(u"Movement", i);
            if (i == 2) pages[i]  = xf::GUI::Page(u"Player", i);
            if (i == 3) pages[i]  = xf::GUI::Page(u"Visual", i);
            if (i == 4) pages[i]  = xf::GUI::Page(u"World", i);
            if (i == 5) pages[i]  = xf::GUI::Page(u"Miscellaneous", i);
            if (i == 6) pages[i]  = xf::GUI::Page(u"Client Settings", i);
            if (i == 7) pages[i]  = xf::GUI::Page(u"Players", i);
            if (i == 8) pages[i]  = xf::GUI::Page(u"", i);
            if (i == 9) pages[i]  = xf::GUI::Page(u"", i);
            if (i == 10) pages[i] = xf::GUI::Page(u"", i);

            int moduleCount = 0;
            switch (i) {
            case 0: // Combat
                moduleCount = 5;
                break;
            case 1: // Movement
                moduleCount = 8;
                break;
            case 2: // Player
                moduleCount = 6;
                break;
            case 3: // Visual
                moduleCount = 5;
                break;
            case 4: // World
                moduleCount = 2;
                break;
            case 5: // Miscellaneous
                moduleCount = 1;
                break;
            case 6: // Client Settings
                moduleCount = 5;
                break;
            case 7: // Players
                moduleCount = 9;
                break;
            case 8: // Player Settings
                moduleCount = 7;
                break;
            case 9: // Every Player Setting
                moduleCount = 6;
                break;
            case 10: // Aim Assist
                moduleCount = 3;
                break;
            }

            xf::GUI::Module* modules = _new3<xf::GUI::Module>(moduleCount);
            
            for (int j = 0; j < moduleCount; j++) {
                modules[j] = xf::GUI::Module(u"Module", -xf::utils::Random::next(300, 500));
            }

            if (i == 0) modules[0] = xf::GUI::Module(AURA_TEXT, AURA_ID);
            if (i == 0) modules[1] = xf::GUI::Module(AUTOCLICKER_TEXT, AUTOCLICKER_ID);
            if (i == 0) modules[2] = xf::GUI::Module(BOW_SPAM_TEXT, BOW_SPAM_ID);
            if (i == 0) modules[3] = xf::GUI::Module(AIMBOT_TEXT, AIMBOT_ID);
            if (i == 0) modules[4] = xf::GUI::Module(AIMASSIST_TEXT, AIMASSIST_ID);
            
            if (i == 1) modules[0] = xf::GUI::Module(STEP_TEXT, STEP_ID);
            if (i == 1) modules[1] = xf::GUI::Module(BHOP_TEXT, BHOP_ID);
            if (i == 1) modules[2] = xf::GUI::Module(ANTI_KB_TEXT, ANTI_KB_ID);
            if (i == 1) modules[3] = xf::GUI::Module(FLY_TEXT, FLY_ID);
            if (i == 1) modules[4] = xf::GUI::Module(RUN_AND_EAT_TEXT, RUN_AND_EAT_ID);
            if (i == 1) modules[5] = xf::GUI::Module(NO_CLIP_TEXT, NO_CLIP_ID);
            if (i == 1) modules[6] = xf::GUI::Module(AUTO_SPRINT_TEXT, AUTO_SPRINT_ID);
            if (i == 1) modules[7] = xf::GUI::Module(SPEED_TEXT, SPEED_ID);
            //if (i == 1) modules[8] = xf::GUI::Module(SCAFFOLD_TEXT, SCAFFOLD_ID);
            
            if (i == 2) modules[0] = xf::GUI::Module(RANDOM_NNID_TEXT, RANDOM_NNID_ID);
            if (i == 2) modules[1] = xf::GUI::Module(INSTANT_CRAFT_TEXT, INSTANT_CRAFT_ID);
            if (i == 2) modules[2] = xf::GUI::Module(ALL_GAMEMODE_TEXT, ALL_GAMEMODE_ID);
            if (i == 2) modules[3] = xf::GUI::Module(SWAP_HANDS_TEXT, SWAP_HANDS_ID);
            if (i == 2) modules[4] = xf::GUI::Module(BOOST_PLAYER_TEXT, BOOST_PLAYER_ID);
            if (i == 2) modules[5] = xf::GUI::Module(RANDOM_XUID_TEXT, RANDOM_XUID_ID);
            //if (i == 2) modules[6] = xf::GUI::Module(PATH_WALKER_TEXT, PATH_WALKER_ID);
            
            if (i == 3) modules[0] = xf::GUI::Module(SHOW_POSITION_TEXT, SHOW_POSITION_ID);
            if (i == 3) modules[1] = xf::GUI::Module(FULLBRIGHT_TEXT, FULLBRIGHT_ID);
            if (i == 3) modules[2] = xf::GUI::Module(ARRAY_LIST_TEXT, ARRAY_LIST_ID);
            if (i == 3) modules[3] = xf::GUI::Module(NIGHT_VISION_TEXT, NIGHT_VISION_ID);
            if (i == 3) modules[4] = xf::GUI::Module(KEYSTROKES_TEXT, KEYSTROKES_ID);

            if (i == 4) modules[0] = xf::GUI::Module(NUKER_TEXT, NUKER_ID);
            if (i == 4) modules[1] = xf::GUI::Module(WORLDEDIT_TEXT, WORLDEDIT_ID);

            if (i == 5) modules[0] = xf::GUI::Module(CREDITS_TEXT, CREDITS_ID);

            if (i == 6) modules[0] = xf::GUI::Module(POSITION_X_TEXT, POSITION_X_ID);
            if (i == 6) modules[1] = xf::GUI::Module(POSITION_Y_TEXT, POSITION_Y_ID);
            if (i == 6) modules[2] = xf::GUI::Module(RGB_SPEED_TEXT, RGB_SPEED_ID);
            if (i == 6) modules[3] = xf::GUI::Module(SAVE_CONFIG_TEXT, SAVE_CONFIG_ID);
            if (i == 6) modules[4] = xf::GUI::Module(CLIENT_NAME_TEXT, CLIENT_NAME_ID);

            if (i == 7) modules[0] = xf::GUI::Module(PLAYER_ALL_TEXT, PLAYER_ALL_ID);
            if (i == 7) modules[1] = xf::GUI::Module(PLAYER_TEXT, PLAYER_1_ID);
            if (i == 7) modules[2] = xf::GUI::Module(PLAYER_TEXT, PLAYER_2_ID);
            if (i == 7) modules[3] = xf::GUI::Module(PLAYER_TEXT, PLAYER_3_ID);
            if (i == 7) modules[4] = xf::GUI::Module(PLAYER_TEXT, PLAYER_4_ID);
            if (i == 7) modules[5] = xf::GUI::Module(PLAYER_TEXT, PLAYER_5_ID);
            if (i == 7) modules[6] = xf::GUI::Module(PLAYER_TEXT, PLAYER_6_ID);
            if (i == 7) modules[7] = xf::GUI::Module(PLAYER_TEXT, PLAYER_7_ID);
            if (i == 7) modules[8] = xf::GUI::Module(PLAYER_TEXT, PLAYER_8_ID);

            if (i == 8) modules[0] = xf::GUI::Module(ID_TEXT, ID_ID);
            if (i == 8) modules[1] = xf::GUI::Module(AMOUNT_TEXT, AMOUNT_ID);
            if (i == 8) modules[2] = xf::GUI::Module(AUX_TEXT, AUX_ID);
            if (i == 8) modules[3] = xf::GUI::Module(GIVE_TEXT, GIVE_ID);
            if (i == 8) modules[4] = xf::GUI::Module(GIVE_ALL_TEXT, GIVE_ALL_ID);
            if (i == 8) modules[5] = xf::GUI::Module(TELEPORT_TO_TEXT, TELEPORT_TO_ID);
            if (i == 8) modules[6] = xf::GUI::Module(HIT_PLAYER_TEXT, HIT_PLAYER_ID);

            if (i == 9) modules[0] = xf::GUI::Module(ID_TEXT, ID_ID);
            if (i == 9) modules[1] = xf::GUI::Module(AMOUNT_TEXT, AMOUNT_ID);
            if (i == 9) modules[2] = xf::GUI::Module(AUX_TEXT, AUX_ID);
            if (i == 9) modules[3] = xf::GUI::Module(GIVE_TEXT, GIVE_ID);
            if (i == 9) modules[4] = xf::GUI::Module(GIVE_ALL_TEXT, GIVE_ALL_ID);
            if (i == 9) modules[5] = xf::GUI::Module(HIT_PLAYER_TEXT, HIT_PLAYER_ID);

            if (i == 10) modules[0] = xf::GUI::Module(AIMASSIST_STRENGTH_TEXT, AIMASSIST_STRENGTH_ID);
            if (i == 10) modules[1] = xf::GUI::Module(AIMASSIST_RANGE_TEXT, AIMASSIST_RANGE_ID);
            if (i == 10) modules[2] = xf::GUI::Module(AIMASSIST_ENABLE_TEXT, AIMASSIST_ENABLE_ID);

            pages[i].setModules(modules, moduleCount);
        }
 
        gui->setPages(pages, pageCount);

        gui->setNotification(u"AuroraClient v1.0", u"Created by David xF#1337");
        gui->setNotifDuration(4500);

        mc::File file = mc::File(CONFIG_FILE_NAME);
        mc::File nameFile = mc::File(u"Names.txt");
        if (!nameFile.exists()) {
            xf::String nameContent = "Raymen22,samroogers,Domestic_Appetit,hellokitty4ever,KM_look,KANEMC123,senzitivebebe,Kiwi_Bear123,yunkeun0104,ImaMinor000,Jobnazz,coxaini,DarKerss_x,Skywalker_TW,Sxiems,firechickinoodle,zzzslayer123,Minefun_123,CUUMformedady,derrreekcarrr,wkrentarka,dark_musician,Loveintacos,whatdapoopoo3242,Bqlt,CHENGPENGZHAO,rip_kam,OMGNewJeans,JManCrafting,Lord_Hashbrown,cyberkero,firewarrior99,ItzAnos,opps__,AmeliaNotFound6,Acknown,hyfs2959,Roanukay,LostHikki,iM_Fury0,PcGamerJosh,asdfghjklp,lovedbymusic,Whizzing,PONTOLEGIT,ilywissal,eatmyheart,SonicBW,DyingRich,Jokew78,Cheesyboy13582,Kuinsho,roxas333,ChimCiao,laninha_5k,Baxtre,saraaaundso,nicolvssg,IllIlIlllIIIlI,The_Dre05,Hirigana,DanniB0y__,CrystalFast,Jhonyy,Ramiz_Karaseksi,OsamaBinDancin,candymain69,snugg1e,AfterFairy,AlgoFasst,TheRealRibal,xxungie,badach,Velxyu,try4Luck,Itz_ZI,Sxhara,eth777,9balas,khunmybeloved,EthanolThePro,dariger,Bapilapilapilith,itsmeBFG,Lizerius,JulciaFortnite69,NeverMan,BookGame,m1kus,DMC8706,ZeroVice,darkgangethan,ICHTRETEINOMA,JohnnyPaw,xRadent,Kid_Benger,SaleAdd,gyyoza,AssGame,DarkBlue_LM,roalv123456789,rushccx,TheZkillerZZ,Liamdekraai2000,WaflesPad,jckina,TTVRodinioo,AMiR_LeGeNdArY,ORYX_OW,T_one96,someone_cool123,Sokerri__,Superhyperninja,poopyfartgrl,KZA_Zelq,Blitzzzzzzzzzzz,MinionCult,PeetaTheHutt,dannygreen10,Finanzen,Xuccie,marakasu8931,xDragonne73,MantidenKek,Arfelis,hahahahahahaah,BooskHalloween,fySwAyplayZz,Aslanaka";

            mc::FileOutputStream out = mc::FileOutputStream(nameFile);
            out.write(nameContent);
            out.close();
        }
        
        xf::GUI::Page page = pages[6];
        for (int i = 0; i < page.getModuleCount(); i++) {
            xf::GUI::Module* module = &page[i];
            if (module->getID() == CLIENT_NAME_ID) module->toggle();
        }

        if (file.exists()) {
            mc::FileInputStream* istr = _new1(mc::FileInputStream(file));
            char* t = _new<char>(istr->filesize() + 1);
            for (int i = 0; i < istr->filesize(); i++) t[i] = istr->read();
            t[istr->filesize()] = 0;
            istr->close();

            xf::String* config = xf::String(t).split(',');
            gui->setPosX(toFloat(xf::String::strToInt(config[0])));
            gui->setPosY(toFloat(xf::String::strToInt(config[1])));
            gui->setRGBSpeed(xf::String::strToInt(config[2]));

            int m = xf::String::strToInt(config[3]);
            page = pages[3];
            for (int i = 0; i < page.getModuleCount(); i++) {
                xf::GUI::Module* module = &page[i];
                int n = page.getModuleCount() - i - 1;
                bool isEnabled = (m >> n) & 1;
                if (isEnabled) module->toggle();
            }

            page = pages[6];
            for (int i = 0; i < page.getModuleCount(); i++) {
                xf::GUI::Module* module = &page[i];
                if (module->getID() == CLIENT_NAME_ID) {
                    bool isEnabled = xf::String::strToInt(config[4]);
                    if (!isEnabled) module->toggle();
                }
            }
        } else {
            gui->saveConfig(false);
        }
    }
    
    //gui->setNotification("Test", "%08X|%08X", mc::System::processTimeInMilliSecs().p1, mc::System::processTimeInMilliSecs().p2);

    frameCounter++;
    xf::GUI::frameCount++;
    xf::utils::Random::next(0, 5000);

    mc::Minecraft* minecraft = mc::Minecraft::getInstance();
    mc::Font* font = minecraft->defaultFonts;
    mc::LocalPlayer* lPlayer = minecraft->thePlayer;
    
    xf::GUI::DrawHelper::renderSetup();
    if (isGuiInit && lPlayer != nullptr) gui->onTick();

    if (isGuiInit && lPlayer == nullptr) {
        xf::GUI::TextColorIndex += gui->getRGBSpeed();
    }

    mc::GlStateManager::enableDepthTest();
}