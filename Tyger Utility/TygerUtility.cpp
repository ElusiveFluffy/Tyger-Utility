#include "TygerUtility.h"
#include "GUI.h"
#include "ini.h"
#include <filesystem>
//Ty Memory
#include "TyMemoryValues.h"
#include "TyMovement.h"
#include "TyAttributes.h"
#include "TyState.h"
#include "Camera.h"
#include "Levels.h"

#include "TygerFrameworkAPI.hpp"
namespace fs = std::filesystem;

void TygerUtility::TickBeforeGame(float deltaSeconds)
{
    //Update it after Ty uses a charge bite in game
    GUI::ChargeBiteCount = *TyAttributes::GetChargeBiteOpalCounterPtr() / 100;

    if (!GUI::init)
        GUI::Initialize();
    else
        GUI::DrawUI();

    //The second Ty state one is a bit behind but will always be Ty's current state, while the first is more like Ty's next state
    if (GUI::DisableFallDamage && (*TyState::GetTyStatePtr() == 27 || TyState::GetTyState() == 27) && TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay && Levels::GetCurrentLevelID() != 10)
        *TyState::GetTyStatePtr() = 26;

    //Just incase the camera state changes
    if (GUI::EnableFreeCam && Camera::GetCameraState() != Camera::FreeCam)
        Camera::SetCameraState(Camera::FreeCam);

    if (API::DrawingGUI())
        API::SetTyInputFlag(NoKeyboardInput, ImGui::GetIO().WantCaptureKeyboard);
}

void TygerUtility::OnTyInit() {
    //Will be set when reaching the title screen or gameplay (5 or 8)
    TyMemoryValues::SetLevelSelect(GUI::EnableLevelSelect);
    API::LogPluginMessage("Startup Set Level Select State");
    *TyMovement::GetBullSpeedPtr() = 35.0f;
}

void TygerUtility::SaveSettings() {
    ini::File settings;

    //Create Ty 1 Modding Util section
    settings.add_section(PluginName);
    //Save the data, [Section name], (Value name, value)
    settings[PluginName].set<bool>("ShowOverlay", GUI::Overlay::ShowOverlay);
    settings[PluginName].set<bool>("EnableLevelSelect", GUI::EnableLevelSelect);

    settings.write(API::GetPluginDirectory() / (PluginName + ".ini"));

    API::LogPluginMessage("Saved Settings to ini");
}

void TygerUtility::LoadSettings() {
    if (!fs::exists(API::GetPluginDirectory() / (PluginName + ".ini")))
        return;

    ini::File settings = ini::open(API::GetPluginDirectory() / (PluginName + ".ini"));

    if (settings.has_section(PluginName)) {
        ini::Section ty1ToolsSection = settings[PluginName];

        if (ty1ToolsSection.has_key("ShowOverlay"))
            GUI::Overlay::ShowOverlay = ty1ToolsSection.get<bool>("ShowOverlay");

        if (ty1ToolsSection.has_key("EnableLevelSelect"))
            GUI::EnableLevelSelect = ty1ToolsSection.get<bool>("EnableLevelSelect");
    }

    API::LogPluginMessage("Loaded Settings from ini");
}
