#pragma once
#include "imgui.h"
#include "TyPositionRotation.h"
#include <string>

namespace GUI
{
	inline bool init;

	inline bool EnableLevelSelect;
	inline bool DisableFallDamage;
	inline bool AutoSetPosition = true;
	inline bool DontAutoUpdatePosition;
	inline bool EnableFreeCam;
	inline bool LockTyMovement = true;
	//Mostly used for the count in the UI and to easily set the amount of charge bites
	inline ImU8 ChargeBiteCount;

	inline float FontSize = 20;
	inline ImFont* TyFont;
	inline ImFont* TyNumberFont;

	inline int IntStepAmount = 1;
	inline float FloatStepAmount = 10.0f;

	inline float sliderWidth = 270;

	//Position
	inline Vector3 TyBullPos;
	inline std::string PositionText = "";
	inline bool AnyChanged;

	void AddToolTip(const char* toolTip);

	void Initialize();

	void DrawUI();
	void RangsDrawUI();
	void MovementDrawUI();
	void FloatSliderElement(std::string text, float* valuePtr, float min, float max, float defaultValue, const char* toolTipText = "");
	void PositionDrawUI();
	void SetPositionElements();
	void FreeCamDrawUI();
	void MiscDrawUI();

	void SetImGuiStyle();
	bool ImGuiWantCaptureMouse();

	int PositionTextBoxFilter(ImGuiInputTextCallbackData* data);

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