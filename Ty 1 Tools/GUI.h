#pragma once
#include "imgui.h"
namespace GUI
{
	inline bool init;
	static float FontSize = 16;
	static ImFont* TyFont;
	static ImFont* TyNumberFont;

	void Initialize();
	void DrawUI();
	void SetImGuiStyle();
	bool ImGuiHasFocus();

	void DrawCanvas();

	void DrawListAddDropShadowText(ImVec2 position, ImVec2 Offset, ImDrawList* drawList, const char* text);
};

