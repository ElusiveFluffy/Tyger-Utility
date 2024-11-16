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
		Vector3 CameraPosition;
		float CameraYaw;
		float CameraPitch;
	};
	inline int CurrentSlot = 0;
	inline constexpr int SlotCount = 10;
	inline std::map<int, PositionValues> SpawnPositions{};
	inline std::map<int, std::array<PositionValues, SlotCount>> SavedPositions{};

	//basically 1.0.0
	inline constexpr int FileVersionNumber = 100;
	void SavePositionsToFile();
	void LoadPositionsFromFile();
	void AdvancedTeleportPlayer(TeleportPositions::PositionValues& position);

	void TeleportPosDrawUI();
}