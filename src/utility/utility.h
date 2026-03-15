#pragma once

#include <types.h>
#include <string>
#include <vector>

namespace LootingVUtility {
	namespace UI {
		int ShowNotification(const char* text);
		void ShowSubtitle(const char* text, int duration = 4000);
		void ShowMessage(const char* text, bool emitSound = true);
	}

	namespace Core {
		std::string FormatVector(Vector3 vec3);
		Entity GetClosestEntity(const Entity* entities, int& count, float maxDistance=10.0f);
		int GameTimer();
	}

	namespace PlayerEntity {
		int GetPlayerMoney();
		void AddPlayerMoney(const unsigned int &amount);

		bool IsPlayerStoryModel();
	}
}