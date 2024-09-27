#include "TyAttributes.h"

void TyAttributes::SetAllRangs(bool newValue)
{
	std::vector<UINT> addresses = TyMemoryValues::GetPointerAddresses(RangBaseAddress(),
		{ RangOffsets::Dive, RangOffsets::Swim, RangOffsets::Two, RangOffsets::Aqua, RangOffsets::Frosty, RangOffsets::Flame, RangOffsets::Zappy, RangOffsets::Kaboom, RangOffsets::Doom, RangOffsets::Mega, RangOffsets::Zoomer, RangOffsets::Infra, RangOffsets::Multi, RangOffsets::Chrono });

	for (UINT address : addresses) {
		*(bool*)address = newValue;
	}
}

bool* TyAttributes::GetRangState(Rangs rang)
{
	UINT offset = 0;
	switch (rang)
	{
	case TyAttributes::Two:
		offset = RangOffsets::Two;
		break;
	case TyAttributes::Dive:
		offset = RangOffsets::Dive;
		break;
	case TyAttributes::Swim:
		offset = RangOffsets::Swim;
		break;
	case TyAttributes::Aqua:
		offset = RangOffsets::Aqua;
		break;
	case TyAttributes::IronBark:
		offset = RangOffsets::IronBark;
		break;
	case TyAttributes::Flame:
		offset = RangOffsets::Flame;
		break;
	case TyAttributes::Frosty:
		offset = RangOffsets::Frosty;
		break;
	case TyAttributes::Zappy:
		offset = RangOffsets::Zappy;
		break;
	case TyAttributes::Zoomer:
		offset = RangOffsets::Zoomer;
		break;
	case TyAttributes::Multi:
		offset = RangOffsets::Multi;
		break;
	case TyAttributes::Infra:
		offset = RangOffsets::Infra;
		break;
	case TyAttributes::Mega:
		offset = RangOffsets::Mega;
		break;
	case TyAttributes::Kaboom:
		offset = RangOffsets::Kaboom;
		break;
	case TyAttributes::Chrono:
		offset = RangOffsets::Chrono;
		break;
	case TyAttributes::Doom:
		offset = RangOffsets::Doom;
		break;
	default:
		break;
	}

	return (bool*)TyMemoryValues::GetPointerAddress(RangBaseAddress(), offset);
}
