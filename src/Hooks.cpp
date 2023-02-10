#include "Hooks.h"
#include "Settings.h"

namespace Hooks::SendHUDMessage
{
	struct SendHUDMessage
	{
		static void thunk(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data)
		{
			const auto data = a_data ? static_cast<RE::HUDData*>(a_data) : nullptr;
			const auto crossHairRef = data ? data->crossHairRef.get() : RE::TESObjectREFRPtr();

			if (data && crossHairRef) {
				const auto settings = Settings::GetSingleton();
				std::string currentText = data->text.c_str();

				if (settings->HasLockBashScript(crossHairRef.get())) {
					std::string coloredString{ colored_font };
					if (currentText.contains("Novice")) {
						if (settings->GetPlayerStrength() >= settings->bashThresholdNovice) {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashAllowed);
						} else if (settings->GetPlayerStrengthMax() < settings->bashThresholdNovice) {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashDisallowed);
						} else {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashStandBy);
						}

						string::replace_first_instance(coloredString, "text", settings->Message(settings->GetPlayerStrength(), settings->bashThresholdNovice));
						currentText.append(coloredString);
						settings->SetTarget(crossHairRef);
					}

					if (currentText.contains("Apprentice")) {
						if (settings->GetPlayerStrength() >= settings->bashThresholdApprentice) {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashAllowed);
						} else if (settings->GetPlayerStrengthMax() < settings->bashThresholdApprentice) {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashDisallowed);
						} else {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashStandBy);
						}

						string::replace_first_instance(coloredString, "text", settings->Message(settings->GetPlayerStrength(), settings->bashThresholdApprentice));
						currentText.append(coloredString);
						settings->SetTarget(crossHairRef);
					}

					if (currentText.contains("Adept")) {
						if (settings->GetPlayerStrength() >= settings->bashThresholdAdept) {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashAllowed);
						} else if (settings->GetPlayerStrengthMax() < settings->bashThresholdAdept) {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashDisallowed);
						} else {
							string::replace_first_instance(coloredString, "rgba", settings->colorBashStandBy);
						}

						string::replace_first_instance(coloredString, "text", settings->Message(settings->GetPlayerStrength(), settings->bashThresholdAdept));
						currentText.append(coloredString);
						settings->SetTarget(crossHairRef);
					}
				}

				if (data->text.c_str() != currentText) {
					data->text = currentText;
				}

			} else {
				logger::info("SendHUDMessage    :    There is no data or crosshair ref");
			}
			func(a_this, a_menuName, a_type, a_data);
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static inline constexpr auto colored_font{ R"(<font color='rgba'>text</font>)"sv };
	};

	void Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(39535, 40621), OFFSET(0x289, 0x280) };
		stl::write_thunk_call<SendHUDMessage>(target.address());
		logger::info("Hook    :    Hook \"SendHUDMessage\" was installed.");
	}
}

namespace Hooks::Update
{
	struct Update
	{
		static int64_t thunk(RE::PlayerCharacter* a_player)
		{

			const auto settings = Settings::GetSingleton();
			auto crosshair = RE::CrosshairPickData::GetSingleton();

			if (settings->GetTarget() == crosshair->target.get())
			{
				a_player->UpdateCrosshairs();
			}

			return func(a_player);

		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	void Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(38102, 39058), 0x7 };
		stl::write_thunk_branch<Update>(target.address());
		logger::info("Hook    :    Hook \"Update\" was installed.");
	}
}

namespace Hooks
{
	void Install()
	{
		SendHUDMessage::Install();
		Update::Install();
	}
}
