#include "pch.h"
#include "ShortcutKeyPage.xaml.h"
#if __has_include("UI/Settings/ShortcutKeyPage.g.cpp")
#include "UI/Settings/ShortcutKeyPage.g.cpp"
#endif

#include "App.xaml.h"

namespace nonlocalized {
	constexpr std::string_view ErrorMessage_LoadKeyConfig { "Failed to load key config." };
}

using namespace winrt::PositiveDesktop::implementation;
using namespace winrt::PositiveDesktop::UI::Settings::implementation;

ShortcutKeyPage::ShortcutKeyPage() noexcept
	: viewModel_(nullptr) {
	LOG_BEGIN(app::logger::ltg_presenter);

	com_ptr<App> appXaml { Microsoft::UI::Xaml::Application::Current().try_as<App>() };
	if (appXaml) {
		std::shared_ptr<app::storage::KeyConfig> config { appXaml->Context().config()->key() };
		viewModel_ = winrt::make<winrt::PositiveDesktop::ViewModels::Settings::implementation::ShortcutKeyViewModel>(config);
	} else {
		LOG_FATAL(nonlocalized::ErrorMessage_LoadKeyConfig, E_NOINTERFACE);
	}

	InitializeComponent();

	LOG_END_NOLABEL();
}
