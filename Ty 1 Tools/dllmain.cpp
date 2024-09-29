// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "GUI.h"
#include <filesystem>
#include "TyMemoryValues.h"
#include "Levels.h"
#include "TyState.h"
#include "ini.h"
namespace fs = std::filesystem;

void LoadSettings() {
    if (!fs::exists("Plugins/Ty 1 Tools.ini"))
        return;

    ini::File settings = ini::open("Plugins/Ty 1 Tools.ini");

    if (settings.has_section("Ty 1 Tools")) {
        ini::Section ty1ToolsSection = settings["Ty 1 Tools"];

        if (ty1ToolsSection.has_key("ShowOverlay"))
            GUI::Overlay::ShowOverlay = ty1ToolsSection.get<bool>("ShowOverlay");

        if (ty1ToolsSection.has_key("EnableLevelSelect"))
            GUI::EnableLevelSelect = ty1ToolsSection.get<bool>("EnableLevelSelect");
    }

    API::LogPluginMessage("Loaded Settings from ini");
}

void SaveSettings() {
    ini::File settings;

    //Create Ty 1 Tools section
    settings.add_section("Ty 1 Tools");
    //Save the data, [Section name], (Value name, value)
    settings["Ty 1 Tools"].set<bool>("ShowOverlay", GUI::Overlay::ShowOverlay);
    settings["Ty 1 Tools"].set<bool>("EnableLevelSelect", GUI::EnableLevelSelect);

    settings.write("Plugins/Ty 1 Tools.ini");

    API::LogPluginMessage("Saved Settings to ini");
}

void Shutdown() {
    SaveSettings();
}

void CheckIfGameFinishInit() {
    //Just waiting for the game to startup, values below 5 are all uses before fully initialized
    //Will be set when reaching the title screen or gameplay (5 or 8)
    while (TyMemoryValues::GetTyGameState() < 5) {
        Sleep(100);
    }
    TyMemoryValues::SetLevelSelect(GUI::EnableLevelSelect);
    API::LogPluginMessage("Startup Set Level Select State");
}

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
        Shutdown();
        break;
    }
    return TRUE;
}

//Ticks before the game does
void TickBeforeGame(float deltaSeconds) {
    if (!GUI::init)
        GUI::Initialize();
    else
        GUI::DrawUI();

    if (GUI::DisableFallDamage && *TyState::GetTyStatePtr() == 27 && TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay && Levels::GetCurrentLevelID() != 10)
        *TyState::GetTyStatePtr() = 26;
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

    LoadSettings();

    TyMemoryValues::GetBaseAddress();

    //Subcribe functions to TygerFramework events
    //API::AddDrawPluginUI(GUI::DrawUI);
    //Make sure to cast this, otherwise TygerFramework won't get the return value
    API::AddPluginImGuiWantCaptureMouse((ImGuiWantCaptureMouseFunc)GUI::ImGuiWantCaptureMouse);

    API::AddTickBeforeGame(TickBeforeGame);

    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)CheckIfGameFinishInit, NULL, 0, nullptr);

    return true;
}

