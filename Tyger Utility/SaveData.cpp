#include "SaveData.h"
#include <algorithm>

void SaveData::GiveAllOpals(LevelCode level) {
	//If the level is loaded it just stores it in a int but checks the actors when saving
	if (Levels::GetCurrentLevelID() == level)
	{
		//Show the opal counter
		*(int*)(TyBaseAddress + 0x265488) = 1;
		//Opal counter display time
		*(int*)(TyBaseAddress + 0x26548C) = 160;
		*(int*)((TyBaseAddress + 0x2888b0)) = 300;
	}

	memset(&GetData()->LevelData[level].Opals, 0xFF, 0x25);
	GetData()->LevelData[level].Opals[0x25] = 0x0F;
}

void SaveData::ResetAllOpals(LevelCode level) {
	//If the level is loaded it just stores it in a int but checks the actors when saving
	if (Levels::GetCurrentLevelID() == level)
	{
		//Show the opal counter
		*(int*)(TyBaseAddress + 0x265488) = 1;
		//Opal counter display time
		*(int*)(TyBaseAddress + 0x26548C) = 160;
		*(int*)((TyBaseAddress + 0x2888b0)) = 0;
	}

	memset(&GetData()->LevelData[level].Opals, 0, 0x26);
}

int SaveData::SetOpals(int opalCount, LevelCode level) {
	opalCount = std::clamp(opalCount, 0, 300);

	//If the level is loaded it just stores it in a int but checks the actors when saving
	if (Levels::GetCurrentLevelID() == level)
	{
		//Show the opal counter
		*(int*)(TyBaseAddress + 0x265488) = 1;
		//Opal counter display time
		*(int*)(TyBaseAddress + 0x26548C) = 160;
		return *(int*)((TyBaseAddress + 0x2888b0)) = opalCount;
	}

	int currentCount = CountLevelOpals(level);
	if (currentCount == opalCount)
		return currentCount;

	int difference = opalCount - currentCount;

	char* opalsStart = GetData()->LevelData[level].Opals;

	//Remove opals
	if (difference < 0) {
		for (int i = 0; i < 0x26; i++) {
			for (int b = 0; b < 8; b++) {
				int bitMask = (1 << b);
				if ((opalsStart[i] & bitMask) == 0)
					continue;

				//If the bit is set
				opalsStart[i] &= ~bitMask;
				difference += 1;

				//Return once enough bits have been set
				if (difference == 0)
					return CountLevelOpals(level);
			}
		}
	}
	//Add opals
	else {
		for (int i = 0; i < 0x26; i++) {
			for (int b = 0; b < 8; b++) {
				int bitMask = (1 << b);
				if ((opalsStart[i] & bitMask) != 0)
					continue;

				//If the bit is set
				opalsStart[i] |= bitMask;
				difference -= 1;

				//Return once enough bits have been set
				if (difference == 0)
					return CountLevelOpals(level);
			}
		}
	}

	return 0;
}

void SaveData::SetPictureFrame(int id, bool state)
{
	int byteOffset = id / 8;
	int bitOffset = id - byteOffset * 8;

	if (state)
		GetData()->PictureFrames[byteOffset] |= (1 << bitOffset);
	else
		GetData()->PictureFrames[byteOffset] &= ~(1 << bitOffset);
}

bool SaveData::GetPictureFrame(int id)
{
	int byteOffset = id / 8;
	int bitOffset = id - byteOffset * 8;
	return (GetData()->PictureFrames[byteOffset] & (1 << bitOffset)) != 0;
}

void SaveData::SetAllLevelPictures(LevelCode level, bool state)
{
	int* picturePointersStart = (int*)(TyBaseAddress + 0x288610) + (level * 3);
	int* pictureDataPointers = (int*)*picturePointersStart;
	for (int pointer = 0; (int)&pictureDataPointers[pointer] < *(picturePointersStart + 1); pointer++) {
		int id = pictureDataPointers[pointer];
		SaveData::SetPictureFrame(id, state);
	}
}
