#include "TyMemoryValues.h"
#include "TygerFrameworkAPI.hpp"

void TyMemoryValues::GetBaseAddress()
{
	TyBaseAddress = (DWORD)API::Get()->param()->tygerFrameworkModule;
}
