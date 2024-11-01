#pragma once
#include "TyMemoryValues.h"
#include <cmath>

class TyPositionRotation {
public:
	typedef struct {
		float* X;
		float* Y;
		float* Z;
	}PtrVector3;
	typedef struct {
		float X;
		float Y;
		float Z;
	}Vector3;
	//Get
	//Ty Pos and Rot
	static PtrVector3 GetTyPosPtr() { return { (float*)(TyMemoryValues::TyBaseAddress + 0x270B78), (float*)(TyMemoryValues::TyBaseAddress + 0x270B7C), (float*)(TyMemoryValues::TyBaseAddress + 0x270B80) }; };
	static Vector3 GetTyPos() { return { *(float*)(TyMemoryValues::TyBaseAddress + 0x270B78), *(float*)(TyMemoryValues::TyBaseAddress + 0x270B7C), *(float*)(TyMemoryValues::TyBaseAddress + 0x270B80) }; };
	static float GetTyRot() { return *(float*)(TyMemoryValues::TyBaseAddress + 0x271C20); };

	//Bull Pos and Rot
	static PtrVector3 GetBullPosPtr() { return { (float*)(TyMemoryValues::TyBaseAddress + 0x254268), (float*)(TyMemoryValues::TyBaseAddress + 0x25426C), (float*)(TyMemoryValues::TyBaseAddress + 0x254270) }; };
	static Vector3 GetBullPos() { return { *(float*)(TyMemoryValues::TyBaseAddress + 0x254268), *(float*)(TyMemoryValues::TyBaseAddress + 0x25426C), *(float*)(TyMemoryValues::TyBaseAddress + 0x254270) }; };
	static float GetBullRot() { 
		float bullRot = *(float*)(TyMemoryValues::TyBaseAddress + 0x2545F4);
		return std::abs((float)((int)(bullRot * 1000) % 6282) / 1000);
	};

	//Set
	static void SetTyPos(Vector3 position) {
		PtrVector3 tyPos = GetTyPosPtr();
		*tyPos.X = position.X;
		*tyPos.Y = position.Y;
		*tyPos.Z = position.Z;
	};

	static void SetBullPos(Vector3 position) {
		PtrVector3 bullPos = GetBullPosPtr();
		*bullPos.X = position.X;
		*bullPos.Y = position.Y;
		*bullPos.Z = position.Z;
	};
};