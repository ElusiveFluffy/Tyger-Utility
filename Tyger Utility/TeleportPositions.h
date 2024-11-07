#pragma once
#include <map>
#include <array>
#include "Vector3.h"

namespace TeleportPositions {
	struct PositionValues {
		bool ValidSlot;
		Vector3 Position;
		float Rotation;
		int State;
		float CameraYaw;
		float CameraPitch;
	};
	inline std::map<int, PositionValues> SpawnPositions{};
	inline std::map<int, std::array<PositionValues, 8>> SavedPositions{};

	//basically 1.0.0
	inline constexpr int FileVersionNumber = 100;
	void SavePositionsToFile();
	void LoadPositionsFromFile();
	void AdvancedTeleportPlayer(TeleportPositions::PositionValues& position);
}