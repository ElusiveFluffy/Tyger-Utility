#pragma once
#include <memory>

enum GameState {
	NoWindow = 0, //Maybe
	StartUp = 1,
	Unknown = 2,
	Init = 3,
	Unknown2 = 4,
	Gameplay = 5,
	Unknown3 = 6,
	Unknown4 = 7,
	TitleScreen = 8,
	LoadingScreen = 9
};
class TyMemoryValues {
public:
	GameState TyGameState() const { return *(GameState*)0x00F78A6C; };

	static std::shared_ptr<TyMemoryValues>& Get();
	static void GetMemoryValues();
};