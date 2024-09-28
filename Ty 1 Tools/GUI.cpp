#include "GUI.h"
#include "framework.h"
#include "TyMemoryValues.h"
#include "TyAttributes.h"
#include <string>
#include <format>

//Fonts
#include "Fonts/TyFont.hpp"
#include "Fonts/TyNumberFont.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h" //For Free Drawing

#include "TygerFrameworkAPI.hpp"

//WndProc to be able to interact with imgui or block any WndProc from interacting with the Ty window
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (msg == WM_KEYDOWN && (int)wParam == 'B')
		TyMemoryValues::SetTyPos({ 71.0f, 2623.0f, 209.0f });

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
		ImGui::SetNextWindowSize(ImVec2(285, 240), ImGuiCond_FirstUseEver);
		ImGui::Begin("Ty 1 Tools");
		ImGui::Checkbox("Show Ty Location Overlay", &Overlay::ShowOverlay);
		ImGui::SameLine();
		ImGui::Text("(?)");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Tip: You can drag the overlay around to place it anywhere you want");

		//Is only true if the check box changes value
		if (ImGui::Checkbox("Enable Level Select", &EnableLevelSelect))
			TyMemoryValues::SetLevelSelect(EnableLevelSelect);

		ImGui::Spacing();

		if (ImGui::BeginTabBar("Tool Tabs", ImGuiTabBarFlags_Reorderable)) {
			if (ImGui::BeginTabItem("Rangs")) {
				RangsDrawUI();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

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

	if (ImGui::BeginTable("Rangs", 3)) {
		//Row 1
		ImGui::Checkbox("2nd Rang", TyAttributes::GetRangState(TyAttributes::Two));
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

	//ImGui::Columns(3, NULL);
	////Row 1
	//ImGui::Checkbox("2nd Rang", TyAttributes::GetRangState(TyAttributes::Two));
	//ImGui::NextColumn(); ImGui::Checkbox("Swim", TyAttributes::GetRangState(TyAttributes::Swim));
	//ImGui::NextColumn(); ImGui::Checkbox("Dive", TyAttributes::GetRangState(TyAttributes::Dive));
	//
	////Row 2
	//ImGui::Separator(); 
	//ImGui::NextColumn(); ImGui::Checkbox("Boomerang", TyAttributes::GetRangState(TyAttributes::IronBark));
	//ImGui::NextColumn(); ImGui::Checkbox("Flamerang", TyAttributes::GetRangState(TyAttributes::Flame));
	//ImGui::NextColumn(); ImGui::Checkbox("Frostyrang", TyAttributes::GetRangState(TyAttributes::Frosty));
	//
	////Row 3
	//ImGui::Separator(); 
	//ImGui::NextColumn(); ImGui::Checkbox("Zappyrang", TyAttributes::GetRangState(TyAttributes::Zappy));
	//ImGui::NextColumn(); ImGui::Checkbox("Aquarang", TyAttributes::GetRangState(TyAttributes::Aqua));
	//ImGui::NextColumn(); ImGui::Checkbox("Zoomerang", TyAttributes::GetRangState(TyAttributes::Zoomer));
	//
	////Row 4
	//ImGui::Separator(); 
	//ImGui::NextColumn(); ImGui::Checkbox("Multirang", TyAttributes::GetRangState(TyAttributes::Multi));
	//ImGui::NextColumn(); ImGui::Checkbox("Infrarang", TyAttributes::GetRangState(TyAttributes::Infra));
	//ImGui::NextColumn(); ImGui::Checkbox("Megarang", TyAttributes::GetRangState(TyAttributes::Mega));
	//
	////Row 5
	//ImGui::Separator(); 
	//ImGui::NextColumn(); ImGui::Checkbox("Kaboomarang", TyAttributes::GetRangState(TyAttributes::Kaboom));
	//ImGui::NextColumn(); ImGui::Checkbox("Chronorang", TyAttributes::GetRangState(TyAttributes::Chrono));
	//ImGui::NextColumn(); ImGui::Checkbox("Doomarang", TyAttributes::GetRangState(TyAttributes::Doom));
}

bool GUI::ImGuiHasFocus()
{
	return ImGui::GetIO().WantCaptureMouse;
}

void GUI::Overlay::DrawOverlay()
{
	ImGuiIO& io = ImGui::GetIO();

	//Remove all window elements
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("Ty 1 Tools Overlay", nullptr, ImGuiWindowFlags_NoDecoration);

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
	if (TyMemoryValues::GetTyGameState() == Gameplay)
	{
		DrawDropShadowText(drawList, "Ty:");
		TyMemoryValues::Vector3 tyPos = TyMemoryValues::GetTyPos();
		DrawLabelWithNumbers(drawList, "Pos:", std::format("{:.2f}, {:.2f}, {:.2f}", tyPos.X, tyPos.Y, tyPos.Z));
		DrawLabelWithNumbers(drawList, "Rot:", std::format("{:.3f}", TyMemoryValues::GetTyRot()));
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
