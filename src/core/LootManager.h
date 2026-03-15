#pragma once
#include <memory>
#include <unordered_set>
#include "IBaseLootable.h"

class LootManager {
public:
	void Update();

	inline IBaseLootable* GetCurrentLootableObject() const {
		return m_LootableObject.get();
	};

	bool HasEntityBeenLooted(const Entity& entityID) const;

	LootTier GetLootTierByZone(Vector3 coords);
	LootTier GetLootTierByVehicleClass(Vehicle vehicle) const;
private:
	void ControlLootingLogic();
	void QueryNearbyLootableEntities();
	void ManageLootedEntitiesData();

	std::unique_ptr<IBaseLootable> m_LootableObject;
	std::unordered_set<int> m_LootedObjects;
};