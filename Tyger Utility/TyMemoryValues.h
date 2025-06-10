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

	typedef int(__thiscall* TySetGotDoubleHealth_t)(void* gameData, bool newState);

	inline bool* DrawObjectBounds() { return (bool*)(TyBaseAddress + 0x293b0c); };

	inline void SetGotDoubleHealth(bool newState) {
		((TySetGotDoubleHealth_t)(TyBaseAddress + 0xf7ec0))((void*)(TyBaseAddress + 0x2888ac), newState);
	}

	inline void DisableLeaderboard() { 
		BYTE nopInstructions[]{ 0x90, 0x90, 0x90, 0x90, 0x90 };
		BYTE* functionAddress = (BYTE*)(TyBaseAddress + 0x1C8D6D);

		DWORD oldProtection;
		//Change the memory access to ReadWrite to be able to change the hardcoded value (usually its read only)
		VirtualProtect(functionAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtection);

		std::copy(std::begin(nopInstructions), std::end(nopInstructions), functionAddress);

		//Set it back to the old access protection
		VirtualProtect(functionAddress, 5, oldProtection, &oldProtection);
	}

	//Set
	inline void SetLevelSelect(bool state) {
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectEnable) = state;
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectShow) = state;
	};
	inline int SetHiddenOpalCounter(int amount) { *(int*)(TyBaseAddress + 0x2888B4) = amount; };
};