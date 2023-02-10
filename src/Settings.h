#pragma once

class Settings
{
public:
	[[nodiscard]] static Settings* GetSingleton();

	std::string Message(int a_playerStrength, int a_threshold) const;
	int GetPlayerStrength() const;
	int GetPlayerStrengthMax() const;
	bool HasLockBashScript(RE::TESObjectREFR* a_objRefr) const;
	RE::TESObjectREFRPtr GetTarget() const;
	void SetTarget(RE::TESObjectREFRPtr);

	std::string colorBashDisallowed{ "#FF0000" };
	std::string colorBashStandBy{ "#FFFF00" };
	std::string colorBashAllowed{ "#00FF00" };

	int bashThresholdNovice{ 350 };
	int bashThresholdApprentice{ 450 };
	int bashThresholdAdept{ 550 };

	RE::TESObjectREFRPtr crosshairTarget{ nullptr };
private:
	const wchar_t* path{ L"Data/SKSE/Plugins/Requiem - Lock Bashing Visualized.ini" };

	Settings();
};
