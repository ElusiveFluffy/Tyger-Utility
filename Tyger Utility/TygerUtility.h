#pragma once
#include <string>
namespace TygerUtility
{
	void TickBeforeGame(float deltaSeconds);
	void OnTyInit();
	void SaveSettings();
	void LoadSettings();

	inline std::string PluginName = "Tyger Utility";
};

