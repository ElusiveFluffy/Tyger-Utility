#pragma once
#include <memory>
#include <vector>
#include "framework.h"

class TyMemoryValues {
public:
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
	static GameState GetTyGameState() { return *(GameState*)(TyBaseAddress + 0x288A6C); };
	static int GetTyFloorID() { return *(int*)(TyBaseAddress + 0x2713C0); };
	static int GetHiddenOpalCounter() { return *(int*)(TyBaseAddress + 0x2888B4); };

	//Set
	static void SetLevelSelect(bool state) {
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectEnable) = state;
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectShow) = state;
	};
	static void SetCameraState(int cameraState) {
		*(int*)(TyBaseAddress + 0x27EBD0) = 28;
		*(int*)(TyBaseAddress + 0x27EBC8) = 28;
	}
	static int SetHiddenOpalCounter(int amount) { *(int*)(TyBaseAddress + 0x2888B4) = amount; };

	static inline DWORD TyBaseAddress;
	static void GetBaseAddress();
	static std::vector<UINT> GetPointerAddresses(int baseAddress, std::vector<UINT> offsets);
	static UINT GetPointerAddress(int baseAddress, UINT offset);

private:
	class Offsets {
	public:
		static constexpr int LevelSelectEnable = 0xCA4;
		static constexpr int LevelSelectShow = 0xCA6;
	};
};