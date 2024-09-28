#pragma once
#include <memory>
#include <vector>
#include "framework.h"

class TyMemoryValues {
public:
	enum GameState {
		NoWindow = 0, //Maybe
		StartUp = 1,
		Unknown = 2,
		Init = 3,
		Unknown2 = 4,
		Gameplay = 5,
		Unknown3 = 6,
		Unknown4 = 7,
		TitleScreen = 8,
		LoadingScreen = 9
	};

	typedef struct {
		float X;
		float Y;
		float Z;
	}Vector3;
	//Get
	static GameState GetTyGameState() { return *(GameState*)(TyBaseAddress + 0x288A6C); };
	static int GetTyFloorID() { return *(int*)(TyBaseAddress + 0x2713C0); };
	static Vector3 GetTyPos() { return { *(float*)(TyBaseAddress + 0x270B78), *(float*)(TyBaseAddress + 0x270B7C), *(float*)(TyBaseAddress + 0x270B80) }; };
	static float GetTyRot() { return *(float*)(TyBaseAddress + 0x271C20); };
	static int GetHiddenOpalCounter() { return *(int*)(TyBaseAddress + 0x2888B4); };

	//Set
	static void SetTyPos(Vector3 position) {
		*(float*)(TyBaseAddress + 0x270B78) = position.X;
		*(float*)(TyBaseAddress + 0x270B7C) = position.Y;
		*(float*)(TyBaseAddress + 0x270B80) = position.Z;
	};
	static void SetLevelSelect(bool state) {
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectEnable) = state;
		*(bool*)GetPointerAddress((TyBaseAddress + 0x00286CB0), Offsets::LevelSelectShow) = state;
	};
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