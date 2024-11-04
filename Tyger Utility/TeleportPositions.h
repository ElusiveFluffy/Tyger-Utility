#pragma once
#include <map>
#include "Vector3.h"

namespace TeleportPositions {
	struct SpawnValues {
		Vector3 Position;
		float Rotation;
		float CameraYaw;
		float CameraPitch;
	};
	inline std::map<int, SpawnValues> SpawnPositions{};
}