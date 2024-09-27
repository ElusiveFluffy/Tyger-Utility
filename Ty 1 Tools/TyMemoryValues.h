#pragma once
#include <memory>
#include <vector>
#include "framework.h"

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
class TyMemoryValues {
public:
	typedef struct {
		float X;
		float Y;
		float Z;
	}Vector3;
	//Get
	static GameState GetTyGameState() { return *(GameState*)(TyBaseAddress + 0x288A6C); };
	static Vector3 GetTyPos() { return { *(float*)(TyBaseAddress + 0x270B78), *(float*)(TyBaseAddress + 0x270B7C), *(float*)(TyBaseAddress + 0x270B80) }; };
	static float GetTyRot() { return *(float*)(TyBaseAddress + 0x271C20); };

	//Set
	static void SetTyPos(Vector3 position) {
		*(float*)(TyBaseAddress + 0x270B78) = position.X;
		*(float*)(TyBaseAddress + 0x270B7C) = position.Y;
		*(float*)(TyBaseAddress + 0x270B80) = position.Z;
	};

	static inline DWORD TyBaseAddress;
	static void GetBaseAddress();
	static std::vector<UINT> GetPointerAddresses(int baseAddress, std::vector<UINT> offsets);
	static UINT GetPointerAddress(int baseAddress, UINT offset);
};