#pragma once
#include <map>
#include <array>
#include <string>
#include "Vector3.h"
#include "imgui.h"

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

	inline bool AutoSetPosition = true;
	inline bool DontAutoUpdatePosition;

	//Position
	inline Vector3 TyBullPos;
	inline std::string PositionText = "";
	inline bool AnyChanged;

	//basically 1.0.0
	inline constexpr int FileVersionNumber = 100;
	void SavePositionsToFile();
	void LoadPositionsFromFile();
	void AdvancedTeleportPlayer(TeleportPositions::PositionValues& position);

	void PositionDrawUI();
	void SetPositionElements();
	int PositionTextBoxFilter(ImGuiInputTextCallbackData* data);
	void TeleportPosDrawUI();
}