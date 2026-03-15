#include <types.h>
#include "IBaseLootable.h"
#include <functional>
#include <string>

#pragma once
class LootableBody: public IBaseLootable {
public:
	LootableBody(Ped pedID, std::string helpMsg, LootTier tier) : IBaseLootable(pedID, helpMsg, tier) {};

	bool operator==(const LootableBody& other) const {
		return GetID() == other.GetID();
	}

	bool Loot() override;
};

namespace std {
	template<>
	struct hash<LootableBody> {
		size_t operator()(const LootableBody& body) const noexcept {
			return std::hash<Ped>()(body.GetID());
;		}
	};
}