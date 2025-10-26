#pragma once
#include "TyMemoryValues.h"
#include "string"

using namespace TyMemoryValues;

namespace Levels {
	inline const char* Names[24];
	inline const std::string LevelIDs[] = {
		"[Z1] ", "[Z2] ", "[Z3] ", "[Z4] ",
		"[A1] ", "[A2] ", "[A3] ", "[A4] ",
		"[B1] ", "[B2] ", "[B3] ", "[B4] ",
		"[C1] ", "[C2] ", "[C3] ", "[C4] ",
		"[D1] ", "[D2] ", "[D3] ", "[D4] ",
		"[E1] ", "[E2] ", "[E3] ", "[E4] "
	};
	inline std::string StrNames[24];
	static int GetCurrentLevelID() { return *(int*)(TyBaseAddress + 0x280594); }

	static void InitLevelNames() {
		char** levelNamesStart = (char**)(TyBaseAddress + 0x527BE8);
		
		//Get the level names from the game text, so they'll be correct for any mod that changes them
		for (int level = 0; level < 24; level++) {
			StrNames[level] = LevelIDs[level] + std::string(levelNamesStart[level]);
			Names[level] = StrNames[level].c_str();
		}
	}
};