#pragma once
#include "TyMemoryValues.h"
#include "TyPositionRotation.h"

class TyMovement {
public:
	//Ty
	static float* GetGlideUpDownPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288964); }; //Default 5.5
	static float* GetGlideSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288928); }; //Default 7.0
	static float* GetRunSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288914); }; //Default 10.0
	static float* GetGroundJumpHeightPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x28893C); }; //Default 18.57417488
	static float* GetWaterJumpHeightPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288998); }; //Default 10.67707825
	static float* GetSwimSurfaceSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x28892C); }; //Default 6.0
	static float* GetAirSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x288920); }; //Default 10.0

	static float GetTyHorizontalSpeed() {
		//Scale them based on the direction Ty is facing, so when adding them diagonal speeds aren't wrong
		float speed = std::abs(*(float*)(TyMemoryValues::TyBaseAddress + 0x270B98) * std::sin(TyPositionRotation::GetTyRot())) + //X
			std::abs(*(float*)(TyMemoryValues::TyBaseAddress + 0x270BA0) * std::cos(TyPositionRotation::GetTyRot())); //Z
		return (speed);
	};
	static float GetTyVerticalSpeed() { return *(float*)(TyMemoryValues::TyBaseAddress + 0x270B9C); };

	//Bull
	static float* GetBullSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x25462C); }; //Default 35
	static float* GetBullHardcodedSpeedPtr() { return (float*)(TyMemoryValues::TyBaseAddress + 0x43935); };

	static float GetBullHorizontalSpeed() {
		//Scale them based on the direction Bull is facing, so when adding them diagonal speeds aren't wrong
		float speed = std::abs(*(float*)(TyMemoryValues::TyBaseAddress + 0x254288) * std::sin(TyPositionRotation::GetBullRot())) + //X
			std::abs(*(float*)(TyMemoryValues::TyBaseAddress + 0x254290) * std::cos(TyPositionRotation::GetBullRot())); //Z
		return (speed);
	};
	static float GetBullVerticalSpeed() { return *(float*)(TyMemoryValues::TyBaseAddress + 0x25428C); };
};