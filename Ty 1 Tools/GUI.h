#pragma once
#include "imgui.h"
#include <string>
namespace GUI
{
	inline bool init;
	static float FontSize = 20;
	static ImFont* TyFont;
	static ImFont* TyNumberFont;

	void Initialize();
	void DrawUI();
	void SetImGuiStyle();
	bool ImGuiHasFocus();

	class Overlay {
	public:
		inline static int TextLineCount;
		inline static int LongestLine;
		inline static ImVec2 TextStartPos;
		inline static ImVec2 DropShadowOffset = ImVec2(2, 3);

		static void DrawOverlay();
		static void DrawLabelWithNumbers(ImDrawList* drawList, std::string label, std::string numberText);
		static void DrawDropShadowText(ImDrawList* drawList, const char* text, bool addNewLine = true, ImVec2 positionOffset = ImVec2(0, 0), ImFont* font = GUI::TyFont);
	};
};
