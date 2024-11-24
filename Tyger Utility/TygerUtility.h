#pragma once
#include <string>
namespace TygerUtility
{
	void TickBeforeGame(float deltaSeconds);
	void SetBullSpawnPos();
	void OnTyInit();
	void SaveSettings();
	void LoadSettings();

	//Needs to be -1 as rainbow cliffs is 0
	inline int CurrentLevel = -1;

	inline std::string PluginName = "Tyger Utility";
};

