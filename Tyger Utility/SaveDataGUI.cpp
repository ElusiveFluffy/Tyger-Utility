#include "SaveDataGUI.h"
#include "GUI.h"
#include "SaveData.h"

//Memory
#include "TyAttributes.h"

LevelCode selectedLevel = Z1;
short opalCount = 0;
void SaveDataGUI::SaveDrawUI()
{
	auto saveData = SaveData::GetData();

	ImGui::Text("Some Things Only Update After Reloading the Level");
	ImGui::Checkbox("Hardcore", &saveData->IsHardcore);
	if (ImGui::InputScalar("Lives", ImGuiDataType_U16, &saveData->Lives, &GUI::IntStepAmount))
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
			ImGui::TableNextColumn(); if (ImGui::Checkbox("2nd Rang", &saveData->AttributeData.GotSecondRang)) *(bool*)(TyBaseAddress + 0x27194C) = saveData->AttributeData.GotSecondRang;
			ImGui::TableNextColumn(); ImGui::Checkbox("Boomerang", &saveData->AttributeData.GotBoomerang);
			ImGui::TableNextColumn(); ImGui::Checkbox("Flamerang", &saveData->AttributeData.GotFlamerang);

			//Row 2
			ImGui::TableNextColumn(); ImGui::Checkbox("Frostyrang", &saveData->AttributeData.GotFrostyrang);
			if (GUI::Hub4PluginExists)
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
			if (!GUI::Hub4PluginExists)
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("Kaboomarang", &saveData->AttributeData.GotKaboomerang);
			}
			else {
				ImGui::TableNextColumn(); ImGui::Checkbox("Tech Rang 5", &saveData->Talismans[1]);
			}
			ImGui::TableNextColumn(); ImGui::Checkbox("Chronorang", &saveData->AttributeData.GotChronorang);

			//Row 5
			if (GUI::Hub4PluginExists) {
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
		ImGui::InputScalar("Times Entered", ImGuiDataType_U8, &levelData->TimesEntered, &GUI::IntStepAmount);

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
				if (ImGui::InputScalar("Opal Count", ImGuiDataType_S16, &opalCount, &GUI::IntStepAmount))
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
		if (GUI::Hub4PluginExists) {
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