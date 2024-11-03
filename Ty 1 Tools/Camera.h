#pragma once
#include "TyMemoryValues.h"
#include "TyPositionRotation.h"
#include <map>
#include <string>

using namespace TyMemoryValues;
using namespace TyPositionRotation;

namespace Camera {
    enum CameraState
    {
        Default = 5,
        Scope = 8,
        CameraOverride = 12,
        FluffyRotationLock = 13,
        Locked = 17,
        Cutscene = 18, // DONT DO IT
        RexDiving = 20, // CAN BE USED TO UN-SOFTLOCK WHEN SWAPPED!!!
        ShipSpire = 23,
        Cutscene2 = 24,
        FreeCam = 28,
    };
    inline std::map<CameraState, std::string> StateNames{
        {Default, "Default"},
        {Scope, "Scope"},
        {CameraOverride, "Camera Override"},
        {FluffyRotationLock, "Fluffy Rotation Lock"},
        {Locked, "Locked"},
        {Cutscene, "Cutscene"},
        {RexDiving, "Rex Diving"},
        {ShipSpire, "Camera Spiral"},
        {Cutscene2, "Cutscene"},
        {FreeCam, "Free Cam"}
    };
	inline float* GetFreeCamSpeedPtr() { return (float*)(TyBaseAddress + 0x254B08); };
    inline CameraState GetCameraState() { return *(CameraState*)(TyBaseAddress + 0x27EBC8); };
    inline void SetCameraState(CameraState cameraState) {
        *(CameraState*)(TyBaseAddress + 0x27EBC8) = cameraState;
        *(CameraState*)(TyBaseAddress + 0x27EBD0) = cameraState;
    };
}