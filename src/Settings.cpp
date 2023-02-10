#include "Settings.h"

Settings* Settings::GetSingleton()
{
	static Settings singleton;
	return std::addressof(singleton);
}

Settings::Settings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(path);

	ini::get_value(ini, colorBashDisallowed, "Settings", "Color : Can't Bash", "; The color you'll see when your current strength isn't enough to bash the lock.\n");
	ini::get_value(ini, colorBashStandBy, "Settings", "Color : Bash Stand By", "; The color you'll see when you have the total strength to bash the lock, but you aren't ready yet.\n");
	ini::get_value(ini, colorBashAllowed, "Settings", "Color : Bash Ready", "; The color you'll see when you are ready to bash the lock.\n");

	ini::get_value(ini, bashThresholdNovice, "Settings", "Bash Threshold : Novice", "; The current amount of 2 * health + stamina to bash a novice lock.");
	ini::get_value(ini, bashThresholdApprentice, "Settings", "Bash Threshold : Apprentice", "; The current amount of 2 * health + stamina to bash a apprentice lock.");
	ini::get_value(ini, bashThresholdAdept, "Settings", "Bash Threshold : Adept", "; The current amount of 2 * health + stamina to bash a adept lock.");

	(void)ini.SaveFile(path);
}

std::string Settings::Message(int a_playerStrength, int a_threshold) const
{

	return "\nBash Status : " + std::to_string(a_playerStrength).substr(0, std::to_string(a_playerStrength).find(".")) + " / " + std::to_string(a_threshold);
}

int Settings::GetPlayerStrengthMax() const
{
	const auto player = RE::PlayerCharacter::GetSingleton();
	return static_cast<int>(2 * player->GetPermanentActorValue(RE::ActorValue::kHealth) + player->GetPermanentActorValue(RE::ActorValue::kStamina));
}

int Settings::GetPlayerStrength() const
{
	const auto player = RE::PlayerCharacter::GetSingleton();
	return static_cast<int>(2 * player->GetActorValue(RE::ActorValue::kHealth) + player->GetActorValue(RE::ActorValue::kStamina));
}

bool Settings::HasLockBashScript(RE::TESObjectREFR* a_objRefr) const
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using ObjectPtr = RE::BSTSmartPointer<RE::BSScript::Object>;
	using ArrayPtr = RE::BSTSmartPointer<RE::BSScript::Array>;
	using CallbackPtr = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>;
	using Args = RE::BSScript::IFunctionArguments;

	auto vm = VM::GetSingleton();
	auto policy = vm->GetObjectHandlePolicy();
	RE::VMHandle handle = policy->GetHandleForObject(a_objRefr->GetFormType(), a_objRefr);
	ObjectPtr object = nullptr;

	return vm->FindBoundObject(handle, "AA000XarrianLockBreakScriptNormal", object);
}

RE::TESObjectREFRPtr Settings::GetTarget() const
{
	return crosshairTarget;
}

void Settings::SetTarget(RE::TESObjectREFRPtr a_target)
{
	crosshairTarget = a_target;
}

void Settings::UpdateThresholds()
{
	bashThresholdNovice = 450;
	bashThresholdApprentice = 550;
	bashThresholdAdept = 650;
}
