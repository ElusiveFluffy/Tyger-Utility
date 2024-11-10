#pragma once
#include <map>
#include <string>
#include "TyMemoryValues.h"

using namespace TyMemoryValues;

namespace TyState {
	//Taken from global.mad
	enum GroundIDFlags {
		ID_None = 0, // no material id
		ID_Wall = 1 << 0, // regular walls we can collide with
		ID_Slippery = 1 << 1, // slippery slopes
		ID_Camera_Ignore = 1 << 2, // the camera will not try to find a better view
		ID_Ice = 1 << 3, // will give ty inertia. For ice and oil
		ID_Sand = 1 << 4, // sandy floor
		ID_Enemy_Collide = 1 << 5, // enemies only check for collisions with this material
		ID_Mud = 1 << 6, // muddy surfaces
		ID_Quicksand = 1 << 7, // quicksand
		ID_Boomerang_Ignore = 1 << 8,
		ID_Lava = 1 << 9, // hot stuff. For lava and coals
		ID_Water_Blue = 1 << 10, // Blue water
		ID_Snow = 1 << 11, // snow
		ID_Wood = 1 << 12, // bridge
		ID_Tunnel = 1 << 13, // a tunnel
		ID_Inviscollide = 1 << 14, // invisible texture used for collision (like the vertical collision on the bridge)
		ID_Unused_15 = 1 << 15, // snowtop
		ID_Soft = 1 << 16,
		ID_Water_Slide = 1 << 17, // material ID for the water slide
		ID_Grass_Thin = 1 << 18, // Thin grass pattern
		ID_Grass_Thick = 1 << 19, // Dense grass pattern
		ID_Rock = 1 << 20, // Rock ground
		ID_TreeLeaves = 1 << 21, // Tree leave texture ID so leaves wobble
		ID_Jump_Camera = 1 << 22, // Enable the JumpCamera when Ty jumps off the ground with this ID applied
		ID_Normal_Cam = 1 << 23, // if the floor below ty has this ID, the camera's Y will be at it's normal height
		ID_Cam_Go_Through = 1 << 24, // if the camera collides with a poly with this ID, then it won't register a hit
		ID_Fast = 1 << 25, // used to mark that movement should be fast here
		ID_Slow = 1 << 26, // used to mark that movement should be slow here
		ID_Turnaway = 1 << 27, // used to turn ty away from walls
		ID_Metal = 1 << 28, // Metal surfaces
		ID_Ice_Slide = 1 << 29, // Waterslide, but icy
		ID_HollowWood = 1 << 30, // Hollow wood.  Makes a different sound to normal wood.
	};
	inline std::map<GroundIDFlags, std::string> GroundIDs{
		{ID_None, "None"},
		{ID_Wall, "Wall"},
		{ID_Slippery, "Slippery"},
		{ID_Camera_Ignore, "Camera Ignore"},
		{ID_Ice, "Ice"},
		{ID_Sand, "Sand"},
		{ID_Enemy_Collide, "Enemy Collide"},
		{ID_Mud, "Mud"},
		{ID_Quicksand, "Quicksand"},
		{ID_Boomerang_Ignore, "Boomerang Ignore"},
		{ID_Lava, "Lava"},
		{ID_Water_Blue, "Water"},
		{ID_Snow, "Snow"},
		{ID_Wood, "Wood"},
		{ID_Tunnel, "Tunnel"},
		{ID_Inviscollide, "Inviscollide"},
		{ID_Unused_15, "Unused_15 (Snowtop?)"},
		{ID_Soft, "Soft"},
		{ID_Water_Slide, "Water Slide"},
		{ID_Grass_Thin, "Grass Thin"},
		{ID_Grass_Thick, "Grass Thick"},
		{ID_Rock, "Rock"},
		{ID_TreeLeaves, "Tree Leaves"},
		{ID_Jump_Camera, "Jump Camera"},
		{ID_Normal_Cam, "Normal Camera"},
		{ID_Cam_Go_Through, "Cam Go Through"},
		{ID_Fast, "Fast"},
		{ID_Slow, "Slow"},
		{ID_Turnaway, "Turnaway"},
		{ID_Metal, "Metal"},
		{ID_Ice_Slide, "Ice Slide"},
		{ID_HollowWood, "Hollow Wood"}
	};
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
	inline void SetBullState(int state) { *(int*)(TyBaseAddress + 0x254560) = state; };

	inline bool* TyInvincibility() { return (bool*)(TyBaseAddress + 0x288A55); };

	inline bool IsBull() { return *(bool*)(TyBaseAddress + 0x27E544); };

	inline const char* GetTyAnimationName() {
		//Store the first pointer
		int firstPointer = *(int*)(TyBaseAddress + 0x271014);
		//Make sure its not null (Ty's pointer is null when switching from outback)
		if (firstPointer != NULL)
			return (char*)*(int*)firstPointer;
		//Just use the pointer for Ty's null state text
		return Ty[-1].c_str();
	}
	inline const char* GetBullAnimationName() { 
		//Store the first pointer
		int firstPointer = *(int*)(TyBaseAddress + 0x2546DC);
		//Make sure its not null (Bull's pointer is null while the level intro cutscene is playing)
		if (firstPointer != NULL)
			return (char*)*(int*)firstPointer;
		//Just use the pointer for Ty's null state text
		return Ty[-1].c_str();
	}

	inline std::string setFlagsText;
	inline const char* GetGroundIDName() {
		setFlagsText = "Ground ID: ";
		GroundIDFlags setFlags = IsBull() ? *(GroundIDFlags*)(TyBaseAddress + 0x27E960) //Bull's Flags
										: *(GroundIDFlags*)(TyBaseAddress + 0x2713C4); //Ty's Flags
		if (setFlags == ID_None)
			return (setFlagsText += "None").c_str();

		//Interate through all the IDs just in case 2 or more are set
		for (auto&& [id, name] : GroundIDs)
		{
			if ((setFlags & id) == 0)
				continue;

			if (setFlagsText == "Ground ID: ")
				setFlagsText += name;
			else
				setFlagsText += " | " + name;
		}
		//If none were found its probably standard (usually when its 1 << 31)
		if (setFlagsText == "Ground ID: ")
			setFlagsText += "Standard";

		return setFlagsText.c_str();
	}
};