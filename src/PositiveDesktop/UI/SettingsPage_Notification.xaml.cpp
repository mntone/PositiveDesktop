#include "pch.h"
#include "SettingsPage_Notification.xaml.h"
#if __has_include("UI/SettingsPage_Notification.g.cpp")
#include "UI/SettingsPage_Notification.g.cpp"
#endif

#include "App.xaml.h"

#include "UI/Controls/KeyVisual.xaml.h"

namespace nonlocalized {
	constexpr std::string_view ErrorMessage_LoadDesktopDefaultConfig { "Failed to load desktop default config." };
}

using namespace winrt::PositiveDesktop::implementation;
using namespace winrt::PositiveDesktop::UI::implementation;

SettingsPage_Notification::SettingsPage_Notification() noexcept
	: viewModel_(nullptr) {
	LOG_BEGIN(app::logger::ltg_presenter);

	com_ptr<App> appXaml { Microsoft::UI::Xaml::Application::Current().try_as<App>() };
	if (appXaml) {
		std::shared_ptr<app::storage::DesktopConfig> config { appXaml->Context().config()->desktop() };
		viewModel_ = winrt::make<winrt::PositiveDesktop::ViewModels::Settings::implementation::NotificationViewModel>(config);
	} else {
		LOG_FATAL(nonlocalized::ErrorMessage_LoadDesktopDefaultConfig, E_NOINTERFACE);
	}

	InitializeComponent();

	std::vector<Controls::KeyVisual> keytops;
	for (int i = 160; i != 223; ++i) {
		Controls::KeyVisual keytop;
		keytop.Key(static_cast<Windows::System::VirtualKey>(i));
		keytops.push_back(keytop);
	}
	itemsRepeater().ItemsSource(single_threaded_observable_vector(std::move(keytops)));

	LOG_END_NOLABEL();
}
