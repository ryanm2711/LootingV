#include "LootableBody.h"
#include "../utility/utility.h"
#include <natives.h>
#include <iostream>
#include <cstdlib>

bool LootableBody::Loot()
{
	// Play animations
	static const char* ANIM_DICT = "missarmenian3_gardener";
	static const char* ANIM_NAME = "idle_a";
	//STREAMING::REQUEST_ANIM_DICT(ANIM_DICT);
	static unsigned int const ANIM_FLAG = 0;
	//TASK::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), ANIM_DICT, ANIM_NAME, 4.0f, 0.0f, 1000, 0, 4.0f, true, true, true);

	srand(GetTickCount());

	int minMoney{ 0 };
	int maxMoney{ 0 };
	int lootChance{ 100 }; // Chance of any loot

	switch (GetTier()) {
	case LootTier::POOR:
		minMoney = 1;
		maxMoney = 20;
		lootChance = 95;
		std::cout << "Loot tier: POOR\n";
		break;
	case LootTier::WORKING_CLASS:
		minMoney = 20;
		maxMoney = 150;
		lootChance = 92;
		std::cout << "Loot tier: WORKING CLASS\n";
		break;
	case LootTier::RICH:
		minMoney = 100;
		maxMoney = 1000;
		lootChance = 80;
		std::cout << "Loot tier: RICH\n";
		break;
	}

	int chance{ rand() % 100 };
	std::cout << "CHANCE: " << chance << "%\n";
	if (chance > lootChance) {
		return false; // Body had nothing of worth, return false.
	}

	unsigned int amount = minMoney + (rand() % (maxMoney - minMoney + 1));
	LootingVUtility::PlayerEntity::AddPlayerMoney(amount);
	return true;
};