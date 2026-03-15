#include "LootableVehicle.h"
#include "../utility/utility.h"
#include <natives.h>
#include <iostream>

bool LootableVehicle::Loot()
{
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
		return false; // Vehicle had nothing of worth, return false.
	}

	unsigned int amount = minMoney + (rand() % (maxMoney - minMoney + 1));
	LootingVUtility::PlayerEntity::AddPlayerMoney(amount);
	return true;
}
