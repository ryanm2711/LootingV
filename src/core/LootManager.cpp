#include "LootManager.h"
#include "../utility/utility.h"
#include <natives.h>
#include <iostream>
#include <algorithm>
#include <GTA/Controls/Controls.h>
#include <GTA/PickupTypes.h>
#include <GTA/UI/Hud.h>
#include <functional>

#include "LootableBody.h"
#include "LootableVehicle.h"

long nearbyLootableEntitiesQueryTime;
long manageLootedEntitiesStorageTime;

void LootManager::Update() {
	int gameTime = LootingVUtility::Core::GameTimer();

	if (gameTime >= nearbyLootableEntitiesQueryTime) {
		// Prevent player picking up cash from ground (Will miss some due to this running every 500ms. So if player is punching for example, player may pickup cash)
		OBJECT::REMOVE_ALL_PICKUPS_OF_TYPE(GTA::PickupTypes::PICKUP_MONEY_VARIABLE);

		QueryNearbyLootableEntities();
		nearbyLootableEntitiesQueryTime = gameTime + 500;
	}

	if (gameTime >= manageLootedEntitiesStorageTime) {
		ManageLootedEntitiesData();
		manageLootedEntitiesStorageTime = gameTime + 5000;
	}

	ControlLootingLogic();
}

void LootManager::QueryNearbyLootableEntities() {
	if (!LootingVUtility::PlayerEntity::IsPlayerStoryModel()) {
		m_LootableObject = nullptr;
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// Vehicle check
#pragma region Vehicle Check

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
		Vehicle playerCurrentVehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		if (HasEntityBeenLooted(playerCurrentVehicle) || !PED::IS_PED_SITTING_IN_VEHICLE(playerPed, playerCurrentVehicle)) {
			m_LootableObject = nullptr;
			return;
		}

		// Check if the vehicle is moving
		float vehSpeed = ENTITY::GET_ENTITY_SPEED(playerCurrentVehicle);
		if (vehSpeed > 0) {
			m_LootableObject = nullptr;
			return;
		}

		LootTier vehicleLootTier = GetLootTierByVehicleClass(playerCurrentVehicle);

		LootableVehicle lootableVehicle{ playerCurrentVehicle, "Press ~INPUT_CONTEXT~ to loot vehicle.", vehicleLootTier };
		m_LootableObject = std::make_unique<LootableVehicle>(lootableVehicle);
		return;
	}
#pragma endregion
#pragma region Closest Entities Check
	const int ARR_SIZE = 128;

	Ped pedBuffer[ARR_SIZE];
	//Object objectBuffer[ARR_SIZE];
	//int objectCount = worldGetAllObjects(objectBuffer, ARR_SIZE);
	int pedCount = worldGetAllPeds(pedBuffer, ARR_SIZE);

	Ped closestPed = static_cast<Ped>(LootingVUtility::Core::GetClosestEntity(pedBuffer, pedCount, 3.0f));

	Entity closestEntities[] = { closestPed };
	int count = 1;
	Entity closestEntity = LootingVUtility::Core::GetClosestEntity(closestEntities, count, 3.0f);

	// Can't find any objects close enough
	if (closestEntity == -1) {
		m_LootableObject = nullptr;
		return;
	}

	Vector3 entityPos = ENTITY::GET_ENTITY_COORDS(closestEntity, false);
	const int box_size = 1;
	//GRAPHICS::DRAW_BOX(entityPos.x, entityPos.y, entityPos.z, entityPos.x + box_size, entityPos.y + box_size, entityPos.z + box_size, 255, 0, 0, 255);

	if (ENTITY::IS_ENTITY_A_PED(closestEntity) && PED::IS_PED_DEAD_OR_DYING(closestEntity, false) && !PED::IS_PED_SITTING_IN_ANY_VEHICLE(closestEntity) && PED::IS_PED_HUMAN(closestEntity)) {
		if (HasEntityBeenLooted(closestEntity)) {
			m_LootableObject = nullptr;
			return;
		}

		LootTier pedLootTier = GetLootTierByZone(entityPos);

		LootableBody newLootableBody{ closestEntity, "Press ~INPUT_CONTEXT~ to loot body.", pedLootTier };
		m_LootableObject = std::make_unique<LootableBody>(newLootableBody);
	}
#pragma endregion
}

void LootManager::ControlLootingLogic() {
	if (m_LootableObject == nullptr) {
		return;
	}

	GTA::UI::ShowMessage(m_LootableObject->GetHelpMessage());
	GTA::Controls::DisableControlThisFrame(GTA::Controls::PlayerControls, GTA::Controls::VehicleHorn);

	if (GTA::Controls::IsControlJustPressed(2, GTA::Controls::Context)) {
		bool success = GetCurrentLootableObject()->Loot();
		m_LootedObjects.insert(GetCurrentLootableObject()->GetID());

		if (success) {
			AUDIO::PLAY_SOUND_FRONTEND(0, "PICK_UP_MONEY", "HUD_FRONTEND_CUSTOM_SOUNDSET", false);
		}
		else {
			GTA::UI::ShowMessage("Failed to find anything of value.", true, 3000); // 3000 being the ms duration
		}

		// Prevent double looting within same frame
		m_LootableObject = nullptr;
	}
}

void LootManager::ManageLootedEntitiesData() {
	std::erase_if(m_LootedObjects, [](const int& lootedObjectID) {
		if (!ENTITY::DOES_ENTITY_EXIST(lootedObjectID)) {
			std::cout << "Erased entity ID: " << lootedObjectID << " as it no longer exists\n";
			return true;
		}

		return false; // Keep element, still existss
	});
}

bool LootManager::HasEntityBeenLooted(const Entity& entityID) const
{
	if (m_LootedObjects.contains(entityID)) {
		return true;
	}

	return false;
}

LootTier LootManager::GetLootTierByZone(Vector3 coords)
{
	const char* zoneName = ZONE::GET_NAME_OF_ZONE(coords.x, coords.y, coords.z);
	if (zoneName == nullptr) return LootTier::WORKING_CLASS;

	// --- RICH ---
	if (strcmp(zoneName, "GOLF") == 0 ||
		strcmp(zoneName, "RICHM") == 0 ||
		strcmp(zoneName, "PBLUFF") == 0 ||
		strcmp(zoneName, "ROCKF") == 0 ||
		strcmp(zoneName, "VBHILL") == 0 ||
		strcmp(zoneName, "VINE") == 0 ||
		strcmp(zoneName, "ALTA") == 0)
	{
		return LootTier::RICH;
	}

	// --- WORKING CLASS / MIDDLE ---
	if (strcmp(zoneName, "DOWNT") == 0 ||
		strcmp(zoneName, "MIRR") == 0 ||
		strcmp(zoneName, "MORN") == 0 ||
		strcmp(zoneName, "DELPE") == 0 ||
		strcmp(zoneName, "KOREAT") == 0)
	{
		return LootTier::WORKING_CLASS;
	}

	// --- POOR / INDUSTRIAL / RURAL ---
	if (strcmp(zoneName, "RANCHO") == 0 ||
		strcmp(zoneName, "STRAW") == 0 ||
		strcmp(zoneName, "DAVIS") == 0 ||
		strcmp(zoneName, "CHAMH") == 0 ||
		strcmp(zoneName, "CYPRE") == 0 ||
		strcmp(zoneName, "ELYSIAN") == 0 ||
		strcmp(zoneName, "ALAMO") == 0 ||
		strcmp(zoneName, "GRAPES") == 0 ||
		strcmp(zoneName, "DESRT") == 0 ||
		strcmp(zoneName, "PALETO") == 0 ||
		strcmp(zoneName, "SANDY") == 0)
	{
		return LootTier::POOR;
	}

	// Default fallback if zone isn't in the list
	return LootTier::WORKING_CLASS;
}

LootTier LootManager::GetLootTierByVehicleClass(Vehicle vehicle) const
{
	int vehicleClass = VEHICLE::GET_VEHICLE_CLASS(vehicle);
	switch (vehicleClass) {
	// Rich
	case 7: // Super
	case 15: // Helicopters
	case 16: // Planes
	case 2: // SUVs
	case 3: // Coupes
	case 5: // Sports classics
	case 6: // Sports
	case 14: // Boats
	case 18: // Emergency
	case 19: // Military
		return LootTier::RICH;

	case 0: // Compacts
	case 1: // Sedans
	case 4: // Muscle
	case 8: // Motorcycles
	case 9: // Off-road
	case 12: // Vans
		return LootTier::WORKING_CLASS;

	case 10: // Industrial
	case 11: // Utility
	case 13: // Cycles
	case 17: // Service
	case 20: // Commercial
		return LootTier::POOR;

	default:
		return LootTier::WORKING_CLASS;
		break;
	}
}
