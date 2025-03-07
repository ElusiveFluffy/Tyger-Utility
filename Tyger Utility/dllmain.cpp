// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "GUI.h"
#include "TyMemoryValues.h"
#include "TygerUtility.h"
#include "TeleportPositions.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        if (API::IsInitialized())
        {
            TygerUtility::SaveSettings();
            TeleportPositions::SavePositionsToFile();
        }
        break;
    }
    return TRUE;
}

EXTERN_C void TygerFrameworkPluginRequiredVersion(TygerFrameworkPluginVersion* version) {
    //Specifiy the version number defined in the API
    version->Major = TygerFrameworkPluginVersion_Major;
    version->Minor = TygerFrameworkPluginVersion_Minor;
    version->Patch = TygerFrameworkPluginVersion_Patch;

    version->CompatibleGames = { 1 };
}

EXTERN_C bool TygerFrameworkPluginInitialize(TygerFrameworkPluginInitializeParam* param) {

    API::Initialize(param);

    TygerUtility::LoadSettings();
    TeleportPositions::LoadPositionsFromFile();

    TyMemoryValues::GetBaseAddress();

    //Subcribe functions to TygerFramework events
    //Make sure to cast this, otherwise TygerFramework won't get the return value
    API::AddPluginImGuiWantCaptureMouse((ImGuiWantCaptureMouseFunc)GUI::ImGuiWantCaptureMouse);

    API::AddTickBeforeGame(TygerUtility::TickBeforeGame);

    API::AddOnTyInitialized(TygerUtility::OnTyInit);

    return true;
}

