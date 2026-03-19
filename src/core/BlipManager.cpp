#include "BlipManager.h"
#include "../utility/utility.h"
#include <natives.h>
#include <iostream>

long blipQueryUpdateTime;
long checkOnAddedBlipsTime;
void BlipManager::Update(std::function<bool(Entity)> lootedCondition) {
	int gameTime = LootingVUtility::Core::GameTimer();

	if (gameTime >= blipQueryUpdateTime) {
		RegisterValidEntitiesForBlips(lootedCondition);
		blipQueryUpdateTime = gameTime + 2000;
	}
	
	if (gameTime >= checkOnAddedBlipsTime) {
		CheckInOnRegisteredBlips(lootedCondition);
		checkOnAddedBlipsTime = gameTime + 5000;
	}
}

Blip BlipManager::AddBlip(Entity entity, int blipSprite)
{
	Blip blip = HUD::ADD_BLIP_FOR_ENTITY(entity);
	HUD::SET_BLIP_SPRITE(blip, blipSprite);

	m_Blips.insert({entity, blip});
	return blip;
}

void BlipManager::RemoveBlip(Entity entity)
{
	//std::cout << "BLIP COORDS: " << LootingVUtility::Core::FormatVector(blipPos) << "\n";
	auto it = m_Blips.find(entity);
	if (it != m_Blips.end()) {
		Blip blip = it->second;
		//std::cout << "BLIP EXISTS IN LIST\n";
		HUD::SET_BLIP_ALPHA(blip, 0); // REMOVE_BLIP doesn't remove it right away, so cheat by setting alpha first.
		HUD::REMOVE_BLIP(&blip);
		m_Blips.erase(it);
	}
}

void BlipManager::ClearAllBlips()
{
	for (auto &i : GetBlips()) {
		Entity entity = i.first;

		RemoveBlip(entity);
	}
}

void BlipManager::RegisterValidEntitiesForBlips(std::function<bool(Entity)> addCondition)
{
	const int ARR_SIZE = 128;
	static int DEAD_PED_BLIP_SPRITE = 274;

	Ped pedBuffer[ARR_SIZE];
	int pedCount = worldGetAllPeds(pedBuffer, ARR_SIZE);

	for (int i = 0; i < pedCount; i++) {
		Ped ped = pedBuffer[i];

		if (!ENTITY::DOES_ENTITY_EXIST(ped) || !PED::IS_PED_DEAD_OR_DYING(ped, false) || ped == PLAYER::PLAYER_PED_ID()) {
			continue;
		}

		bool shouldAdd;
		if (addCondition) {
			shouldAdd = !addCondition(ped); // Invert it because if ped has been looted, then shouldAdd would be false
		}

		if (!DoesEntityBlipExist(ped) && shouldAdd) {
			AddBlip(ped, DEAD_PED_BLIP_SPRITE);
		}
	}
}

void BlipManager::CheckInOnRegisteredBlips(std::function<bool(Entity)> removeCondition)
{
	for (auto &i : GetBlips()) {
		Entity entity = i.first;
		Blip blip = i.second;

		bool shouldRemove = !ENTITY::DOES_ENTITY_EXIST(entity);

		if (!shouldRemove && removeCondition) {
			shouldRemove = removeCondition(entity);
		}

		//std::cout << "Blip Pos: " << LootingVUtility::Core::FormatVector(blipPos) << "Blip ID: " << blip << "Distance: " << playerPos.DistanceTo(blipPos) << "\n";

		if (shouldRemove) {
			//std::cout << "Remove blip " << blip << "\n";
			RemoveBlip(entity);
		}
	}
}