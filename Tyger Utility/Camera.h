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
        LookAt = 6,
        Scope = 8,
        CameraOverride = 12,
        FluffyRotationLock = 13,
        Locked = 17,
        LockOn = 18, // DONT DO IT
        RexDiving = 20, // CAN BE USED TO UN-SOFTLOCK WHEN SWAPPED!!!
        CameraFocus = 23,
        Cutscene = 24,
        FreeCam = 28,
    };
    inline std::map<CameraState, std::string> StateNames{
        {Default, "Default"},
        {LookAt, "Look At"},
        {Scope, "Scope"},
        {CameraOverride, "Camera Override"},
        {FluffyRotationLock, "Fluffy Rotation Lock"},
        {Locked, "Locked"},
        {LockOn, "Lock On"},
        {RexDiving, "Rex Diving"},
        {CameraFocus, "Camera Focus"},
        {Cutscene, "Cutscene"},
        {FreeCam, "Free Cam"}
    };
	inline float* GetFreeCamSpeedPtr() { return (float*)(TyBaseAddress + 0x254B08); };
    inline CameraState GetCameraState() { return *(CameraState*)(TyBaseAddress + 0x27EBC8); };
    inline void SetCameraState(CameraState cameraState) {
        *(CameraState*)(TyBaseAddress + 0x27EBC8) = cameraState;
        *(CameraState*)(TyBaseAddress + 0x27EBD0) = cameraState;
    };

    inline Vector3 GetCameraPos() { return *(Vector3*)(TyBaseAddress + 0x27EB98); };
    inline void SetCameraPos(Vector3 position) { 
        //*(Vector3*)(TyBaseAddress + 0x27EB78) = position; //Don't change this one, causes jitter (and maybe it getting stuck)
        *(Vector3*)(TyBaseAddress + 0x27F488) = position; //Important, doesn't jitter, doesn't get stuck
        //*(Vector3*)(TyBaseAddress + 0x27F4B8) = position;
        //*(Vector3*)(TyBaseAddress + 0x27F550) = position;
    };

    inline float GetCameraRotYaw() { return *(float*)(TyBaseAddress + 0x27EE10); };
    inline float GetCameraRotPitch() { return *(float*)(TyBaseAddress + 0x27EE0C); };

    inline void SetCameraRotYaw(float rotation) { 
        *(float*)(TyBaseAddress + 0x27EE10) = rotation;
        //*(float*)(TyBaseAddress + 0x27ECC0) = rotation;
    };
    inline void SetCameraRotPitch(float rotation) { *(float*)(TyBaseAddress + 0x27EE0C) = rotation; };
}