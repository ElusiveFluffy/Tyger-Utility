#pragma once
#include "TyMemoryValues.h"

using namespace TyMemoryValues;

class TyAttributes
{
public:
	static void SetAllRangs(bool newValue);
	static void SetElementRangs(bool newValue);
	static void SetTechnoRangs(bool newValue);

	static int* GetChargeBiteOpalCounterPtr() { return (int*)(TyBaseAddress + 0x2888B4); };

private:
};

