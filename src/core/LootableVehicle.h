#include <types.h>
#include "IBaseLootable.h"
#include <functional>

#pragma once
class LootableVehicle : public IBaseLootable {
public:
	LootableVehicle(Vehicle vehicleID, std::string helpMsg, LootTier tier) : IBaseLootable(vehicleID, helpMsg, tier) {};

	bool operator==(const LootableVehicle& other) const {
		return GetID() == other.GetID();
	}

	bool Loot() override;
};

namespace std {
	template<>
	struct hash<LootableVehicle> {
		size_t operator()(const LootableVehicle& vehicle) const noexcept {
			return std::hash<Ped>()(vehicle.GetID());
		};
	};
}