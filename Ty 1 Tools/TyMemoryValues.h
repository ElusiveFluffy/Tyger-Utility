#pragma once
#include <memory>
#include <vector>
#include "framework.h"

namespace TyMemoryValues {
	namespace Offsets {
		constexpr int LevelSelectEnable = 0xCA4;
		constexpr int LevelSelectShow = 0xCA6;
	}
	inline DWORD TyBaseAddress;
	void GetBaseAddress();
	std::vector<UINT> GetPointerAddresses(int baseAddress, std::vector<UINT> offsets);
	UINT GetPointerAddress(int baseAddress, UINT offset);

	enum GameState {
		NoWindow = 0, //or just no state
		StartUp = 1,
		Init = 3,
		MainGameInit = 4, //Maybe
		Gameplay = 5,
		TitleScreenOrFMV = 8,
		LoadingScreen = 9,
		ShuttingDown = 12 //Only if quit with the quit button on the main menu
	};
	//Get
	inline GameState GetTyGameState() { return *(GameState*)(TyBaseAddress + 0x288A6C); };
	inline int GetTyFloorID() { return *(int*)(TyBaseAddress + 0x2713C0); };
	inline int GetHiddenOpalCounter() { return *(int*)(TyBaseAddress + 0x2888B4); };

	//Set
	inline void SetLevelSelect(bool state) {
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectEnable) = state;
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectShow) = state;
	};
	inline void SetCameraState(int cameraState) {
		*(int*)(TyBaseAddress + 0x27EBD0) = 28;
		*(int*)(TyBaseAddress + 0x27EBC8) = 28;
	}
	inline int SetHiddenOpalCounter(int amount) { *(int*)(TyBaseAddress + 0x2888B4) = amount; };
};