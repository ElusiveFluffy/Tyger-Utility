#include "TeleportPositions.h"
#include "TygerUtility.h"
#include "TygerFrameworkAPI.hpp"

//Ty Memory
#include "TyState.h"
#include "TyPositionRotation.h"
#include "Camera.h"

using namespace TyPositionRotation;

void TeleportPositions::SavePositionsToFile()
{
	FILE* positionFile;
	auto error = fopen_s(&positionFile, (API::GetPluginDirectory() / (TygerUtility::PluginName + " Saved Positions.TyPos")).string().c_str(), "wb");
	if (positionFile == NULL) {
		API::LogPluginMessage("Failed to Create Saved Positions File, with error code: " + std::to_string(error), Error);
		return;
	}
	//Just in case the file ever changes
	fwrite(&FileVersionNumber, sizeof(FileVersionNumber), 1, positionFile);
	for (auto&& [levelID, positions] : SavedPositions) {
		//Write the level ID so its know what level the positions are for
		fwrite(&levelID, sizeof(levelID), 1, positionFile);
		//Write the positions
		fwrite(&positions, sizeof(positions), 1, positionFile);
	}
	fclose(positionFile);
	API::LogPluginMessage("Successfully Saved Ty/Bull Positions File");
}

void TeleportPositions::LoadPositionsFromFile()
{
	if (!std::filesystem::exists(API::GetPluginDirectory() / (TygerUtility::PluginName + " Saved Positions.TyPos")))
		return;

	FILE* positionFile;
	auto error = fopen_s(&positionFile, (API::GetPluginDirectory() / (TygerUtility::PluginName + " Saved Positions.TyPos")).string().c_str(), "rb");
	if (positionFile == NULL) {
		API::LogPluginMessage("Failed to Open Saved Positions File, with error code: " + std::to_string(error), Error);
		return;
	}
	fseek(positionFile, 0, SEEK_END); // seek to end of file
	long size = ftell(positionFile); // get current file pointer
	fseek(positionFile, 0, SEEK_SET); // seek back to beginning of file
	//Only contains the version number
	if (size <= 4)
	{
		fclose(positionFile);
		return;
	}

	int levelID = 0;
	int versionNumber = 0;
	std::array<PositionValues, 8> positions{};
	fread(&versionNumber, sizeof(versionNumber), 1, positionFile);
	while (size != ftell(positionFile)) {
		fread(&levelID, sizeof(levelID), 1, positionFile);
		fread(&positions, sizeof(positions), 1, positionFile);
		SavedPositions.emplace(levelID, positions);
	}
	fclose(positionFile);
	API::LogPluginMessage("Successfully Loaded Ty/Bull Positions File");
}

void TeleportPositions::AdvancedTeleportPlayer(TeleportPositions::PositionValues& position)
{
	if (!TyState::IsBull())
	{
		SetTyPos(position.Position);
		SetTyRot(position.Rotation);
		*TyState::GetTyStatePtr() = position.State;
	}
	else
	{
		SetBullPos(position.Position);
		SetBullRot(position.Rotation);
		TyState::SetBullState(position.State);
	}

	Camera::SetCameraRotYaw(position.CameraYaw);
	Camera::SetCameraRotPitch(position.CameraPitch);
}
