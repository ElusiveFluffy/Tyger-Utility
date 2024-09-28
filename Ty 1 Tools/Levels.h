#pragma once
#include "TyMemoryValues.h"

class Levels {
public:

	static int GetCurrentLevelID() { return *(int*)(TyMemoryValues::TyBaseAddress + 0x280594); }
};