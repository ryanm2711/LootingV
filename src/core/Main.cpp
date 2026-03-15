#include "main.h"
#include "../utility/utility.h"
#include <iostream>
#include <GTA/PickupTypes.h>

void LootingV::Init() {
	std::cout << "Initialize script!\n";
	m_LootManager = LootManager{};
	m_BlipManager = BlipManager{};
}

void LootingV::Main() {
	m_LootManager.Update();
	m_BlipManager.Update([this](Entity e) {
		return m_LootManager.HasEntityBeenLooted(e);
	});
}

void LootingV::OnShutdown()
{
	m_BlipManager.ClearAllBlips();
}
