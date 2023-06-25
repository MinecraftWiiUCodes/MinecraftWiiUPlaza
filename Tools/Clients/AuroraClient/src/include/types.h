#pragma once

#include <code/code.h>

namespace mc {
	enum Button {
		A         = 0x8000, B          = 0x4000, X       = 0x2000, Y         = 0x1000,
		DPAD_LEFT = 0x800,  DPAD_RIGHT = 0x400,  DPAD_UP = 0x200,  DPAD_DOWN = 0x100, 
		ZL        = 0x80,   ZR         = 0x40,   L       = 0x20,   R         = 0x10,
		PLUS      = 0x8,    MINUS      = 0x4,
	};

	static uint32_t getButton() {
		return code::Mem(0x1058C16C).as<uint32_t>();
	}

	enum InventorySlotId {
		CRAFTING_OUTPUT = 0, 
		CRAFTING_SLOT_ONE = 1, CRAFTING_SLOT_TWO = 2, CRAFTING_SLOT_THREE = 3, CRAFTING_SLOT_FOURTH = 4,
		HEAD = 5, CHESTPLATE = 6, LEGGINGS = 7, BOOTS = 8,
		FIRST_SLOT = 36, SECOND_SLOT = 37, THIRD_SLOT = 38, FOURTH_SLOT = 39, FIFTH_SLOT = 40,
		SIXTH_SLOT = 41, SEVENTH_SLOT = 42, EIGHTH_SLOT = 43, NINTH_SLOT = 44, OFFHAND_SLOT = 45
	};

	enum Items {
		AIR = 0, STONE = 1, DIRT = 3, LOG = 17, LEAVES = 18, BARRIER = 166, BOW_ID = 261, 
		TOTEM_OF_UNDYING = 449, WOODEN_AXE = 271, STONE_AXE = 275, STONE_SHOVEL = 273, 
		STONE_HOE = 291, DIAMOND_AXE = 279, GOLDEN_HOE = 294, STONE_PICKAXE = 274, IRON_HOE = 292,
		IRON_SWORD = 267, DIAMOND_SWORD_ID = 276,
	};

	enum CraftItems {
		DIAMOND_SWORD = 0x19C, BOW = 0x170, E_GOLDEN_APPLE = 0x2A, DIAMOND_HELMET = 0xAC, 
		DIAMOND_CHESTPLATE = 0xAD, DIAMOND_LEGGINGS = 0xAE, DIAMOND_BOOTS = 0xAF, STRENGTH_2_ARROW = 0x197,
	};

	enum Difficulty {
		PEACEFUL = 0, EASY = 1, NORMAL = 2, HARD = 3,
	};

	enum GameMode {
		SURVIVAL = 0, CREATIVE = 1, ADVENTURE = 2, SPECTATOR = 3, LOBBY = 4,
	};

	enum Weather {
		CLEAR = 0, RAINY = 1, THUNDER = 2,
	};
}