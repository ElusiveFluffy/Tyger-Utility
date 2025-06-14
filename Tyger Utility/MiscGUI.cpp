#include "MiscGUI.h"
#include "GUI.h"

//Memory
#include "TyAttributes.h"
#include "TyState.h"

void MiscGUI::MiscDrawUI()
{
	if (ImGui::InputScalar("Charge Bite Count", ImGuiDataType_U8, &ChargeBiteCount, &GUI::IntStepAmount))
	{
		*TyAttributes::GetChargeBiteOpalCounterPtr() = ChargeBiteCount * 100;

		//Show the opal counter
		*(int*)(TyBaseAddress + 0x265488) = 1;
		//Opal counter display time
		*(int*)(TyBaseAddress + 0x26548C) = 160;
	}

	ImGui::Checkbox("Ty Invincibility", TyState::TyInvincibility());
	GUI::AddToolTip("Only affects Ty, doesn't affect Bull");

	ImGui::Checkbox("Draw Object Bounds", TyMemoryValues::DrawObjectBounds());
}