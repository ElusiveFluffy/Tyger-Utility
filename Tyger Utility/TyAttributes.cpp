#include "TyAttributes.h"
#include "GUI.h"
#include "SaveData.h"

void TyAttributes::SetAllRangs(bool newValue)
{
	auto saveData = SaveData::GetData();

	//Instantly show/hide second rang
	*(bool*)(TyBaseAddress + 0x27194C) = newValue;
	saveData->AttributeData.GotSecondRang = newValue;
	saveData->AttributeData.GotAquarang = newValue;
	saveData->AttributeData.GotFlamerang = newValue;
	saveData->AttributeData.GotFrostyrang = newValue;
	saveData->AttributeData.GotZappyrang = newValue;
	saveData->AttributeData.GotZoomerang = newValue;
	saveData->AttributeData.GotMultirang = newValue;
	saveData->AttributeData.GotInfrarang = newValue;
	saveData->AttributeData.GotMegarang = newValue;
	saveData->AttributeData.GotKaboomerang = newValue;
	saveData->AttributeData.GotChronorang = newValue;
	saveData->AttributeData.GotDoomerang = newValue;

	if (GUI::Hub4PluginExists) {
		//Smasharang
		saveData->Talismans[0] = newValue;
		//New technorang 1
		saveData->Talismans[1] = newValue;
		//New technorang 2
		saveData->Talismans[2] = newValue;
	}
}

void TyAttributes::SetElementRangs(bool newValue)
{
	auto saveData = SaveData::GetData();

	//Instantly show/hide second rang
	*(bool*)(TyBaseAddress + 0x27194C) = newValue;
	saveData->AttributeData.GotSecondRang = newValue;
	saveData->AttributeData.GotAquarang = newValue;
	saveData->AttributeData.GotBoomerang = newValue;
	saveData->AttributeData.GotFlamerang = newValue;
	saveData->AttributeData.GotFrostyrang = newValue;
	saveData->AttributeData.GotZappyrang = newValue;

	if (GUI::Hub4PluginExists) {
		//Smasharang
		saveData->Talismans[0] = newValue;
	}
}

void TyAttributes::SetTechnoRangs(bool newValue)
{
	auto saveData = SaveData::GetData();

	saveData->AttributeData.GotZoomerang = newValue;
	saveData->AttributeData.GotMultirang = newValue;
	saveData->AttributeData.GotInfrarang = newValue;
	saveData->AttributeData.GotMegarang = newValue;
	saveData->AttributeData.GotKaboomerang = newValue;
	saveData->AttributeData.GotChronorang = newValue;
	saveData->AttributeData.GotDoomerang = newValue;

	if (GUI::Hub4PluginExists) {
		//New technorang 1
		saveData->Talismans[1] = newValue;
		//New technorang 2
		saveData->Talismans[2] = newValue;
	}
}
