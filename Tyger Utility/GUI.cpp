#include "GUI.h"
#include "framework.h"
#include "TygerFrameworkAPI.hpp"
#include "TygerUtility.h"
#include "TeleportPositions.h"
#include <string>
#include <format>
#include <regex>

//Memory
#include "TyMemoryValues.h"
#include "TyAttributes.h"
#include "TyState.h"
#include "TyMovement.h"
#include "TyPositionRotation.h"
#include "Camera.h"
#include "Levels.h"
#include "SaveData.h"
using namespace TyPositionRotation;

//Fonts
#include "Fonts/TyFont.hpp"
#include "Fonts/TyNumberFont.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h" //For std::string
#include "imgui_internal.h" //For Free Drawing

//WndProc to be able to interact with imgui or block any WndProc from interacting with the Ty window
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	//Teleport back to spawn from the position auto set
	if (msg == WM_KEYDOWN && TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay &&
		((!TyState::IsBull() && TyState::GetTyState() != 0) ||
		(TyState::IsBull() && TyState::GetBullState() != -1)))
	{
		switch ((int)wParam)
		{
		case VK_HOME:
			auto position = TeleportPositions::SpawnPositions[Levels::GetCurrentLevelID()];
			AdvancedTeleportPlayer(position);
			GUI::Overlay::SetAndShowSlotText("Returned to Spawn", -1);
			break;
		// [{
		case VK_OEM_4:
			TeleportPositions::CurrentSlot = TeleportPositions::CurrentSlot == 0 ? TeleportPositions::SlotCount - 1 : TeleportPositions::CurrentSlot - 1;
			GUI::Overlay::SetAndShowSlotText("Changed to Slot", TeleportPositions::CurrentSlot);
			break;
		// ]}
		case VK_OEM_6:
			TeleportPositions::CurrentSlot = (TeleportPositions::CurrentSlot + 1) % TeleportPositions::SlotCount;
			GUI::Overlay::SetAndShowSlotText("Changed to Slot", TeleportPositions::CurrentSlot);
			break;
		case VK_F4:
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				auto& positions = TeleportPositions::SavedPositions[Levels::GetCurrentLevelID()];

				if (!TyState::IsBull())
					positions[TeleportPositions::CurrentSlot] = { true, TyPositionRotation::GetTyPos(), TyPositionRotation::GetTyYawRot(), TyState::GetTyState(), Camera::GetCameraPos(), Camera::GetCameraRotYaw(), Camera::GetCameraRotPitch() };
				else
					positions[TeleportPositions::CurrentSlot] = { true, TyPositionRotation::GetBullPos(), TyPositionRotation::GetUnmodifiedBullRot(), TyState::GetBullState(), Camera::GetCameraPos(), Camera::GetCameraRotYaw(), Camera::GetCameraRotPitch() };

				//Save the positions every time its set just in case a crash happens and you lose your unsaved positions
				TeleportPositions::SavePositionsToFile();

				GUI::Overlay::SetAndShowSlotText("Saved slot", TeleportPositions::CurrentSlot);
			}
			else
			{
				auto position = TeleportPositions::SavedPositions[Levels::GetCurrentLevelID()][TeleportPositions::CurrentSlot];
				if (position.ValidSlot)
				{
					AdvancedTeleportPlayer(position);
					GUI::Overlay::SetAndShowSlotText("Loaded slot", TeleportPositions::CurrentSlot);
				}
				else
				{
					GUI::Overlay::SetAndShowSlotText("No position in slot", TeleportPositions::CurrentSlot);
				}
			}
			break;
		}
	}

	if (API::DrawingGUI())
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

	return false;
}

void GUI::Initialize()
{
	//Need to cast this, otherwise TygerFramework won't get the return value
	API::AddPluginWndProc((WndProcFunc)WndProc);

	Levels::InitLevelNames();

	//Setup ImGui Context
	ImGui::CreateContext();

	//Set the font to be the same as TygerFramework
	API::SetImGuiFont(ImGui::GetIO().Fonts);
	SetImGuiStyle();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	if (API::GetTyWindowHandle() == 0) {
		API::LogPluginMessage("Error Getting Ty Window Handle", Error);
		return;
	}
	//Setup backend
	ImGui_ImplWin32_InitForOpenGL(API::GetTyWindowHandle());
	ImGui_ImplOpenGL3_Init();

	API::LogPluginMessage("Initialized ImGui");
	GUI::init = true;
}

void GUI::SetImGuiStyle() {
	ImFontAtlas* fonts = ImGui::GetIO().Fonts;

	ImFontConfig custom_icons{};
	custom_icons.FontDataOwnedByAtlas = false;

	GUI::TyFont = fonts->AddFontFromMemoryCompressedTTF(SfSlapstickComic_compressed_data, SfSlapstickComic_compressed_size, GUI::FontSize);
	GUI::TyNumberFont = fonts->AddFontFromMemoryCompressedTTF(TyNumberFont_compressed_data, TyNumberFont_compressed_size, 28);
	fonts->Build();
}

void GUI::DrawUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Overlay::ShowOverlay)
		Overlay::DrawOverlay();

	if (Overlay::PosTextShowSeconds != 0.0f)
		Overlay::DrawPosSlotOverlay();

	if (API::DrawingGUI())
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(60, 420), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(390, 340), ImGuiCond_FirstUseEver);
		ImGui::Begin(TygerUtility::PluginName.c_str());
		ImGui::Checkbox("Show Debug Overlay", &Overlay::ShowOverlay);
		AddToolTip("Tip: You can drag the overlay around to place it anywhere you want\nThe overlay only shows during gameplay");

		//Game finished initializing
		if (TyMemoryValues::GetTyGameState() > 4)
		{
			//Is only true if the check box changes value
			if (ImGui::Checkbox("Enable Level Select", &EnableLevelSelect))
				TyMemoryValues::SetLevelSelect(EnableLevelSelect);

			ImGui::Spacing();

			if (ImGui::BeginTabBar("Tool Tabs")) {
				if (ImGui::BeginTabItem("Save")) {
					SaveDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Movement")) {
					MovementDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Position")) {
					PositionDrawUI();
					TeleportPositions::TeleportPosDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Free Cam")) {
					FreeCamDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Misc")) {
					MiscDrawUI();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		else
			ImGui::Text("Game still initializing, please wait");

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

LevelCode selectedLevel = Z1;
short opalCount = 0;
void GUI::SaveDrawUI()
{
	auto saveData = SaveData::GetData();

	ImGui::Text("Some Things Only Update After Reloading the Level");
	ImGui::Checkbox("Hardcore", &saveData->IsHardcore);
	if (ImGui::InputScalar("Lives", ImGuiDataType_U16, &saveData->Lives, &IntStepAmount))
	{
		//The game caps lives to 99
		if (saveData->Lives > 99)
			saveData->Lives = 99;
		//Show time
		*(int*)(TyBaseAddress + 0x26550c) = 160;
		//Show lives UI
		*(int*)(TyBaseAddress + 0x265508) = 1;
	}

	if (ImGui::TreeNode("Attributes")) {
		if (ImGui::BeginTable("Attributes", 3)) {
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextColumn(); ImGui::Checkbox("Swim", &saveData->AttributeData.LearntToSwim);
			ImGui::TableNextColumn(); ImGui::Checkbox("Dive", &saveData->AttributeData.LearntToDive);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Double Health", &saveData->AttributeData.GotExtraHealth)) {
				//Run the function from the game to make it so you don't need to change levels for the double health to show up
				TyMemoryValues::SetGotDoubleHealth(saveData->AttributeData.GotExtraHealth);
			}
		}
		ImGui::EndTable();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Rangs"))
	{
		if (ImGui::Button("Give All Rangs"))
			TyAttributes::SetAllRangs(true);
		ImGui::SameLine();
		if (ImGui::Button("Remove All Rangs"))
			TyAttributes::SetAllRangs(false);

		if (ImGui::Button("Give Element Rangs"))
			TyAttributes::SetElementRangs(true);
		ImGui::SameLine();
		if (ImGui::Button("Give Techno Rangs"))
			TyAttributes::SetTechnoRangs(true);

		if (ImGui::BeginTable("Rangs", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("2nd Rang", &saveData->AttributeData.GotSecondRang);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boomerang", &saveData->AttributeData.GotBoomerang);
			ImGui::TableNextColumn(); ImGui::Checkbox("Flamerang", &saveData->AttributeData.GotFlamerang);

			//Row 2
			ImGui::TableNextColumn(); ImGui::Checkbox("Frostyrang", &saveData->AttributeData.GotFrostyrang);
			if (Hub4PluginExists)
			{
				//Talisman data is right after the attribute data and is used for the extra rangs in the hub 4 restored plugin
				ImGui::TableNextColumn(); ImGui::Checkbox("Smasharang", &saveData->Talismans[0]);
			}
			ImGui::TableNextColumn(); ImGui::Checkbox("Zappyrang", &saveData->AttributeData.GotZappyrang);
			ImGui::TableNextColumn(); ImGui::Checkbox("Aquarang", &saveData->AttributeData.GotAquarang);

			//Row 3
			ImGui::TableNextColumn(); ImGui::Checkbox("Zoomerang", &saveData->AttributeData.GotZoomerang);
			ImGui::TableNextColumn(); ImGui::Checkbox("Multirang", &saveData->AttributeData.GotMultirang);
			ImGui::TableNextColumn(); ImGui::Checkbox("Infrarang", &saveData->AttributeData.GotInfrarang);

			//Row 4
			ImGui::TableNextColumn(); ImGui::Checkbox("Megarang", &saveData->AttributeData.GotMegarang);
			if (!Hub4PluginExists)
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("Kaboomarang", &saveData->AttributeData.GotKaboomerang);
			}
			else {
				ImGui::TableNextColumn(); ImGui::Checkbox("Tech Rang 5", &saveData->Talismans[1]);
			}
			ImGui::TableNextColumn(); ImGui::Checkbox("Chronorang", &saveData->AttributeData.GotChronorang);

			//Row 5
			if (Hub4PluginExists) {
				ImGui::TableNextColumn(); ImGui::Checkbox("Kaboomarang", &saveData->AttributeData.GotKaboomerang);
				ImGui::TableNextColumn(); ImGui::Checkbox("Tech Rang 8", &saveData->Talismans[2]);
			}
			ImGui::TableNextColumn(); ImGui::Checkbox("Doomarang", &saveData->AttributeData.GotDoomerang);
		}
		ImGui::EndTable();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("LevelData")) {
		ImGui::Combo("Level", (int*)&selectedLevel, Levels::Names, 24);

		LevelData* levelData = &saveData->LevelData[selectedLevel];
		ImGui::InputScalar("Times Entered", ImGuiDataType_U8, &levelData->TimesEntered, &IntStepAmount);

		if (ImGui::TreeNode("Opals"))
		{
			if (Levels::GetCurrentLevelID() == selectedLevel) {
				ImGui::TextWrapped("Live Opal Editing Isn't Supported, Exit the Level to Edit the Opals.");
			}
			else
			{
				if (ImGui::Button("Give All Opals"))
					SaveData::GiveAllOpals(selectedLevel);
				ImGui::SameLine();
				if (ImGui::Button("Reset All Opals"))
					SaveData::ResetAllOpals(selectedLevel);

				opalCount = SaveData::CountLevelOpals(selectedLevel);
				if (ImGui::InputScalar("Opal Count", ImGuiDataType_S16, &opalCount, &IntStepAmount))
					opalCount = SaveData::SetOpals(opalCount, selectedLevel);
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Thunder Eggs"))
		{
			if (ImGui::BeginTable("Thunder Eggs", 4)) {
				//Row 1
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 1", &levelData->ThunderEggs[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 2", &levelData->ThunderEggs[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 3", &levelData->ThunderEggs[2]);
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 4", &levelData->ThunderEggs[3]);

				//Row 2
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 5", &levelData->ThunderEggs[4]);
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 6", &levelData->ThunderEggs[5]);
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 7", &levelData->ThunderEggs[6]);
				ImGui::TableNextColumn(); ImGui::Checkbox("TE 8", &levelData->ThunderEggs[7]);
			}
			ImGui::EndTable();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Golden Cogs"))
		{
			if (ImGui::BeginTable("Golden Cogs", 3)) {
				//Row 1
				ImGui::TableNextColumn(); ImGui::Checkbox("Cog 1", &levelData->GoldenCogs[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Cog 2", &levelData->GoldenCogs[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Cog 3", &levelData->GoldenCogs[2]);

				//Row 2
				ImGui::TableNextColumn(); ImGui::Checkbox("Cog 4", &levelData->GoldenCogs[3]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Cog 5", &levelData->GoldenCogs[4]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Cog 6", &levelData->GoldenCogs[5]);

				//Row 3
				ImGui::TableNextColumn(); ImGui::Checkbox("Gog 7", &levelData->GoldenCogs[6]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Gog 8", &levelData->GoldenCogs[7]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Gog 9", &levelData->GoldenCogs[8]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Gog 10", &levelData->GoldenCogs[9]);
			}
			ImGui::EndTable();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bilbies"))
		{
			if (ImGui::BeginTable("Bilbies", 3)) {
				//Row 1
				ImGui::TableNextColumn(); ImGui::Checkbox("Bilby 1", &levelData->Bilbies[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Bilby 2", &levelData->Bilbies[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Bilby 3", &levelData->Bilbies[2]);

				//Row 2
				ImGui::TableNextColumn(); ImGui::Checkbox("Bilby 4", &levelData->Bilbies[3]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Bilby 5", &levelData->Bilbies[4]);
			}
			ImGui::EndTable();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Picture Frames"))
		{
			if (ImGui::Button("Give All Pictures Frames"))
				SaveData::SetAllLevelPictures(selectedLevel, true);
			ImGui::SameLine();
			if (ImGui::Button("Reset Pictures Frames"))
				SaveData::SetAllLevelPictures(selectedLevel, false);

			if (ImGui::BeginTable("Picture Frames", 4)) {
				int* picturePointersStart = (int*)(TyBaseAddress + 0x288610) + (selectedLevel * 3);
				int* pictureDataPointers = (int*)*picturePointersStart;
				for (int pointer = 0; (int)&pictureDataPointers[pointer] < *(picturePointersStart + 1); pointer++) {
					int id = pictureDataPointers[pointer];
					std::string name = "ID " + std::to_string(id);
					bool pictureState = SaveData::GetPictureFrame(id);
					if (ImGui::TableNextColumn(); ImGui::Checkbox(name.c_str(), &pictureState))
						SaveData::SetPictureFrame(id, pictureState);
				}
			}
			ImGui::EndTable();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Zone Data")) {
		ImGui::SeparatorText("Rainbow Cliffs [Z Zone]:");
		if (ImGui::BeginTable("Z Zone", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("Unlocked", &saveData->ZoneData[ZoneZ].Unlocked);
			ImGui::TableNextColumn(); ImGui::Checkbox("Complete", &saveData->ZoneData[ZoneZ].Complete);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boss Active", &saveData->ZoneData[ZoneZ].BossActive);
		}
		ImGui::EndTable();
		ImGui::SeparatorText("Hub 1 [A Zone]:");
		if (ImGui::BeginTable("A Zone", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("Unlocked", &saveData->ZoneData[ZoneA].Unlocked);
			ImGui::TableNextColumn(); ImGui::Checkbox("Complete", &saveData->ZoneData[ZoneA].Complete);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boss Active", &saveData->ZoneData[ZoneA].BossActive);
		}
		ImGui::EndTable();
		ImGui::SeparatorText("Hub 2 [B Zone]:");
		if (ImGui::BeginTable("B Zone", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("Unlocked", &saveData->ZoneData[ZoneB].Unlocked);
			ImGui::TableNextColumn(); ImGui::Checkbox("Complete", &saveData->ZoneData[ZoneB].Complete);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boss Active", &saveData->ZoneData[ZoneB].BossActive);
		}
		ImGui::EndTable();
		ImGui::SeparatorText("Hub 3 [C Zone]:");
		if (ImGui::BeginTable("C Zone", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("Unlocked", &saveData->ZoneData[ZoneC].Unlocked);
			ImGui::TableNextColumn(); ImGui::Checkbox("Complete", &saveData->ZoneData[ZoneC].Complete);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boss Active", &saveData->ZoneData[ZoneC].BossActive);
		}
		ImGui::EndTable();
		ImGui::SeparatorText("Hub 4 [D Zone]:");
		if (ImGui::BeginTable("D Zone", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("Unlocked", &saveData->ZoneData[ZoneD].Unlocked);
			ImGui::TableNextColumn(); ImGui::Checkbox("Complete", &saveData->ZoneData[ZoneD].Complete);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boss Active", &saveData->ZoneData[ZoneD].BossActive);
		}
		ImGui::EndTable();
		ImGui::SeparatorText("Final Hub [E Zone]:");
		if (ImGui::BeginTable("E Zone", 3)) {
			//Row 1
			ImGui::TableNextColumn(); ImGui::Checkbox("Unlocked", &saveData->ZoneData[ZoneE].Unlocked);
			ImGui::TableNextColumn(); ImGui::Checkbox("Complete", &saveData->ZoneData[ZoneE].Complete);
			ImGui::TableNextColumn(); ImGui::Checkbox("Boss Active", &saveData->ZoneData[ZoneE].BossActive);
		}
		ImGui::EndTable();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Talismans")) {
		if (Hub4PluginExists) {
			//The talisman bools get moved on top of the talisman placed bools, as they're seemingly unused and
			//basically do the same thing as the talisman bools
			if (ImGui::BeginTable("Talismans", 3)) {
				//Row 1
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 1", &saveData->TalismansPlaced[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 2", &saveData->TalismansPlaced[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 3", &saveData->TalismansPlaced[2]);
		
				//Row 2
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 4", &saveData->TalismansPlaced[3]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 5", &saveData->TalismansPlaced[4]);
			}
			ImGui::EndTable();
		
		}
		else {
			if (ImGui::BeginTable("Talismans", 3)) {
				//Row 1
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 1", &saveData->Talismans[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 2", &saveData->Talismans[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 3", &saveData->Talismans[2]);

				//Row 2
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 4", &saveData->Talismans[3]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Talisman 5", &saveData->Talismans[4]);
			}
			ImGui::EndTable();
			ImGui::Separator();
			ImGui::TextWrapped("The placed ones seem unused, the zone complete check is what actually makes them appear in RC.");
			if (ImGui::BeginTable("Talismans Placed", 3)) {
				//Row 1
				ImGui::TableNextColumn(); ImGui::Checkbox("Placed 1", &saveData->TalismansPlaced[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Placed 2", &saveData->TalismansPlaced[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Placed 3", &saveData->TalismansPlaced[2]);

				//Row 2
				ImGui::TableNextColumn(); ImGui::Checkbox("Placed 4", &saveData->TalismansPlaced[3]);
				ImGui::TableNextColumn(); ImGui::Checkbox("Placed 5", &saveData->TalismansPlaced[4]);
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("FVMs Watched")) {

		if (ImGui::BeginTable("FMV", 2)) {
			//Row 1
			bool fmvState = SaveData::GetFmvWatched(Prologue);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Prologue", &fmvState))
				SaveData::SetFmvWatched(Prologue, fmvState);
			fmvState = SaveData::GetFmvWatched(CassFindsFirstTalisman);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Cass Finds Talisman", &fmvState))
				SaveData::SetFmvWatched(CassFindsFirstTalisman, fmvState);

			//Row 2
			fmvState = SaveData::GetFmvWatched(SlyRevealsTyAfterTalismans);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Sly Reveals Ty", &fmvState))
				SaveData::SetFmvWatched(SlyRevealsTyAfterTalismans, fmvState);
			fmvState = SaveData::GetFmvWatched(CassSendsSlyStopTy);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Cass Sends Sly to Stop Ty", &fmvState))
				SaveData::SetFmvWatched(CassSendsSlyStopTy, fmvState);

			//Row 3
			fmvState = SaveData::GetFmvWatched(CassRevealsPlans);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Cass Reveals Plan", &fmvState))
				SaveData::SetFmvWatched(CassRevealsPlans, fmvState);
			fmvState = SaveData::GetFmvWatched(FrogTalisman);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Defeat Bull Talisman", &fmvState))
				SaveData::SetFmvWatched(FrogTalisman, fmvState);

			//Row 4
			fmvState = SaveData::GetFmvWatched(CockatooTalisman);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Defeat Crikey Talisman", &fmvState))
				SaveData::SetFmvWatched(CockatooTalisman, fmvState);
			fmvState = SaveData::GetFmvWatched(PlatypusTalisman);
			ImGui::TableNextColumn(); if (ImGui::Checkbox("Defeat Fluffy Talisman", &fmvState))
				SaveData::SetFmvWatched(PlatypusTalisman, fmvState);
		}
		ImGui::EndTable();
		ImGui::TreePop();
	}
}

void GUI::FloatSliderElement(std::string text, float* valuePtr, float min, float max, float defaultValue) {
	ImGui::Text(text.c_str());
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat(("##" + text).c_str(), valuePtr, min, max);
	ImGui::SameLine();
	//Needs ## so it has a different internal name from all the others
	if (ImGui::Button(("Reset##Reset " + text).c_str()))
		*valuePtr = defaultValue;
}

void GUI::MovementDrawUI()
{
	if (!TyState::IsBull())
	{
		ImGui::Checkbox("Disable Fall Damage", &DisableFallDamage);
		AddToolTip("Also disables the long fall animation");

		ImGui::Spacing();
		if (ImGui::Button("Give Groundswim"))
			*TyState::GetTyStatePtr() = 39;

		ImGui::Spacing();
		FloatSliderElement("Glide Up/Down", TyMovement::GetGlideUpDownPtr(), 20, -20, 5.5f);

		ImGui::Spacing();
		FloatSliderElement("Glide Speed", TyMovement::GetGlideSpeedPtr(), 2.0f, 100, 7.0f);

		ImGui::Spacing();
		FloatSliderElement("Run Speed", TyMovement::GetRunSpeedPtr(), 1.0f, 100, 10.0f);

		ImGui::Spacing();
		FloatSliderElement("Jump Height", TyMovement::GetGroundJumpHeightPtr(), 5.0f, 100, 18.57417488f);

		ImGui::Spacing();
		FloatSliderElement("Airborne Speed", TyMovement::GetAirSpeedPtr(), 0.25f, 100, 10.0f);

		ImGui::Spacing();
		FloatSliderElement("Swim Speed", &TyMovement::SwimSpeed, 2.5f, 100, 20.0f);

		ImGui::Spacing();
		FloatSliderElement("Swim Surface Speed", TyMovement::GetSwimSurfaceSpeedPtr(), 1.0f, 100, 6.0f);

		ImGui::Spacing();
		FloatSliderElement("Water Jump Height", TyMovement::GetWaterJumpHeightPtr(), 0.25f, 100, 10.67707825f);
	}
	else
	{
		ImGui::Text("Bull Run Speed");
		ImGui::SetNextItemWidth(sliderWidth);
		//Only true when the slider changes
		if (ImGui::SliderFloat("##Bull Run Speed", TyMovement::GetBullSpeedPtr(), 0.25f, 200))
		{
			TyMovement::SetHardcodedBullSpeed();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset##Reset Bull Run Speed"))
		{
			*TyMovement::GetBullSpeedPtr() = 35.0f;
			TyMovement::SetHardcodedBullSpeed();
		}
	}
}

int GUI::PositionTextBoxFilter(ImGuiInputTextCallbackData* data)
{
	//Only allow stuff that matches any of these characters
	if (strchr("-1234567890,. ", (char)data->EventChar))
		return 0;
	return 1;
}

std::vector<std::string> Split(const std::string str, const std::string regex_str)
{
	std::regex regexz(regex_str);
	std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
		std::sregex_token_iterator());
	return list;
}

void GUI::PositionDrawUI()
{
	ImGui::Checkbox("Auto Teleport", &AutoSetPosition);
	AddToolTip("Automatically set's Ty/Bull's position when they're edited");
	ImGui::SameLine();
	ImGui::Checkbox("Don't Auto Update Position", &DontAutoUpdatePosition);

	if (!TyState::IsBull())
	{
		//Only auto update it if none have changed
		if (!AnyChanged && !DontAutoUpdatePosition)
		{
			TyBullPos = TyPositionRotation::GetTyPos();
			PositionText = std::format("{:.3f}, {:.3f}, {:.3f}", TyBullPos.X, TyBullPos.Y, TyBullPos.Z);
		}
		ImGui::Text("Ty Position (X, Y, Z):");
		SetPositionElements();
		if (ImGui::Button("Teleport") || (AutoSetPosition && AnyChanged))
		{
			auto posDelta = TyBullPos - TyPositionRotation::GetTyPos();

			Camera::SetCameraPos(Camera::GetCameraPos() + posDelta);
			TyPositionRotation::SetTyPos(TyBullPos);
			AnyChanged = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Update Position"))
			TyBullPos = TyPositionRotation::GetTyPos();
	}
	else
	{
		//Only auto update it if none have changed
		if (!AnyChanged && !DontAutoUpdatePosition)
		{
			TyBullPos = TyPositionRotation::GetBullPos();
			PositionText = std::format("{:.3f}, {:.3f}, {:.3f}", TyBullPos.X, TyBullPos.Y, TyBullPos.Z);
		}
		ImGui::Text("Bull Position (X, Y, Z):");
		SetPositionElements();
		if (ImGui::Button("Teleport") || (AutoSetPosition && AnyChanged))
		{
			auto posDelta = TyBullPos - TyPositionRotation::GetBullPos();

			Camera::SetCameraPos(Camera::GetCameraPos() + posDelta);
			TyPositionRotation::SetBullPos(TyBullPos);
			AnyChanged = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Update Position"))
			TyBullPos = TyPositionRotation::GetBullPos();
	}

}

void GUI::SetPositionElements()
{
	//To be able to copy and paste in the position
	ImGui::InputText("##Position", &PositionText, ImGuiInputTextFlags_CallbackCharFilter, GUI::PositionTextBoxFilter);
	if (ImGui::IsItemDeactivated())
	{
		std::vector<std::string> positions = Split(PositionText, ",");
		if (positions.size() == 3 && PositionText != std::format("{:.3f}, {:.3f}, {:.3f}", TyBullPos.X, TyBullPos.Y, TyBullPos.Z))
		{
			TyBullPos = { std::stof(positions[0]), std::stof(positions[1]), std::stof(positions[2]) };
			AnyChanged = true;
		}
	}
	//ImGui::Spacing();
	//Or just so if any have previously changed it'll keep it true
	//AnyChanged = ImGui::InputScalar("X", ImGuiDataType_Float, &TyBullPos.X, &FloatStepAmount) || AnyChanged;
	//AnyChanged = ImGui::InputScalar("Y", ImGuiDataType_Float, &TyBullPos.Y, &FloatStepAmount) || AnyChanged;
	//AnyChanged = ImGui::InputScalar("Z", ImGuiDataType_Float, &TyBullPos.Z, &FloatStepAmount) || AnyChanged;
}

void GUI::FreeCamDrawUI()
{
	//Only runs when the checkbox state changes
	if (ImGui::Checkbox("Enable Free Cam", &EnableFreeCam)) {
		if (EnableFreeCam)
		{
			Camera::SetCameraState(Camera::FreeCam);
			//Ty states get set in the tick before game event in TygerUtility.cpp
		}
		else
		{
			Camera::SetCameraState(Camera::Default);
			//Just always reset it just incase
			*TyState::GetTyStatePtr() = 35;
			TyState::SetBullState(0);
		}
	}
	//To unlock/lock it while free cam is active
	if (ImGui::Checkbox("Lock Ty's Movement During Free Cam", &LockTyMovement)) {
		if (LockTyMovement && EnableFreeCam)
		{
			*TyState::GetTyStatePtr() = 50;
			TyState::SetBullState(-1);
		}
		else
		{
			*TyState::GetTyStatePtr() = 35;
			TyState::SetBullState(0);
		}
	}
	AddToolTip("Makes Bull invisible as they have no locked state that they can be set to\nthat doesn't turn them invisible and doesn't cause any issues");

	ImGui::Spacing();
	FloatSliderElement("Free Cam Speed", Camera::GetFreeCamSpeedPtr(), 0.1f, 25, 0.6f);
}

void GUI::MiscDrawUI()
{
	if (ImGui::InputScalar("Charge Bite Count", ImGuiDataType_U8, &ChargeBiteCount, &IntStepAmount))
		*TyAttributes::GetChargeBiteOpalCounterPtr() = ChargeBiteCount * 100;

	ImGui::Checkbox("Ty Invincibility", TyState::TyInvincibility());
	AddToolTip("Only affects Ty, doesn't affect Bull");
}

void GUI::AddToolTip(const char* toolTip)
{
	ImGui::SameLine();
	ImGui::Text("(?)");
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(toolTip);
}

bool GUI::ImGuiWantCaptureMouse()
{
	return ImGui::GetIO().WantCaptureMouse;
}

std::string GUI::Overlay::AddSpacesBeforeCapitalAndNum(std::string text)
{
	std::string new_str = "";
	bool lastCharWasNumber = false;
	//Iterate through the characters in the string (except the last character)
	for (UINT i = 0; i < (text.length() - 1); i++) {
		new_str += text[i]; // Append the current character to the new string

		bool isNum = isdigit(text[i + 1]);
		//Check if the next character is uppercase or a number, group numbers together
		if (isupper(text[i + 1]) || (!lastCharWasNumber && isNum)) {
			//If the next character is uppercase, insert a space in the new string
			new_str += " ";
			lastCharWasNumber = isNum;
		}
	}
	//Append the last character of the input string to the new string
	new_str += text.back();
	return new_str;
}

std::string GUI::Overlay::TyStateText() {
	//If the map doesn't contain the state just return a blank string
	if (!TyState::Ty.contains(TyState::GetTyState()))
		return "";

	return TyState::Ty[TyState::GetTyState()];
}

std::string GUI::Overlay::BullStateText() {
	//If the map doesn't contain the state just return a blank string
	if (!TyState::Bull.contains(TyState::GetBullState()))
		return "";

	return TyState::Bull[TyState::GetBullState()];
}

std::string GUI::Overlay::CameraStateText()
{
	//If the map doesn't contain the state just return a blank string
	if (!Camera::StateNames.contains(Camera::GetCameraState()))
		return "";

	return Camera::StateNames[Camera::GetCameraState()];
}

void GUI::Overlay::DrawOverlay()
{
	ImGuiIO& io = ImGui::GetIO();

	//Remove all window elements
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin((TygerUtility::PluginName + " Overlay").c_str(), nullptr, ImGuiWindowFlags_NoDecoration);
	//Reset the counts
	TextLineCount = 0;
	LongestLine = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImDrawList* drawList = window->DrawList;
	//Set the text start pos to the window pos
	TextStartPos = window->Pos;

	//Only draw this overlay elements during gameplay
	if (TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay)
	{
		//Only Show the Values for the bull if in outback safari
		if (!TyState::IsBull())
		{
			DrawDropShadowText(drawList, "Ty:");
			Vector3 tyPos = TyPositionRotation::GetTyPos();
			DrawLabelWithNumbers(drawList, "Pos:", std::format("{:.2f}, {:.2f}, {:.2f}", tyPos.X, tyPos.Y, tyPos.Z));
			DrawLabelWithNumbers(drawList, "Rot:", std::format("{:.3f}, {:.3f}", TyPositionRotation::GetTyYawRot(), TyPositionRotation::GetTyPitchRot()));

			DrawDropShadowText(drawList, ("State: (" + std::to_string(TyState::GetTyState()) + ") " + TyStateText()).c_str());
			DrawDropShadowText(drawList, TyState::GetGroundIDName());
			std::string tyAnimText = AddSpacesBeforeCapitalAndNum(std::string(TyState::GetTyAnimationName()));
			DrawDropShadowText(drawList, ("Anim: " + tyAnimText).c_str());
			//DrawLabelWithNumbers(drawList, "Floor ID:", std::to_string(TyMemoryValues::GetTyFloorID()));
			DrawDropShadowText(drawList, ("Camera State: (" + std::to_string(Camera::GetCameraState()) + ") " + CameraStateText()).c_str());

			DrawLabelWithNumbers(drawList, "Horizontal Speed:", std::format("{:.3f}", TyMovement::GetTyHorizontalSpeed()));
			DrawLabelWithNumbers(drawList, "Vertical Speed:", std::format("{:.3f}", TyMovement::GetTyVerticalSpeed()));
		}
		else
		{
			DrawDropShadowText(drawList, "Bull:");
			Vector3 bullPos = TyPositionRotation::GetBullPos();
			DrawLabelWithNumbers(drawList, "Pos:", std::format("{:.2f}, {:.2f}, {:.2f}", bullPos.X, bullPos.Y, bullPos.Z));
			DrawLabelWithNumbers(drawList, "Rot:", std::format("{:.3f}", TyPositionRotation::GetBullRot()));

			DrawDropShadowText(drawList, ("State: (" + std::to_string(TyState::GetBullState()) + ") " + BullStateText()).c_str());
			DrawDropShadowText(drawList, TyState::GetGroundIDName());
			std::string bullAnimText = AddSpacesBeforeCapitalAndNum(std::string(TyState::GetBullAnimationName()));
			DrawDropShadowText(drawList, ("Anim: " + bullAnimText).c_str());
			DrawDropShadowText(drawList, ("Camera State: (" + std::to_string(Camera::GetCameraState()) + ") " + CameraStateText()).c_str());

			DrawLabelWithNumbers(drawList, "Horizontal Speed:", std::format("{:.3f}", TyMovement::GetBullHorizontalSpeed()));
			DrawLabelWithNumbers(drawList, "Vertical Speed:", std::format("{:.3f}", TyMovement::GetBullVerticalSpeed()));
		}
	}

	//Only set the pos on the first time loading the plugin
	ImGui::SetWindowPos(ImVec2(io.DisplaySize.x - 550, 150), ImGuiCond_FirstUseEver);
	//Auto resize to content
	ImGui::SetWindowSize(ImVec2(LongestLine * 13.71f, TextLineCount* (FontSize + 5)), ImGuiCond_Always);

	drawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void GUI::Overlay::DrawLabelWithNumbers(ImDrawList* drawList, std::string label, std::string numberText)
{
	DrawDropShadowText(drawList, label.c_str(), false);
	DrawDropShadowText(drawList, numberText.c_str(), true, ImVec2((label.length() * 13.71f), -5), TyNumberFont);

	//Work out if this line is longer than any of the other ones (needs to be done here too to check the two combined)
	int lineLength = (label.length() + numberText.length());
	if (lineLength > LongestLine)
		LongestLine = lineLength;
}

void GUI::Overlay::DrawDropShadowText(ImDrawList* drawList, const char* text, bool addNewLine, ImVec2 positionOffset, ImFont* font)
{
	//Pos
	float x = TextStartPos.x + positionOffset.x;
	float y = TextStartPos.y + (TextLineCount * (FontSize + 5)) + positionOffset.y;
	//Draw text
	drawList->AddText(font, font->FontSize, ImVec2(x + DropShadowOffset.x, y + DropShadowOffset.y), IM_COL32(10, 10, 10, 255), text); //Drop shadow text
	drawList->AddText(font, font->FontSize, ImVec2(x, y), IM_COL32(255, 255, 255, 255), text); //Normal text
	//Add New Line
	if (addNewLine)
		TextLineCount++;

	//Work out if this line is longer than any of the other ones
	int lineLength = strlen(text);
	if (lineLength > LongestLine)
		LongestLine = lineLength;
}

void GUI::Overlay::DrawPosSlotOverlay()
{
	ImGuiIO& io = ImGui::GetIO();

	//Remove all window elements
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin((TygerUtility::PluginName + " Slot Overlay").c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImDrawList* drawList = window->DrawList;
	//Set the text start pos to the window pos
	TextStartPos = window->Pos;

	//Fade in and out
	ImU32 opacity = 255;
	if (PosTextShowSeconds > 1.6f)
		opacity = ((0.4f - (PosTextShowSeconds - 1.6f)) / 0.4f) * 255;
	else if (PosTextShowSeconds < 0.4f)
		opacity = (PosTextShowSeconds / 0.4f) * 255;

	//Draw text
	const char* charText = PosSlotText.c_str();
	drawList->AddText(TyFont, TyFont->FontSize, ImVec2(TextStartPos.x + DropShadowOffset.x, TextStartPos.y + DropShadowOffset.y), IM_COL32(10, 10, 10, opacity), charText); //Drop shadow text
	drawList->AddText(TyFont, TyFont->FontSize, ImVec2(TextStartPos.x, TextStartPos.y), IM_COL32(255, 255, 255, opacity), charText); //Normal text

	//Get the text size with the Ty font
	ImGui::PushFont(TyFont);
	ImVec2 size = ImGui::CalcTextSize(charText);
	//Reset the current font back to the default font
	ImGui::PopFont();

	//Only set the pos on the first time loading the plugin
	ImGui::SetWindowPos(ImVec2(io.DisplaySize.x - (size.x + 10), io.DisplaySize.y - (size.y + 10)), ImGuiCond_Always);
	//Auto resize to content
	ImGui::SetWindowSize({size.x + DropShadowOffset.x, size.y + DropShadowOffset.y}, ImGuiCond_Always);

	drawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void GUI::Overlay::SetAndShowSlotText(std::string text, int slotNumber)
{
	//-1 used for the spawn teleport
	if (slotNumber != -1)
		GUI::Overlay::PosSlotText = text + " " + std::to_string(slotNumber);
	else
		GUI::Overlay::PosSlotText = text;
	GUI::Overlay::PosTextShowSeconds = GUI::Overlay::ShowTime;
}
