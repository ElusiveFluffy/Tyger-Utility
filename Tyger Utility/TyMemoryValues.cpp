#include "TyMemoryValues.h"
#include "TyAttributes.h"
#include "TygerFrameworkAPI.hpp"

void TyMemoryValues::GetBaseAddress()
{
	TyBaseAddress = (DWORD)API::Get()->param()->TyHModule;
	API::LogPluginMessage("Got Ty Base Address");
}

std::vector<UINT> TyMemoryValues::GetPointerAddresses(int baseAddress, std::vector<UINT> offsets)
{
	std::vector<UINT> addresses = {};

	for (UINT offset : offsets) {
		addresses.push_back(GetPointerAddress(baseAddress, offset));
	}

	return addresses;
}

UINT TyMemoryValues::GetPointerAddress(int baseAddress, UINT offset)
{
	//Use the address to get a pointer then add the offset to that pointer
	return *(UINT*)(baseAddress) + offset;
}
