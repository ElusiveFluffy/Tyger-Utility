#include "GUI.h"
#include "framework.h"
#include "TygerFrameworkAPI.hpp"
#include "Ty1ModdingUtil.h"
#include <string>
#include <format>
#include <regex>

//Memory
#include "TyMemoryValues.h"
#include "TyAttributes.h"
#include "TyState.h"
#include "TyMovement.h"
#include "TyPositionRotation.h"
#include "Camera.h"
#include "Levels.h"
using namespace TyPositionRotation;

//Fonts
#include "Fonts/TyFont.hpp"
#include "Fonts/TyNumberFont.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h" //For std::string
#include "imgui_internal.h" //For Free Drawing

//WndProc to be able to interact with imgui or block any WndProc from interacting with the Ty window
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (msg == WM_KEYDOWN && (int)wParam == 'B')
		TyPositionRotation::SetTyPos({ 71.0f, 2623.0f, 209.0f });

	if (API::DrawingGUI())
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

	return false;
}

void GUI::Initialize()
{
	//Need to cast this, otherwise TygerFramework won't get the return value
	API::AddPluginWndProc((WndProcFunc)WndProc);

	//Setup ImGui Context
	ImGui::CreateContext();

	//Set the font to be the same as TygerFramework
	API::SetImGuiFont(ImGui::GetIO().Fonts);
	SetImGuiStyle();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	if (API::GetTyWindowHandle() == 0) {
		API::LogPluginMessage("Error Getting Ty Window Handle", Error);
		return;
	}
	//Setup backend
	ImGui_ImplWin32_InitForOpenGL(API::GetTyWindowHandle());
	ImGui_ImplOpenGL3_Init();

	API::LogPluginMessage("Initialized ImGui");
	GUI::init = true;
}

void GUI::SetImGuiStyle() {
	ImFontAtlas* fonts = ImGui::GetIO().Fonts;

	ImFontConfig custom_icons{};
	custom_icons.FontDataOwnedByAtlas = false;

	GUI::TyFont = fonts->AddFontFromMemoryCompressedTTF(SfSlapstickComic_compressed_data, SfSlapstickComic_compressed_size, GUI::FontSize);
	GUI::TyNumberFont = fonts->AddFontFromMemoryCompressedTTF(TyNumberFont_compressed_data, TyNumberFont_compressed_size, 28);
	fonts->Build();
}

void GUI::DrawUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Overlay::ShowOverlay)
		Overlay::DrawOverlay();

	if (API::DrawingGUI())
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(60, 420), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(390, 340), ImGuiCond_FirstUseEver);
		ImGui::Begin(Ty1ModdingUtil::PluginName.c_str());
		ImGui::Checkbox("Show Debug Overlay", &Overlay::ShowOverlay);
		AddToolTip("Tip: You can drag the overlay around to place it anywhere you want\nThe overlay only shows during gameplay");

		//Game finished initializing
		if (TyMemoryValues::GetTyGameState() > 4)
		{
			//Is only true if the check box changes value
			if (ImGui::Checkbox("Enable Level Select", &EnableLevelSelect))
				TyMemoryValues::SetLevelSelect(EnableLevelSelect);

			ImGui::Spacing();

			if (ImGui::BeginTabBar("Tool Tabs", ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem("Rangs")) {
					RangsDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Movement")) {
					MovementDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Position")) {
					PositionDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Free Cam")) {
					FreeCamDrawUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Misc")) {
					MiscDrawUI();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		else
			ImGui::Text("Game still initializing, please wait");

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::RangsDrawUI()
{
	if (ImGui::Button("Give All Rangs"))
		TyAttributes::SetAllRangs(true);
	ImGui::SameLine();
	if (ImGui::Button("Remove All Rangs"))
		TyAttributes::SetAllRangs(false);

	if (ImGui::Button("Give Element Rangs"))
		TyAttributes::SetElementRangs(true);
	ImGui::SameLine();
	if (ImGui::Button("Give Techno Rangs"))
		TyAttributes::SetTechnoRangs(true);

	if (ImGui::BeginTable("Rangs", 3)) {
		//Row 1
		ImGui::TableNextColumn(); ImGui::Checkbox("2nd Rang", TyAttributes::GetRangState(TyAttributes::Two));
		ImGui::TableNextColumn(); ImGui::Checkbox("Swim", TyAttributes::GetRangState(TyAttributes::Swim));
		ImGui::TableNextColumn(); ImGui::Checkbox("Dive", TyAttributes::GetRangState(TyAttributes::Dive));

		//Row 2
		ImGui::TableNextColumn(); ImGui::Checkbox("Boomerang", TyAttributes::GetRangState(TyAttributes::IronBark));
		ImGui::TableNextColumn(); ImGui::Checkbox("Flamerang", TyAttributes::GetRangState(TyAttributes::Flame));
		ImGui::TableNextColumn(); ImGui::Checkbox("Frostyrang", TyAttributes::GetRangState(TyAttributes::Frosty));

		//Row 3
		ImGui::TableNextColumn(); ImGui::Checkbox("Zappyrang", TyAttributes::GetRangState(TyAttributes::Zappy));
		ImGui::TableNextColumn(); ImGui::Checkbox("Aquarang", TyAttributes::GetRangState(TyAttributes::Aqua));
		ImGui::TableNextColumn(); ImGui::Checkbox("Zoomerang", TyAttributes::GetRangState(TyAttributes::Zoomer));

		//Row 4
		ImGui::TableNextColumn(); ImGui::Checkbox("Multirang", TyAttributes::GetRangState(TyAttributes::Multi));
		ImGui::TableNextColumn(); ImGui::Checkbox("Infrarang", TyAttributes::GetRangState(TyAttributes::Infra));
		ImGui::TableNextColumn(); ImGui::Checkbox("Megarang", TyAttributes::GetRangState(TyAttributes::Mega));

		//Row 5
		ImGui::TableNextColumn(); ImGui::Checkbox("Kaboomarang", TyAttributes::GetRangState(TyAttributes::Kaboom));
		ImGui::TableNextColumn(); ImGui::Checkbox("Chronorang", TyAttributes::GetRangState(TyAttributes::Chrono));
		ImGui::TableNextColumn(); ImGui::Checkbox("Doomarang", TyAttributes::GetRangState(TyAttributes::Doom));
	}
	ImGui::EndTable();
}

void GUI::MovementDrawUI()
{
	if (Levels::GetCurrentLevelID() != 10)
	{
		ImGui::Checkbox("Disable Fall Damage", &DisableFallDamage);
		AddToolTip("Also disables the long fall animation");

		ImGui::Spacing();
		if (ImGui::Button("Give Groundswim"))
			*TyState::GetTyStatePtr() = 39;

		ImGui::Spacing();
		if (ImGui::Button("Reset Glide Up/Down"))
			*TyMovement::GetGlideUpDownPtr() = 5.5f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Glide Up/Down", TyMovement::GetGlideUpDownPtr(), 20, -20);

		ImGui::Spacing();
		if (ImGui::Button("Reset Glide Speed"))
			*TyMovement::GetGlideSpeedPtr() = 7.0f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Glide Speed", TyMovement::GetGlideSpeedPtr(), 0.25f, 100);

		ImGui::Spacing();
		if (ImGui::Button("Reset Run Speed"))
			*TyMovement::GetRunSpeedPtr() = 10.0f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Run Speed", TyMovement::GetRunSpeedPtr(), 0.25f, 100);

		ImGui::Spacing();
		if (ImGui::Button("Reset Jump Height"))
			*TyMovement::GetGroundJumpHeightPtr() = 18.57417488f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Jump Height", TyMovement::GetGroundJumpHeightPtr(), 0.25f, 100);

		ImGui::Spacing();
		if (ImGui::Button("Reset Water Jump Height"))
			*TyMovement::GetWaterJumpHeightPtr() = 10.67707825f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Water Jump Height", TyMovement::GetWaterJumpHeightPtr(), 0.25f, 100);

		ImGui::Spacing();
		if (ImGui::Button("Reset Airborne Speed"))
			*TyMovement::GetAirSpeedPtr() = 10.0f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Airborne Speed", TyMovement::GetAirSpeedPtr(), 0.25f, 100);

		ImGui::Spacing();
		if (ImGui::Button("Reset Swim Surface Speed"))
			*TyMovement::GetSwimSurfaceSpeedPtr() = 6.0f;
		ImGui::SetNextItemWidth(sliderWidth);
		ImGui::SliderFloat("Swim Surface Speed", TyMovement::GetSwimSurfaceSpeedPtr(), 0.25f, 100);
	}
	else
	{
		if (ImGui::Button("Reset Bull Run Speed"))
		{
			*TyMovement::GetBullSpeedPtr() = 35.0f;
			TyMovement::SetHardcodedBullSpeed();
		}
		ImGui::SetNextItemWidth(sliderWidth);
		//Only true when the slider changes
		if (ImGui::SliderFloat("Bull Run Speed", TyMovement::GetBullSpeedPtr(), 0.25f, 200))
		{
			TyMovement::SetHardcodedBullSpeed();
		}
	}
}

int GUI::PositionTextBoxFilter(ImGuiInputTextCallbackData* data)
{
	//Only allow stuff that matches any of these characters
	if (strchr("-1234567890,. ", (char)data->EventChar))
		return 0;
	return 1;
}

std::vector<std::string> Split(const std::string str, const std::string regex_str)
{
	std::regex regexz(regex_str);
	std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
		std::sregex_token_iterator());
	return list;
}

void GUI::PositionDrawUI()
{
	ImGui::Checkbox("Auto Teleport", &AutoSetPosition);
	AddToolTip("Automatically set's Ty/Bull's position when they're edited");
	ImGui::SameLine();
	ImGui::Checkbox("Don't Auto Update Position", &DontAutoUpdatePosition);

	ImGui::InputScalar("Step Amount", ImGuiDataType_Float, &FloatStepAmount);
	AddToolTip("Sets the amount the -/+ buttons add or subtract");
	if (Levels::GetCurrentLevelID() != 10)
	{
		//Only auto update it if none have changed
		if (!AnyChanged && !DontAutoUpdatePosition)
		{
			TyBullPos = TyPositionRotation::GetTyPos();
			PositionText = std::format("{:.3f}, {:.3f}, {:.3f}", TyBullPos.X, TyBullPos.Y, TyBullPos.Z);
		}
		ImGui::Text("Ty Position (X, Y, Z):");
		SetPositionElements();
		if (ImGui::Button("Teleport") || (AutoSetPosition && AnyChanged))
		{
			auto posDelta = TyBullPos - TyPositionRotation::GetTyPos();

			Camera::SetCameraPos(Camera::GetCameraPos() + posDelta);
			TyPositionRotation::SetTyPos(TyBullPos);
			AnyChanged = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Update Position"))
			TyBullPos = TyPositionRotation::GetTyPos();
	}
	else
	{
		//Only auto update it if none have changed
		if (!AnyChanged && !DontAutoUpdatePosition)
		{
			TyBullPos = TyPositionRotation::GetBullPos();
			PositionText = std::format("{:.3f}, {:.3f}, {:.3f}", TyBullPos.X, TyBullPos.Y, TyBullPos.Z);
		}
		ImGui::Text("Bull Position (X, Y, Z):");
		SetPositionElements();
		if (ImGui::Button("Teleport") || (AutoSetPosition && AnyChanged))
		{
			TyPositionRotation::SetBullPos(TyBullPos);
			AnyChanged = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Update Position"))
			TyBullPos = TyPositionRotation::GetBullPos();
	}

}

void GUI::SetPositionElements()
{
	//To be able to copy and paste in the position
	ImGui::InputText("Position", &PositionText, ImGuiInputTextFlags_CallbackCharFilter, GUI::PositionTextBoxFilter);
	if (ImGui::IsItemDeactivated())
	{
		std::vector<std::string> positions = Split(PositionText, ",");
		if (positions.size() == 3 && PositionText != std::format("{:.3f}, {:.3f}, {:.3f}", TyBullPos.X, TyBullPos.Y, TyBullPos.Z))
		{
			TyBullPos = { std::stof(positions[0]), std::stof(positions[1]), std::stof(positions[2]) };
			AnyChanged = true;
		}
	}
	ImGui::Spacing();
	//Or just so if any have previously changed it'll keep it true
	AnyChanged = ImGui::InputScalar("X", ImGuiDataType_Float, &TyBullPos.X, &FloatStepAmount) || AnyChanged;
	AnyChanged = ImGui::InputScalar("Y", ImGuiDataType_Float, &TyBullPos.Y, &FloatStepAmount) || AnyChanged;
	AnyChanged = ImGui::InputScalar("Z", ImGuiDataType_Float, &TyBullPos.Z, &FloatStepAmount) || AnyChanged;
}

void GUI::FreeCamDrawUI()
{
	//Only runs when the checkbox state changes
	if (ImGui::Checkbox("Enable Free Cam", &EnableFreeCam)) {
		if (EnableFreeCam)
		{
			Camera::SetCameraState(Camera::FreeCam);
			if (LockTyMovement)
				*TyState::GetTyStatePtr() = 50;
		}
		else
		{
			Camera::SetCameraState(Camera::Default);
			//Just always reset it just incase
			*TyState::GetTyStatePtr() = 35;
		}
	}
	//To unlock/lock it while free cam is active
	if (ImGui::Checkbox("Lock Ty's Movement During Free Cam", &LockTyMovement)) {
		if (LockTyMovement && EnableFreeCam)
			*TyState::GetTyStatePtr() = 50;
		else
			*TyState::GetTyStatePtr() = 35;
	}

	if (ImGui::Button("Reset Free Cam Speed"))
		*Camera::GetFreeCamSpeedPtr() = 0.6f;
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat("Free Cam Speed", Camera::GetFreeCamSpeedPtr(), 0.1f, 25);
}

void GUI::MiscDrawUI()
{
	if (ImGui::InputScalar("Charge Bite Count", ImGuiDataType_U8, &ChargeBiteCount, &IntStepAmount))
		*TyAttributes::GetChargeBiteOpalCounterPtr() = ChargeBiteCount * 100;

	ImGui::Checkbox("Ty Invincibility", TyState::TyInvincibility());
}

void GUI::AddToolTip(const char* toolTip)
{
	ImGui::SameLine();
	ImGui::Text("(?)");
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(toolTip);
}

bool GUI::ImGuiWantCaptureMouse()
{
	return ImGui::GetIO().WantCaptureMouse;
}

std::string GUI::Overlay::AddSpacesBeforeCapitalAndNum(std::string text)
{
	std::string new_str = "";
	bool lastCharWasNumber = false;
	//Iterate through the characters in the string (except the last character)
	for (UINT i = 0; i < (text.length() - 1); i++) {
		new_str += text[i]; // Append the current character to the new string

		bool isNum = isdigit(text[i + 1]);
		//Check if the next character is uppercase or a number, group numbers together
		if (isupper(text[i + 1]) || (!lastCharWasNumber && isNum)) {
			//If the next character is uppercase, insert a space in the new string
			new_str += " ";
			lastCharWasNumber = isNum;
		}
	}
	//Append the last character of the input string to the new string
	new_str += text.back();
	return new_str;
}

std::string GUI::Overlay::TyStateText() {
	//If the map doesn't contain the state just return a blank string
	if (!TyState::Ty.contains(TyState::GetTyState()))
		return "";

	return TyState::Ty[TyState::GetTyState()];
}

std::string GUI::Overlay::BullStateText() {
	//If the map doesn't contain the state just return a blank string
	if (!TyState::Bull.contains(TyState::GetBullState()))
		return "";

	return TyState::Bull[TyState::GetBullState()];
}

std::string GUI::Overlay::CameraStateText()
{
	//If the map doesn't contain the state just return a blank string
	if (!Camera::StateNames.contains(Camera::GetCameraState()))
		return "";

	return Camera::StateNames[Camera::GetCameraState()];
}

void GUI::Overlay::DrawOverlay()
{
	ImGuiIO& io = ImGui::GetIO();

	//Remove all window elements
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin((Ty1ModdingUtil::PluginName + " Overlay").c_str(), nullptr, ImGuiWindowFlags_NoDecoration);

	//Only set the pos on the first time loading the plugin
	ImGui::SetWindowPos(ImVec2(io.DisplaySize.x - 550, 150), ImGuiCond_FirstUseEver);
	//Auto resize to content
	ImGui::SetWindowSize(ImVec2(LongestLine * 13.71f, TextLineCount * (FontSize + 5)), ImGuiCond_Always);
	//Reset the counts
	TextLineCount = 0;
	LongestLine = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImDrawList* drawList = window->DrawList;
	//Set the text start pos to the window pos
	TextStartPos = window->Pos;

	//Only draw this overlay elements during gameplay
	if (TyMemoryValues::GetTyGameState() == TyMemoryValues::Gameplay)
	{
		//Only Show the Values for the bull if in outback safari
		if (Levels::GetCurrentLevelID() != 10)
		{
			DrawDropShadowText(drawList, "Ty:");
			Vector3 tyPos = TyPositionRotation::GetTyPos();
			DrawLabelWithNumbers(drawList, "Pos:", std::format("{:.2f}, {:.2f}, {:.2f}", tyPos.X, tyPos.Y, tyPos.Z));
			DrawLabelWithNumbers(drawList, "Rot:", std::format("{:.3f}", TyPositionRotation::GetTyRot()));

			DrawDropShadowText(drawList, ("State: (" + std::to_string(TyState::GetTyState()) + ") " + TyStateText()).c_str());
			std::string tyAnimText = AddSpacesBeforeCapitalAndNum(std::string(TyState::GetTyAnimationName()));
			DrawDropShadowText(drawList, ("Anim: " + tyAnimText).c_str());
			//DrawLabelWithNumbers(drawList, "Floor ID:", std::to_string(TyMemoryValues::GetTyFloorID()));
			DrawDropShadowText(drawList, ("Camera State: (" + std::to_string(Camera::GetCameraState()) + ") " + CameraStateText()).c_str());

			DrawLabelWithNumbers(drawList, "Horizontal Speed:", std::format("{:.3f}", TyMovement::GetTyHorizontalSpeed()));
			DrawLabelWithNumbers(drawList, "Vertical Speed:", std::format("{:.3f}", TyMovement::GetTyVerticalSpeed()));
		}
		else
		{
			DrawDropShadowText(drawList, "Bull:");
			Vector3 bullPos = TyPositionRotation::GetBullPos();
			DrawLabelWithNumbers(drawList, "Pos:", std::format("{:.2f}, {:.2f}, {:.2f}", bullPos.X, bullPos.Y, bullPos.Z));
			DrawLabelWithNumbers(drawList, "Rot:", std::format("{:.3f}", TyPositionRotation::GetBullRot()));

			DrawDropShadowText(drawList, ("State: (" + std::to_string(TyState::GetBullState()) + ") " + BullStateText()).c_str());
			std::string bullAnimText = AddSpacesBeforeCapitalAndNum(std::string(TyState::GetBullAnimationName()));
			DrawDropShadowText(drawList, ("Anim: " + bullAnimText).c_str());
			DrawDropShadowText(drawList, ("Camera State: (" + std::to_string(Camera::GetCameraState()) + ") " + CameraStateText()).c_str());

			DrawLabelWithNumbers(drawList, "Horizontal Speed:", std::format("{:.3f}", TyMovement::GetBullHorizontalSpeed()));
			DrawLabelWithNumbers(drawList, "Vertical Speed:", std::format("{:.3f}", TyMovement::GetBullVerticalSpeed()));
		}
	}

	drawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void GUI::Overlay::DrawLabelWithNumbers(ImDrawList* drawList, std::string label, std::string numberText)
{
	DrawDropShadowText(drawList, label.c_str(), false);
	DrawDropShadowText(drawList, numberText.c_str(), true, ImVec2((label.length() * 13.71f), -5), TyNumberFont);

	//Work out if this line is longer than any of the other ones (needs to be done here too to check the two combined)
	int lineLength = (label.length() + numberText.length());
	if (lineLength > LongestLine)
		LongestLine = lineLength;
}

void GUI::Overlay::DrawDropShadowText(ImDrawList* drawList, const char* text, bool addNewLine, ImVec2 positionOffset, ImFont* font)
{
	//Pos
	float x = TextStartPos.x + positionOffset.x;
	float y = TextStartPos.y + (TextLineCount * (FontSize + 5)) + positionOffset.y;
	//Draw text
	drawList->AddText(font, font->FontSize, ImVec2(x + DropShadowOffset.x, y + DropShadowOffset.y), IM_COL32(10, 10, 10, 255), text); //Drop shadow text
	drawList->AddText(font, font->FontSize, ImVec2(x, y), IM_COL32(255, 255, 255, 255), text); //Normal text
	//Add New Line
	if (addNewLine)
		TextLineCount++;

	//Work out if this line is longer than any of the other ones
	int lineLength = strlen(text);
	if (lineLength > LongestLine)
		LongestLine = lineLength;
}
