// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "GUI.h"
#include "TyMemoryValues.h"
#include "Ty1ModdingUtil.h"

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
            Ty1ModdingUtil::SaveSettings();
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

    Ty1ModdingUtil::LoadSettings();

    TyMemoryValues::GetBaseAddress();

    //Subcribe functions to TygerFramework events
    //Make sure to cast this, otherwise TygerFramework won't get the return value
    API::AddPluginImGuiWantCaptureMouse((ImGuiWantCaptureMouseFunc)GUI::ImGuiWantCaptureMouse);

    API::AddTickBeforeGame(Ty1ModdingUtil::TickBeforeGame);

    API::AddOnTyInitialized(Ty1ModdingUtil::OnTyInit);

    return true;
}

