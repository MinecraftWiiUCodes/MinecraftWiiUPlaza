#pragma once

#include <minecraft/mc/miscellanious.h>

namespace xf {
    class String {
    public:
        String(const char* _str) {
            bool found = false;
	        int l = 0;
            while (!found) {
	        	if (_str[l++] == 0x0) {
	        		found = true;
	        		l--;
	        	}
	        }
            
            size = l;
            string = _new<char>(size + 1);
            for (int i = 0; i < size; i++) {
                string[i] = _str[i];
            }

            string[size] = 0x0;
        }

        static String intToStr(int input) {
            char16_t* t = _new<char16_t>(0x20);
            swprintf(t, 0x20, charToChar16("%d"), input);
            return char16ToChar(t);
        }

        static int strToInt(String input) {
            int t = 0;
            int m = 0;
            int l = input.length();
            if (input.c_str()[0] == '-') {
                l--;
                for (int i = 1; i < input.length(); i++) {
                    t = input.c_str()[i] - '0';
                    l--;
                    int n = 1;
                    for (int j = 0; j < l; j++) n *= 10;
                    m += t * n;
                }
                m = -m;
            } else {
                for (int i = 0; i < input.length(); i++) {
                    t = input.c_str()[i] - '0';
                    l--;
                    int n = 1;
                    for (int j = 0; j < l; j++) n *= 10;
                    m += t * n;
                }
            }
            return m;
        }

        String toLowerCase() {
            char* newStr = _new<char>(size + 1);
            newStr[size] = 0x0;
            for (int i = 0; i < size; i++) {
                newStr[i] = string[i];
            }

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < 26; j++) {
                    if (newStr[i] == j + 'A') newStr[i] = j + 'a';
                }
            }

            return newStr;
        }

        String toUpperCase() {
            char* newStr = _new<char>(size + 1);
            newStr[size] = 0x0;
            for (int i = 0; i < size; i++) {
                newStr[i] = string[i];
            }

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < 26; j++) {
                    if (newStr[i] == j + 'a') newStr[i] = j + 'A';
                }
            }

            return newStr;
        }

        String* split(char c) {
            int amount = find(c);

            int arrIndex = 0;
            int index = 0;
            String* arr = _new<String>(amount + 1);
            char* temp = _new<char>(size + 1);
            temp[size] = 0x0;
            for (int i = 0; i < size; i++) temp[i] = 0x0;

            for (int i = 0; i < size + 1; i++) {
                if (string[i] == c || i == size) {
                    char* finalTmp = _new<char>(index + 1);
                    finalTmp[index] = 0x0;
                    for (int j = 0; j < index; j++) finalTmp[j] = temp[j];
                    arr[arrIndex++] = finalTmp;
                    for (int j = 0; j < size; j++) temp[j] = 0x0;
                    index = 0;
                } else {
                    temp[index++] = string[i];
                }
            }

            return arr;
        }

        int find(char c) {
            int amount = 0;
            for (int i = 0; i < size; i++) {
                if (string[i] == c) amount++;
            }

            return amount;
        }

        long hashCode() {
            long hash = 0;

            if (size == 0) return hash;

            auto func = [](int a, int b) {
                int c = a;
                if (b == 0) return 1;
                for (int i = 0; i < b - 1; i++) c = c * a;
                return c;
            };

            for (int i = 0; i < size; i++) {
                hash += ((int) string[i]) * func(31, size - (i + 1));
            }

            return hash; // hashCode from Java
        }

        int length()        { return size;   }
        char* c_str() const { return string; }

        bool operator==(String& other) {
            return hashCode() == other.hashCode();
        }

        char& operator[](int i) { return string[i]; }

        String operator+(char c) {
            int newLength = length() + 2;
            char* newChar = _new<char>(newLength);
            int newIndex = 0;
            for (int i = 0; i < length(); i++) {
                newChar[newIndex++] = string[i];
            }

            newChar[newIndex++] = c;
            newChar[newIndex]   = 0;
            return newChar;
        }

        String operator+(String& other) {
            int newLength = other.length() + length() + 1;
            char* newChar = _new<char>(newLength);
            int newIndex = 0;
            for (int i = 0; i < other.length(); i++) {
                newChar[newIndex++] = other[i];
            }

            for (int i = 0; i < length(); i++) {
                newChar[newIndex++] = string[i];
            }

            newChar[newIndex] = 0;
            return newChar;
        }

        bool operator!=(String& other) {
            return hashCode() != other.hashCode();
        }

    private:
        int size;
        char* string;
    };
}