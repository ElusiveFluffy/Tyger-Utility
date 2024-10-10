#include "Ty1Tools.h"
#include "GUI.h"
#include "ini.h"
#include <filesystem>
//Ty Memory
#include "TyMemoryValues.h"
#include "TyMovement.h"
#include "TyAttributes.h"
#include "TyState.h"
#include "Levels.h"

#include "TygerFrameworkAPI.hpp"
namespace fs = std::filesystem;

void Ty1Tools::TickBeforeGame(float deltaSeconds)
{
    //Update it after Ty uses a charge bite in game
    GUI::ChargeBiteCount = *TyAttributes::GetChargeBiteOpalCounterPtr() / 100;

    if (!GUI::init)
        GUI::Initialize();
    else
        GUI::DrawUI();

    if (GUI::DisableFallDamage && *TyState::GetTyStatePtr() == 27 && TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay && Levels::GetCurrentLevelID() != 10)
        *TyState::GetTyStatePtr() = 26;
}

void Ty1Tools::CheckIfGameFinishInit() {
    //Just waiting for the game to startup, values below 5 are all uses before fully initialized
    while (TyMemoryValues::GetTyGameState() < 5) {
        Sleep(100);
    }
    //Will be set when reaching the title screen or gameplay (5 or 8)
    TyMemoryValues::SetLevelSelect(GUI::EnableLevelSelect);
    API::LogPluginMessage("Startup Set Level Select State");
    *TyMovement::GetBullSpeedPtr() = 35.0f;
}

void Ty1Tools::SaveSettings() {
    ini::File settings;

    //Create Ty 1 Tools section
    settings.add_section("Ty 1 Tools");
    //Save the data, [Section name], (Value name, value)
    settings["Ty 1 Tools"].set<bool>("ShowOverlay", GUI::Overlay::ShowOverlay);
    settings["Ty 1 Tools"].set<bool>("EnableLevelSelect", GUI::EnableLevelSelect);

    settings.write("Plugins/Ty 1 Tools.ini");

    API::LogPluginMessage("Saved Settings to ini");
}

void Ty1Tools::LoadSettings() {
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
