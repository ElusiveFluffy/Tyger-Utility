#pragma once
#include <map>
#include <string>
#include "TyMemoryValues.h"

using namespace TyMemoryValues;

namespace TyState {
	inline std::map<int, std::string> Ty{
		{-1, "Null"},
		{0, "No State"},
		{1, "Biting"},
		{2, "Sneaking"},
		{3, "Walking"},
		{5, "Running"},
		{6, "Skidding to Stop"},
		{7, "Jumping"},
		{8, "Gliding Init"},
		{9, "Gliding"},
		{10, "Ledge Grab"},
		{11, "Bouncing on Mushroom"},
		{12, "In Flower"},
		{13, "Flower Launch"},
		{14, "Doggy Paddle Moving"},
		{15, "Swimming on Surface"},
		{16, "Going Underwater"},
		{17, "Swimming Underwater"},
		{19, "Surfacing Water"},
		{20, "Knockback/Hurt Underwater"},
		{21, "Rising in Water"}, //Rises until you hit surface ?? maybe old version of state 22
		{22, "Entering Shark Cage"},
		{23, "Inside Shark Cage"},
		{24, "Exiting Shark Cage"},
		{25, "Eaten by Big Fish"},
		{26, "Falling"},
		{27, "Long Fall"}, //Includes fall damage
		{28, "Dying"},
		{30, "Respawning"},
		{31, "Pushed Back"},
		{32, "Taking Damage"},
		{33, "Collecting TE/Cog"},
		{34, "Deep Breath"},
		{35, "Idle"},
		{36, "Balancing"},
		{37, "Doggy Paddle Idle"},
		{38, "Swim Surface Idle"},
		{39, "Underwater Idle"},
		{40, "Shaking Dry"},
		{41, "The Rotator"}, //No idea, rotates ty to the left slightly???
		{42, "Swapping Rangs"},
		{43, "Collecting Second Rang"}, //Locks all movement, unescapable
		{44, "On Waterslide"},
		{45, "Ty's View"},
		{46, "Slipping"},
		{47, "Diving"},
		{48, "Bit Ground"},
		{49, "Diving & in Water"},
		{50, "Credits Lock"},
		{51, "Quicksand"},
		{52, "Rex Diving"}
	};

	inline std::map<int, std::string> Bull{
		{-1, "No State"},
		{0, "Idle"},
		{1, "Walking"},
		{2, "Running"},
		{3, "Falling"},
		{4, "Taking Damage"},
		{5, "Fall Damage"},
		{6, "Bonking"},
		{7, "Dying"},
		{8, "Respawning"},
		{9, "Charging"},
		{10, "Pulling Emu"},
		{11, "Collecting TE/Cog"},
		{12, "Jogging"}
	};

	//Seems to maybe be something that sets Ty's next state
	inline int* GetTyStatePtr() { return (int*)(TyBaseAddress + 0x271590); };
	//Slightly different from one, doesn't ever seem to be -1, and doesn't change to -1 at the start of a loading screen
	inline int GetTyState() { return *(int*)(TyBaseAddress + 0x26EE4C); };
	inline int GetBullState() { return *(int*)(TyBaseAddress + 0x254560); };

	inline bool* TyInvincibility() { return (bool*)(TyBaseAddress + 0x288A55); };

	//Ty's anim pointer is only null while in outback but it never gets read then so its fine
	inline const char* GetTyAnimationName() { return (char*)*(int*)*(int*)(TyBaseAddress + 0x271014); }
	inline const char* GetBullAnimationName() { 
		//Store the first pointer
		int firstPointer = *(int*)(TyBaseAddress + 0x2546DC);
		//Make sure its not null (Bull's pointer is null while the level intro cutscene is playing)
		if (firstPointer != NULL)
			return (char*)*(int*)firstPointer;
		//Just use the pointer for Ty's null state text
		return Ty[-1].c_str();
	}
};