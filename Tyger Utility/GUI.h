#pragma once
#include "imgui.h"
#include "TyPositionRotation.h"
#include <string>

namespace GUI
{
	inline bool init;

	inline bool EnableLevelSelect;

	inline float FontSize = 20;
	inline ImFont* TyFont;
	inline ImFont* TyNumberFont;

	inline int IntStepAmount = 1;
	inline float FloatStepAmount = 10.0f;

	inline float sliderWidth = 270;

	inline bool Hub4PluginExists;

	void AddToolTip(const char* toolTip);

	void Initialize();

	void DrawUI();
	void FloatSliderElement(std::string text, float* valuePtr, float min, float max, float defaultValue);

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

		inline static float PosTextShowSeconds = 0.0f;
		inline static std::string PosSlotText = "";

		static void DrawPosSlotOverlay();
		static void SetAndShowSlotText(std::string text, int slotNumber);

	private:
		static std::string AddSpacesBeforeCapitalAndNum(std::string text);
		//States
		static std::string TyStateText();
		static std::string BullStateText();

		static std::string CameraStateText();

		inline static float ShowTime = 2.0f;
	};

};