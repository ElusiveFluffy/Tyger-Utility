#pragma once
#include "imgui.h"

namespace MiscGUI
{
	//Mostly used for the count in the UI and to easily set the amount of charge bites
	inline ImU8 ChargeBiteCount;
	inline bool DrawObjectBounds = false;
	inline bool DrawSubObjectBounds = false;

	void MiscDrawUI();
};
