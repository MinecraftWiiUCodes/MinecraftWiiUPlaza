#pragma once

#include <inttypes.h>
#include <cstring>
#include <cmath>

#define WIDTH  640
#define HEIGHT 360

#define M_PI   3.14159265358979323846264338327950288419716939937510 

const char ABC[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

template<typename... Args>
static void swprintf(const char16_t* outputPointer, uint32_t size, const char16_t* inputString, Args... args) {
	code::Func<void, 0x382C8C0, const char16_t*, uint32_t, const char16_t*, Args...>()(outputPointer, size, inputString, args...);
}

// Short Array Allocation
template<typename T>
T* _new(uint32_t size) {
    return code::Func<T*, 0x0382AACC, uint32_t>()(sizeof(T) * size);
}

// Short Allocation
template<typename T>
T* _new1(T t) {
	T* allocDest = _new<T>(1);
	memcpy(allocDest, &t, sizeof(T));
	return allocDest;
}

// Permanent Allocation
uint32_t allocPointer = 0x35000000;
template<typename T>
T* _new2(T t) {
	memcpy((void*) allocPointer, &t, sizeof(T));
	T* ret = (T*) allocPointer;
	allocPointer += sizeof(T);
	return ret;
}

// Permanent Array Allocation
template<typename T>
T* _new3(uint32_t size) {
	T* ret = (T*) allocPointer;
	allocPointer += sizeof(T) * size;
	return ret;
}

char* char16ToChar(const char16_t* str, int length = 0) {
	bool found = length != 0;
	while (!found) {
		if (str[length++] == 0x0) {
			found = true;
			length--;
		}
		if (length >= 250) return "";
	}

	char* newStr = _new<char>(length + 1);
	for (int i = 0; i < length; i++) {
		newStr[i] = str[i];
	}

	newStr[length] = 0;
	return newStr;
}

char16_t* charToChar16(const char* str, int length = 0) {
	bool found = length != 0;
	while (!found) {
		if (str[length++] == 0x0) {
			found = true;
			length--;
		}
		if (length >= 250) return u"";
	}

	char16_t* newStr = _new<char16_t>(length + 1);
	for (int i = 0; i < length; i++) {
		newStr[i] = str[i];
	}

	newStr[length] = 0;
	return newStr;
}

// https://stackoverflow.com/questions/2249110/how-do-i-make-a-portable-isnan-isinf-function
int isinf(double x) {
    union { uint64_t u; double f; } ieee754;
    ieee754.f = x;
    return ( (unsigned)(ieee754.u >> 32) & 0x7fffffff ) == 0x7ff00000 &&
           ( (unsigned)ieee754.u == 0 );
}

// https://stackoverflow.com/questions/2249110/how-do-i-make-a-portable-isnan-isinf-function
int isnan(double x) {
    union { uint64_t u; double f; } ieee754;
    ieee754.f = x;
    return ( (unsigned)(ieee754.u >> 32) & 0x7fffffff ) +
           ( (unsigned)ieee754.u != 0 ) > 0x7ff00000;
}

float abs(float input) {
	bool negative = input < 0.0f;
	if (negative) input *= -1.0f; 

	if (negative) {
		input -= 0x80000000;
        input  = 0x80000000 - input;
	}

	return input;
}

int toInt(float input) {
    bool negative = input < 0.0f;
	if (negative) input *= -1.0f; 

	uint32_t output = static_cast<uint32_t>(input);
	if (negative) {
		output -= 0x80000000;
        output  = 0x80000000 - output;
		output -= 1;
	}
	
	return output;
}

float toFloat(int input) {
    bool negative = (input & 0x80000000) == 0x80000000;
    unsigned int output_tmp = input;
    if (negative) {
        output_tmp -= 0x80000000;
        output_tmp  = 0x80000000 - output_tmp;
    }
    
    float output = static_cast<float>(output_tmp);
    if (negative) {
        uint32_t addr = 0x03FFFFF4;
        *(float*) addr = output;
        uint32_t output_hex = code::Mem(addr).as<uint32_t>();
        output_hex = output_hex + 0x80000000;
        output = *(float *) &output_hex;
    }

    return output;
}

char16_t* getStr(mstd::wstring& wstr) {
	return (wstr.trueLength > 7) ? wstr.pointer : wstr.str;
}

char* toCStr(float input, int decimalPlaces = 5) {
	uint32_t a0 = *(uint32_t*) &input;
    bool isNegative = (a0 & 0x80000000) == 0x80000000;
    if (isNegative) a0 -= 0x80000000;
    float a1 = *(float*) &a0;
	uint32_t multip = 1;
	for (int i = 0; i < decimalPlaces; i++) multip *= 10;
    uint32_t a = static_cast<uint32_t>(a1 * toFloat(multip));
    uint32_t a_dec = a % multip;
    a = (a - a_dec) / multip;
    char16_t* strTemp = _new<char16_t>(0x20);
    if (isnan(input)) {
        swprintf(strTemp, 0x20, u"NaN");
	} else if (isinf(input)) {
		swprintf(strTemp, 0x20, u"Inf"); 
	} else if (isNegative) {
		char16_t* strTemp2 = _new<char16_t>(0x20);
		swprintf(strTemp2, 0x20, u"-%d.%s0%dd", a, "%", decimalPlaces);
        swprintf(strTemp, 0x20, strTemp2, a_dec);
    } else {
		char16_t* strTemp2 = _new<char16_t>(0x20);
		swprintf(strTemp2, 0x20, u"%d.%s0%dd", a, "%", decimalPlaces);
        swprintf(strTemp, 0x20, strTemp2, a_dec);
    }
    return char16ToChar(strTemp, 0x20);
}

template<typename T>
T* sort(T* array, int length, bool(*f)(T&, T&)) {
	T* list = _new<T>(length);
	for (int i = 0; i < length; i++) {
		list[i] = array[i];
	}
	
	T temp;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			if (f(list[i], list[j])) {
				temp    = list[i];
    	        list[i] = list[j];
    	        list[j] = temp;
			}
		}
	}
	
	return list;
}

class UUID {
public:
	uint32_t part1;
	uint32_t part2;
	uint32_t part3;
	uint32_t part4;
};

namespace mboost {
	template<typename T>
	class shared_ptr {
	public:
		shared_ptr(T* p) {
			ptr = p;
	    	cnt = 0;
	  	}
	
		shared_ptr() {
	    	ptr = nullptr;
	    	cnt = 0;
	  	}
		
		T* operator->() const { return ptr; }

	  	T* ptr;
	  	uint32_t cnt;
	};
}

struct vector {
	uint32_t field_0x0;
  	uint32_t field_0x4;
  	uint32_t field_0x8;
  	uint32_t field_0xc;
};

enum Button {
	LEFT_STICK_DOWN  = 0x08000000, LEFT_STICK_LEFT  = 0x40000000, LEFT_STICK_RIGHT = 0x20000000, LEFT_STICK_UP    = 0x10000000,
	A                = 0x8000,     B                = 0x4000,     X                = 0x2000,     Y                = 0x1000,
	DPAD_LEFT        = 0x800,      DPAD_RIGHT       = 0x400,      DPAD_UP          = 0x200,      DPAD_DOWN        = 0x100, 
	ZL               = 0x80,       ZR               = 0x40,       L                = 0x20,       R                = 0x10,
	PLUS             = 0x8,        MINUS            = 0x4,
};

static uint32_t getButton() {
	uint32_t button = code::Mem(0x1058C16C).as<uint32_t>();
	return button;
}

float min(float a, float b) {
	return (a > b) ? b : a;
}

float max(float a, float b) {
	return (a > b) ? a : b;
}

enum CraftItems {
	DIAMOND_SWORD = 0x19C, BOW = 0x170, E_GOLDEN_APPLE = 0x2A, DIAMOND_HELMET = 0xAC, 
	DIAMOND_CHESTPLATE = 0xAD, DIAMOND_LEGGINGS = 0xAE, DIAMOND_BOOTS = 0xAF, STRENGTH_2_ARROW = 0x197,
};

enum Items {
	AIR = 0, STONE = 1, DIRT = 3, LOG = 17, LEAVES = 18, BARRIER = 166, BOW_ID = 261, 
	TOTEM_OF_UNDYING = 449, WOODEN_AXE = 271, STONE_AXE = 275, STONE_SHOVEL = 273, 
	STONE_HOE = 291, DIAMOND_AXE = 279, GOLDEN_HOE = 294, STONE_PICKAXE = 274, IRON_HOE = 292,
	IRON_SWORD = 267, DIAMOND_SWORD_ID = 276, GOLDEN_SHOVEL = 284, DIAMOND_PICKAXE = 278,
};

enum InventorySlotId {
	CRAFTING_OUTPUT = 0, 
	CRAFTING_SLOT_ONE = 1, CRAFTING_SLOT_TWO = 2, CRAFTING_SLOT_THREE = 3, CRAFTING_SLOT_FOURTH = 4,
	HEAD = 5, CHESTPLATE = 6, LEGGINGS = 7, BOOTS = 8,
	FIRST_SLOT = 36, SECOND_SLOT = 37, THIRD_SLOT = 38, FOURTH_SLOT = 39, FIFTH_SLOT = 40,
	SIXTH_SLOT = 41, SEVENTH_SLOT = 42, EIGHTH_SLOT = 43, NINTH_SLOT = 44, OFFHAND_SLOT = 45
};