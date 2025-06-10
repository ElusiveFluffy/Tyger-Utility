#include "FreecamGUI.h"
#include "GUI.h"

//Memory
#include "Camera.h"
#include "TyState.h"

void FreecamGUI::FreeCamDrawUI()
{
	//Only runs when the checkbox state changes
	if (ImGui::Checkbox("Enable Free Cam", &EnableFreeCam)) {
		if (EnableFreeCam)
		{
			Camera::SetCameraState(Camera::FreeCam);
			//Ty states get set in the tick before game event in TygerUtility.cpp
		}
		else
		{
			Camera::SetCameraState(Camera::Default);
			//Just always reset it just incase
			*TyState::GetTyStatePtr() = 35;
			TyState::SetBullState(0);
		}
	}
	//To unlock/lock it while free cam is active
	if (ImGui::Checkbox("Lock Ty's Movement During Free Cam", &LockTyMovement)) {
		if (LockTyMovement && EnableFreeCam)
		{
			*TyState::GetTyStatePtr() = 50;
			TyState::SetBullState(-1);
		}
		else
		{
			*TyState::GetTyStatePtr() = 35;
			TyState::SetBullState(0);
		}
	}
	GUI::AddToolTip("Makes Bull invisible as they have no locked state that they can be set to\nthat doesn't turn them invisible and doesn't cause any issues");

	ImGui::Spacing();
	GUI::FloatSliderElement("Free Cam Speed", Camera::GetFreeCamSpeedPtr(), 0.1f, 25, 0.6f);
}
