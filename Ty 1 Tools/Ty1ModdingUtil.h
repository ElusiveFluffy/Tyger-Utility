#pragma once
#include <string>
namespace Ty1ModdingUtil
{
	void TickBeforeGame(float deltaSeconds);
	void OnTyInit();
	void SaveSettings();
	void LoadSettings();

	inline std::string PluginName = "Ty 1 Modding Util";
};

