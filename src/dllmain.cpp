#include "core/Main.h"
#include <Utility/Console.h>

static LootingV lootingV;

#ifdef _DEBUG
	static bool g_EnableDebugConsole = true;
#else
	static bool g_EnableDebugConsole = false;
#endif



void ScriptMain() {
	if (g_EnableDebugConsole) {
		Utility::CreateDebugConsole();
	}

	lootingV.Init();
	while (true) {
		lootingV.Main();
		WAIT(0);
	}
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		break;
	case DLL_PROCESS_DETACH:
		if (g_EnableDebugConsole) {
			Utility::CloseDebugConsole();
		}

		lootingV.OnShutdown();
		scriptUnregister(hInstance);
		break;
	}

	return TRUE;
}