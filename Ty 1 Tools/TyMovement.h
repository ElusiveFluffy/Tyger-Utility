#pragma once
#include "TyMemoryValues.h"

class TyMovement {
public:
	static float* GetGlideUpDownPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288964); }; //Default 5.5
	static float* GetGlideSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288928); }; //Default 7.0
	static float* GetRunSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288914); }; //Default 10.0
	static float* GetGroundJumpHeightPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x28893C); }; //Default 18.57417488
	static float* GetWaterJumpHeightPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288998); }; //Default 10.67707825
	static float* GetSwimSurfaceSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x28892C); }; //Default 6.0
	static float* GetAirSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288920); }; //Default 10.0

	//Bull
	static float* GetBullSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x25462C); };
};