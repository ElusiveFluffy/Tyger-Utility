#include "TyMemoryValues.h"

std::shared_ptr<TyMemoryValues>& TyMemoryValues::Get()
{
	static auto instance = std::make_shared<TyMemoryValues>();
	return instance;
}

void TyMemoryValues::GetMemoryValues()
{
	//TyGameState = *(GameState*)0x00F78A6C;
}
