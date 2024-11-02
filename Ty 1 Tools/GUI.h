#pragma once
#include "imgui.h"
#include "TyPositionRotation.h"
#include <string>

namespace GUI
{
	inline bool init;

	inline bool EnableLevelSelect;
	inline bool DisableFallDamage;
	inline bool AutoSetPosition;
	inline bool DontAutoUpdatePosition;
	//Mostly used for the count in the UI and to easily set the amount of charge bites
	inline ImU8 ChargeBiteCount;

	inline float FontSize = 20;
	inline ImFont* TyFont;
	inline ImFont* TyNumberFont;

	inline int IntStepAmount = 1;
	inline float FloatStepAmount = 10.0f;

	//Position
	inline TyPositionRotation::Vector3 TyBullPos;
	inline bool AnyChanged;

	void AddToolTip(const char* toolTip);

	void Initialize();

	void DrawUI();
	void RangsDrawUI();
	void MovementDrawUI();
	void PositionDrawUI();
	void MiscDrawUI();

	void SetImGuiStyle();
	bool ImGuiWantCaptureMouse();

	class Overlay {
	public:
		inline static bool ShowOverlay;
		inline static int TextLineCount;
		inline static int LongestLine;
		inline static ImVec2 TextStartPos;
		inline static ImVec2 DropShadowOffset = ImVec2(2, 3);

		static void DrawOverlay();
		static void DrawLabelWithNumbers(ImDrawList* drawList, std::string label, std::string numberText);
		static void DrawDropShadowText(ImDrawList* drawList, const char* text, bool addNewLine = true, ImVec2 positionOffset = ImVec2(0, 0), ImFont* font = TyFont);

	private:
		static std::string AddSpacesBeforeCapitalAndNum(std::string text);
		//States
		static std::string TyStateText();
		static std::string BullStateText();
	};

};
