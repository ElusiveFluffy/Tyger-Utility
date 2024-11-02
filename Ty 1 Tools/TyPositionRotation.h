#pragma once
#include "TyMemoryValues.h"
#include <cmath>

using namespace TyMemoryValues;

namespace TyPositionRotation {
	struct PtrVector3 {
		float* X;
		float* Y;
		float* Z;
	};
	struct Vector3 {
		float X;
		float Y;
		float Z;
	};
	//Get
	//Ty Pos and Rot
	inline PtrVector3 GetTyPosPtr() { return { (float*)(TyBaseAddress + 0x270B78), (float*)(TyBaseAddress + 0x270B7C), (float*)(TyBaseAddress + 0x270B80) }; };
	inline Vector3 GetTyPos() { return { *(float*)(TyBaseAddress + 0x270B78), *(float*)(TyBaseAddress + 0x270B7C), *(float*)(TyBaseAddress + 0x270B80) }; };
	inline float GetTyRot() { return *(float*)(TyBaseAddress + 0x271C20); };

	//Bull Pos and Rot
	inline PtrVector3 GetBullPosPtr() { return { (float*)(TyBaseAddress + 0x254268), (float*)(TyBaseAddress + 0x25426C), (float*)(TyBaseAddress + 0x254270) }; };
	inline Vector3 GetBullPos() { return { *(float*)(TyBaseAddress + 0x254268), *(float*)(TyBaseAddress + 0x25426C), *(float*)(TyBaseAddress + 0x254270) }; };
	inline float GetBullRot() { 
		float bullRot = *(float*)(TyBaseAddress + 0x2545F4);
		return std::abs((float)((int)(bullRot * 1000) % 6282) / 1000);
	};

	//Set
	inline void SetTyPos(Vector3 position) {
		PtrVector3 tyPos = GetTyPosPtr();
		*tyPos.X = position.X;
		*tyPos.Y = position.Y;
		*tyPos.Z = position.Z;
	};

	inline void SetBullPos(Vector3 position) {
		PtrVector3 bullPos = GetBullPosPtr();
		*bullPos.X = position.X;
		*bullPos.Y = position.Y;
		*bullPos.Z = position.Z;
	};
};