#include "TygerUtility.h"
#include "GUI.h"
#include "MovementGUI.h"
#include "FreecamGUI.h"
#include "MiscGUI.h"
#include "ini.h"
#include "TeleportPositions.h"
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
    MiscGUI::ChargeBiteCount = *TyAttributes::GetChargeBiteOpalCounterPtr() / 100;

    if (!GUI::init)
        GUI::Initialize();
    else
        GUI::DrawUI();
    
    //Set the spawn position
    if (Levels::GetCurrentLevelID() != CurrentLevel && TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay)
    {
        if (!TyState::IsBull() && TyState::GetTyState() != 0)
        {
            CurrentLevel = Levels::GetCurrentLevelID();
            TeleportPositions::PositionValues value{ true, TyPositionRotation::GetTyPos(), TyPositionRotation::GetTyYawRot(), 35, Camera::GetCameraPos(), Camera::GetCameraRotYaw(), Camera::GetCameraRotPitch() };
            TeleportPositions::SpawnPositions.emplace(CurrentLevel, value);
        }
        else if (TyState::IsBull() && TyState::GetBullState() != -1)
        {
            //Set the current level first to avoid having the thread be created multiple times
            CurrentLevel = Levels::GetCurrentLevelID();
            //Needs to be delayed a bit, unlike Ty's spawn position
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)SetBullSpawnPos, NULL, 0, nullptr);
        }
    }

    //The second Ty state one is a bit behind but will always be Ty's current state, while the first is more like Ty's next state
    if (MovementGUI::DisableFallDamage && (*TyState::GetTyStatePtr() == 27 || TyState::GetTyState() == 27) && TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay && !TyState::IsBull())
        *TyState::GetTyStatePtr() = 26;

    //Just incase the camera state changes (don't try changing it if its trying to play a cutscene)
    if (FreecamGUI::EnableFreeCam && Camera::GetCameraState() != Camera::Cutscene)
    {
        if (Camera::GetCameraState() != Camera::FreeCam)
            Camera::SetCameraState(Camera::FreeCam);

        if (FreecamGUI::LockTyMovement) {
            *TyState::GetTyStatePtr() = 50;
            TyState::SetBullState(-1);
        }
    }

    if (API::DrawingGUI())
        API::SetTyInputFlag(NoKeyboardInput, ImGui::GetIO().WantCaptureKeyboard);
    else
        //Make is so if you were typing in a text box when closing the menu it won't continue stopping keyboard input because imgui hasn't updated the capture keyboard state
        API::SetTyInputFlag(NoKeyboardInput, false);

    //Decrease the counter for the text using delta seconds
    if (GUI::Overlay::PosTextShowSeconds != 0)
        GUI::Overlay::PosTextShowSeconds = std::clamp(GUI::Overlay::PosTextShowSeconds - deltaSeconds, 0.0f, 2.0f);
}

void TygerUtility::SetBullSpawnPos()
{
    //Give time for the camera to be set to the normal positon (otherwise its in offset to the right)
    Sleep(20);
    TeleportPositions::PositionValues value{ true, TyPositionRotation::GetBullPos(), TyPositionRotation::GetUnmodifiedBullRot(), 0, Camera::GetCameraPos(), Camera::GetCameraRotYaw(), Camera::GetCameraRotPitch() };
    TeleportPositions::SpawnPositions.emplace(CurrentLevel, value);
}

void TygerUtility::OnTyInit() {
    TyMovement::EditSwimSpeedPtrs();

    //Disable the leaderboard to protect the times on it
    TyMemoryValues::DisableLeaderboard();
    API::LogPluginMessage("Protecting the Leaderboard");

    //The game needs to be initialized to set this or it'll just crash
    TyMemoryValues::SetLevelSelect(GUI::EnableLevelSelect);
    API::LogPluginMessage("Startup Set Level Select State");
    //Usually 0 before loading outback
    *TyMovement::GetBullSpeedPtr() = 35.0f;

    GUI::Hub4PluginExists = fs::exists(API::GetPluginDirectory() / "Hub 4 Restored.dll");
}

void TygerUtility::SaveSettings() {
    ini::File settings;

    //Create Tyger Util section
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
