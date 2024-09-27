#pragma once
#include "TyMemoryValues.h"
class TyAttributes
{
public:
	static void SetAllRangs(bool newValue);

	enum Rangs {
		Two,
		Dive,
		Swim,
		Aqua,
		IronBark,
		Flame,
		Frosty,
		Zappy,
		Zoomer,
		Multi,
		Infra,
		Mega,
		Kaboom,
		Chrono,
		Doom
	};

	static bool* GetRangState(Rangs rang);

private:
	class RangOffsets {
	public:
		static constexpr int Two = 0xAB6;
		static constexpr int Dive = 0xAB5;
		static constexpr int Swim = 0xAB4;
		static constexpr int Aqua = 0xAC1;
		static constexpr int IronBark = 0xAB8;
		static constexpr int Flame = 0xABA;
		static constexpr int Frosty = 0xAB9;
		static constexpr int Zappy = 0xAC0;
		static constexpr int Zoomer = 0xABE;
		static constexpr int Multi = 0xAC2;
		static constexpr int Infra = 0xABF;
		static constexpr int Mega = 0xABD;
		static constexpr int Kaboom = 0xABB;
		static constexpr int Chrono = 0xAC3;
		static constexpr int Doom = 0xABC;
	};
	static int RangBaseAddress() { return TyMemoryValues::TyBaseAddress + 0x288730; };
};

