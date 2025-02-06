#pragma once
#include "TyMemoryValues.h"
#include "Levels.h"

//Structure taken from TygerMemory, thanks Matt for this
enum LevelCode {
	Z1,
	Z2,
	Z3,
	Z4,
	A1,
	A2,
	A3,
	A4,
	B1,
	B2,
	B3,
	B4,
	C1,
	C2,
	C3,
	C4,
	D1,
	D2,
	D3,
	D4,
	E1,
	E2,
	E3,
	E4,
};

enum Zone {
	ZoneZ,
	ZoneA,
	ZoneB,
	ZoneC,
	ZoneD,
	ZoneE,
};

enum FMV {
	Prologue,
	CassFindsFirstTalisman,
	SlyRevealsTyAfterTalismans,
	CassSendsSlyStopTy,
	CassRevealsPlans,
	FrogTalisman,
	CockatooTalisman,
	PlatypusTalisman,
};

enum TyAttribute {
	LearntToSwim = 0,
	LearntToDive = 1,
	GotExtraHealth = 2,
	GotSecondRang = 3,
	GotBoomerang = 4,
	GotFrostyrang = 5,
	GotFlamerang = 6,
	GotKaboomerang = 7,
	GotDoomerang = 8,
	GotMegarang = 9,
	GotZoomerang = 10,
	GotInfrarang = 11,
	GotZappyrang = 12,
	GotAquarang = 13,
	GotMultirang = 14,
	GotChronorang = 15,
};

enum Rang {
	Boomerang,
	Frostyrang,
	Flamerang,
	Kaboomerang,
	Doomerang,
	Megarang,
	Zoomerang,
	Infrarang,
	Zappyrang,
	Aquarang,
	Multirang,
	Chronorang,
};

struct LevelData {
	char TimesEntered;
	char Opals[0x26];
	char Unk1;
	bool ThunderEggs[0x8];
	bool GoldenCogs[0xA];
	bool Bilbies[0x5];
	char Unk2;
	int TimeAttackBestSeconds;
	int TimeAttackLastSeconds;
	short TriggerSaves[0x14];
};

struct ZoneData {
	bool Unlocked;
	bool Complete;
	bool BossActive;
};

struct AttributeData {
	bool LearntToSwim;
	bool LearntToDive;
	bool GotSecondRang;
	bool GotExtraHealth;
	bool GotBoomerang;
	bool GotFrostyrang;
	bool GotFlamerang;
	bool GotKaboomerang;
	bool GotDoomerang;
	bool GotMegarang;
	bool GotZoomerang;
	bool GotInfrarang;
	bool GotZappyrang;
	bool GotAquarang;
	bool GotMultirang;
	bool GotChronorang;
};

struct SaveDataStruct {
	int Size;
	int Magic;
	LevelCode SavedLevel;
	char PercentageCompletion;
	char ThunderEggCount;
	bool IsHardcore;
	bool IsDevMode;
	LevelData LevelData[24];
	ZoneData ZoneData[6];
	short Unk2;
	LevelCode CurrentLevel;
	LevelCode PreviousLevel;
	Zone CurrentZone;
	Rang CurrentRang;
	AttributeData AttributeData;
	bool Talismans[5];
	bool TalismansPlaced[5];
	short Unk3;
	short Lives;
	char PictureFrames[0x41];
	char FmvsWatched;
	int PlayTimeSeconds;
	int EnemiesBitten;
	int EnemiesZapped;
	int EnemiesFlamed;
	int CricketBatsBurnt;
	bool CheatsUsed;
	bool CheatsUsed2;
	bool ValidSpeedrun;
	bool Unk5;
	int SkinId;
};

namespace SaveData {
	typedef int(__thiscall* TyCountOpals_t)(void* properties, LevelCode levelIndex);


	inline SaveDataStruct* GetData() {
		return (SaveDataStruct*)(*(int*)(TyBaseAddress + 0x288730));
	}
	inline void SetFmvWatched(FMV fmv, bool value) {
		uint8_t fmvWatchedByte = GetData()->FmvsWatched;
		uint8_t bitMask = (1 << fmv);
		if (value)
			fmvWatchedByte |= bitMask;
		else
			fmvWatchedByte &= ~bitMask;
		GetData()->FmvsWatched = fmvWatchedByte;
	}
	inline bool GetFmvWatched(FMV fmv)
	{
		uint8_t fmvWatchedByte = GetData()->FmvsWatched;
		uint8_t bitMask = (1 << static_cast<int>(fmv));
		return (fmvWatchedByte & bitMask) != 0;
	}

	void GiveAllOpals(LevelCode level);
	void ResetAllOpals(LevelCode level);

	inline int CountLevelOpals(LevelCode level) {
		return ((TyCountOpals_t)(TyBaseAddress + 0xf7060))((void*)(TyBaseAddress + 0x2888ac), level);
	}

	int SetOpals(int opalCount, LevelCode level);

	void SetPictureFrame(int id, bool state);
	bool GetPictureFrame(int id);

	void SetAllLevelPictures(LevelCode level, bool state);
}