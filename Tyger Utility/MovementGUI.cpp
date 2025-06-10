#include "MovementGUI.h"
#include "GUI.h"
using namespace GUI;

//Memory
#include "TyState.h"
#include "TyMovement.h"

void MovementGUI::MovementDrawUI()
{
	if (!TyState::IsBull())
	{
		ImGui::Checkbox("Disable Fall Damage", &DisableFallDamage);
		AddToolTip("Also disables the long fall animation");

		ImGui::Spacing();
		if (ImGui::Button("Give Groundswim"))
			*TyState::GetTyStatePtr() = 39;

		ImGui::Spacing();
		FloatSliderElement("Glide Up/Down", TyMovement::GetGlideUpDownPtr(), 20, -20, 5.5f);

		ImGui::Spacing();
		FloatSliderElement("Glide Speed", TyMovement::GetGlideSpeedPtr(), 2.0f, 100, 7.0f);

		ImGui::Spacing();
		FloatSliderElement("Run Speed", TyMovement::GetRunSpeedPtr(), 1.0f, 100, 10.0f);

		ImGui::Spacing();
		FloatSliderElement("Jump Height", TyMovement::GetGroundJumpHeightPtr(), 5.0f, 100, 18.57417488f);

		ImGui::Spacing();
		FloatSliderElement("Airborne Speed", TyMovement::GetAirSpeedPtr(), 0.25f, 100, 10.0f);

		ImGui::Spacing();
		FloatSliderElement("Swim Speed", &TyMovement::SwimSpeed, 2.5f, 100, 20.0f);

		ImGui::Spacing();
		FloatSliderElement("Swim Surface Speed", TyMovement::GetSwimSurfaceSpeedPtr(), 1.0f, 100, 6.0f);

		ImGui::Spacing();
		FloatSliderElement("Water Jump Height", TyMovement::GetWaterJumpHeightPtr(), 0.25f, 100, 10.67707825f);
	}
	else
	{
		ImGui::Text("Bull Run Speed");
		ImGui::SetNextItemWidth(GUI::sliderWidth);
		//Only true when the slider changes
		if (ImGui::SliderFloat("##Bull Run Speed", TyMovement::GetBullSpeedPtr(), 0.25f, 200))
		{
			TyMovement::SetHardcodedBullSpeed();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset##Reset Bull Run Speed"))
		{
			*TyMovement::GetBullSpeedPtr() = 35.0f;
			TyMovement::SetHardcodedBullSpeed();
		}
	}
}
