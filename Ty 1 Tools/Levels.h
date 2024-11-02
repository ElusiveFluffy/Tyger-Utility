#pragma once
#include "TyMemoryValues.h"

using namespace TyMemoryValues;

namespace Levels {
	static int GetCurrentLevelID() { return *(int*)(TyBaseAddress + 0x280594); }
};