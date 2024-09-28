#pragma once
#include "TyMemoryValues.h"
#include <cmath>

class TyPositionRotation {
public:
	typedef struct {
		float X;
		float Y;
		float Z;
	}Vector3;
	//Get
	//Ty Pos and Rot
	static Vector3 GetTyPos() { return { *(float*)(TyMemoryValues::TyBaseAddress + 0x270B78), *(float*)(TyMemoryValues::TyBaseAddress + 0x270B7C), *(float*)(TyMemoryValues::TyBaseAddress + 0x270B80) }; };
	static float GetTyRot() { return *(float*)(TyMemoryValues::TyBaseAddress + 0x271C20); };

	//Bull Pos and Rot
	static Vector3 GetBullPos() { return { *(float*)(TyMemoryValues::TyBaseAddress + 0x254268), *(float*)(TyMemoryValues::TyBaseAddress + 0x25426C), *(float*)(TyMemoryValues::TyBaseAddress + 0x254270) }; };
	static float GetBullRot() { 
		float bullRot = *(float*)(TyMemoryValues::TyBaseAddress + 0x2545F4);
		return std::abs((float)((int)(bullRot * 1000) % 6282) / 1000);
	};

	//Set
	static void SetTyPos(Vector3 position) {
		*(float*)(TyMemoryValues::TyBaseAddress + 0x270B78) = position.X;
		*(float*)(TyMemoryValues::TyBaseAddress + 0x270B7C) = position.Y;
		*(float*)(TyMemoryValues::TyBaseAddress + 0x270B80) = position.Z;
	};
};