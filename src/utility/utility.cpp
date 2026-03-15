#include "utility.h"
#include <GTA/Entities/Peds/PedHash.h>
#include <natives.h>
#include <iostream>

int LootingVUtility::UI::ShowNotification(const char* text) {
	HUD::BEGIN_TEXT_COMMAND_THEFEED_POST((char*)"STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	return HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(1, 1);
}

void LootingVUtility::UI::ShowSubtitle(const char* text, int duration) {
	HUD::BEGIN_TEXT_COMMAND_PRINT((char*)"STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	HUD::END_TEXT_COMMAND_PRINT(duration, true);
}

void LootingVUtility::UI::ShowMessage(const char* text, bool emitSound)
{
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP((char*)"STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, emitSound, -1);
}

std::string LootingVUtility::Core::FormatVector(Vector3 vec3)
{
	std::string str = "Vector(X:";
	str = str + std::to_string(vec3.x) + ", Y: ";
	str = str + std::to_string(vec3.y) + ", Z: ";
	str = str + std::to_string(vec3.z) + ")";

	return str;
}

// Finds the closest entity to the player within the specified max distance. If no entity is found, return -1
Entity LootingVUtility::Core::GetClosestEntity(const Entity* entities, int &count, float maxDistance) {
	Entity closestEntity = -1;
	float closestDistance = FLT_MAX;

	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);

	for (int i = 0; i < count; i++)
	{
		Entity entity = entities[i];

		//std::cout << "Entity: " << entity << "\n";
		if (!ENTITY::DOES_ENTITY_EXIST(entity) || entity == PLAYER::PLAYER_PED_ID()) {
			continue;
		}

		Vector3 entityPos = ENTITY::GET_ENTITY_COORDS(entity, false);
		float distanceToPlayer = playerPos.DistanceTo(entityPos);

		//std::cout << "Entity " << entity << " distance to player : " << distanceToPlayer << " Stored close dist : " << closestDistance << "\n";

		// Found an entity that is closer, store that instead.
		if (distanceToPlayer < closestDistance && distanceToPlayer <= maxDistance) {
			closestEntity = entity;
			closestDistance = distanceToPlayer;
		}
	}

	return closestEntity;
}

int LootingVUtility::Core::GameTimer()
{
	return MISC::GET_GAME_TIMER();
}

int LootingVUtility::PlayerEntity::GetPlayerMoney()
{
	Ped playerPedID = PLAYER::PLAYER_PED_ID();
	Hash model = ENTITY::GET_ENTITY_MODEL(playerPedID);

	static const Hash MichaelModel = MISC::GET_HASH_KEY("player_zero");
	static const Hash FranklinModel = MISC::GET_HASH_KEY("player_one");
	static const Hash TrevorModel = MISC::GET_HASH_KEY("player_two");
	
	Hash moneyHash{};
	if (model == MichaelModel) {
		moneyHash = MISC::GET_HASH_KEY("SP0_TOTAL_CASH");
	}
	else if (model == FranklinModel) {
		moneyHash = MISC::GET_HASH_KEY("SP1_TOTAL_CASH");
	}
	else if (model == TrevorModel) {
		moneyHash = MISC::GET_HASH_KEY("SP2_TOTAL_CASH");
	}

	int money = 0;
	STATS::STAT_GET_INT(moneyHash, &money, -1);
	return money;
}

void LootingVUtility::PlayerEntity::AddPlayerMoney(const unsigned int &amount)
{
	Ped playerPedID = PLAYER::PLAYER_PED_ID();
	int currentMoney = GetPlayerMoney();

	Hash model = ENTITY::GET_ENTITY_MODEL(playerPedID);

	static const Hash MichaelModel = MISC::GET_HASH_KEY("player_zero");
	static const Hash FranklinModel = MISC::GET_HASH_KEY("player_one");
	static const Hash TrevorModel = MISC::GET_HASH_KEY("player_two");

	Hash moneyHash{};
	if (model == MichaelModel) {
		moneyHash = MISC::GET_HASH_KEY("SP0_TOTAL_CASH");
	}
	else if (model == FranklinModel) {
		moneyHash = MISC::GET_HASH_KEY("SP1_TOTAL_CASH");
	}
	else if (model == TrevorModel) {
		moneyHash = MISC::GET_HASH_KEY("SP2_TOTAL_CASH");
	}

	STATS::STAT_SET_INT(moneyHash, currentMoney + amount, true);
}

bool LootingVUtility::PlayerEntity::IsPlayerStoryModel()
{
	Ped playerPedID = PLAYER::PLAYER_PED_ID();
	Hash model = ENTITY::GET_ENTITY_MODEL(playerPedID);


	switch (model) {
	case GTA::Peds::Franklin:
	case GTA::Peds::Trevor:
	case GTA::Peds::Michael:
		return true;
	}
	
	return false;
}
