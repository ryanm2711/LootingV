#include <main.h>
#include <natives.h>
#include "LootManager.h"
#include "BlipManager.h"

#pragma once
class LootingV {
public:
	void Init();
	void Main();
	void OnShutdown();
private:
	LootManager m_LootManager;
	BlipManager m_BlipManager;
};