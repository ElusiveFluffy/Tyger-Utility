#include "TyMemoryValues.h"
#include "TygerFrameworkAPI.hpp"

void TyMemoryValues::GetBaseAddress()
{
	TyBaseAddress = (DWORD)API::Get()->param()->TyHModule;
	API::LogPluginMessage("Got Ty Base Address");
}
