#pragma once
#include <unordered_map>
#include <types.h>
#include <functional>

/*struct Vector3Hasher {
	size_t operator()(const Vector3& vec) const {
		std::hash<float> hasher;
		size_t hash = 0;

		auto combine = [&](float val) {
			hash ^= hasher(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		};

		combine(vec.x);
		combine(vec.y);
		combine(vec.z);

		return hash;
	}
};*/

class BlipManager {
public:
	void Update(std::function<bool(Entity)> lootedCondition);

	Blip AddBlip(Entity entity, int blipSprite);
	void RemoveBlip(Entity entity);
	void ClearAllBlips();

	inline std::unordered_map<Entity, Blip> GetBlips() const {
		return m_Blips;
	}

	inline bool DoesEntityBlipExist(Entity entity) const {
		return m_Blips.contains(entity);
	}
private:
	void RegisterValidEntitiesForBlips(std::function<bool(Entity)> addCondition);
	void CheckInOnRegisteredBlips(std::function<bool(Entity)> removeCondition);
	std::unordered_map<Entity, Blip> m_Blips;
};