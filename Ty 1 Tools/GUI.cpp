#include "GUI.h"
#include "framework.h"
#include "TyMemoryValues.h"

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

	GUI::TyFont = fonts->AddFontFromMemoryCompressedTTF(SfSlapstickComic_compressed_data, SfSlapstickComic_compressed_size, 20);
	GUI::TyNumberFont = fonts->AddFontFromMemoryCompressedTTF(TyNumberFont_compressed_data, TyNumberFont_compressed_size, GUI::FontSize);
	fonts->Build();
}

void GUI::DrawUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Only draw the overlay during gameplay
	if (TyMemoryValues::Get()->TyGameState() == Gameplay)
		DrawCanvas();

	if (API::DrawingGUI())
	{
		ImGui::Begin("Ty 1 Tools");
		ImGui::Text("Hello World");

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::ImGuiHasFocus()
{
	return ImGui::GetIO().WantCaptureMouse;
}

void GUI::DrawCanvas()
{
	ImGuiIO& io = ImGui::GetIO();

	//Remove all window elements
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("Ty 1 Tools Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImDrawList* draw_list = window->DrawList;
	ImVec2 p0 = ImVec2(50, 25);
	ImVec2 p1 = ImVec2(200, 250);
	draw_list->AddRectFilled(p0, p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(p0, p1, IM_COL32(255, 255, 255, 255));
	DrawListAddDropShadowText(ImVec2(io.DisplaySize.x - 100, io.DisplaySize.y - 30), ImVec2(3, 4), draw_list, "Test");
	DrawListAddDropShadowText(ImVec2(io.DisplaySize.x - 1000, io.DisplaySize.y - 30), ImVec2(3, 4), draw_list, "Hello");

	ImVec2 midpoint = ImVec2(500, 500);
	draw_list->AddCircle(midpoint, 30, ImColor(51, 255, 0), 0, 20);

	window->DrawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void GUI::DrawListAddDropShadowText(ImVec2 position, ImVec2 Offset, ImDrawList* drawList, const char* text)
{
	drawList->AddText(GUI::TyFont, GUI::TyFont->FontSize, ImVec2(position.x + Offset.x, position.y + Offset.y), IM_COL32(10, 10, 10, 255), text);
	drawList->AddText(GUI::TyFont, GUI::TyFont->FontSize, position, IM_COL32(255, 255, 255, 255), text);
}
