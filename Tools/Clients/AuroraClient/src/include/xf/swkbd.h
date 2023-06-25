#pragma once

namespace mc {
    class SWKBD {
    public:
        SWKBD() {

        }

        void draw() {
            char keyboard[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',
                                'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '/', 
                                'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',  27, // ' 
                                'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '?', '!'  };

            int keys = sizeof(keyboard) / sizeof(keyboard[0]);
            
            auto box = [&](char key) {
                
            };
        }

        void onTick() {

        }

        char* getCurrentText() { return currentText; }

    private:
        int currentTextLength = 0;
        char currentText[32];
    };
}