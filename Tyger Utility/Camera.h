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

    inline Vector3 GetCameraPos() { return *(Vector3*)(TyBaseAddress + 0x27EB78); };
    inline void SetCameraPos(Vector3 position) { *(Vector3*)(TyBaseAddress + 0x27EB78) = position; };

    inline float GetCameraRotYaw() { return *(float*)(TyBaseAddress + 0x27EE10); };
    inline float GetCameraRotPitch() { return *(float*)(TyBaseAddress + 0x27EE0C); };

    inline void SetCameraRotYaw(float rotation) { *(float*)(TyBaseAddress + 0x27EE10) = rotation; };
    inline void SetCameraRotPitch(float rotation) { *(float*)(TyBaseAddress + 0x27EE0C) = rotation; };
}