#include "pch.h"
#include "NotificationViewModel.h"
#if __has_include("ViewModels/Settings/NotificationViewModel.g.cpp")
#include "ViewModels/Settings/NotificationViewModel.g.cpp"
#endif

#include "App.xaml.h"

inline ::winrt::Windows::Foundation::IReference<bool> to_winrt(::app::storage::inactive_backdrop_t config) noexcept {
	switch (config) {
	case ::app::storage::ibd_disabled:
		return false;
	case ::app::storage::ibd_enabled:
		return true;
	case ::app::storage::ibd_default:
	default:
		return nullptr;
	}
}


namespace nonlocalized {
	constexpr std::string_view ErrorMessage_LoadDesktopDefaultConfig { "Failed to load desktop default config. Load fallback default config." };
}

namespace properties {
	constexpr std::wstring_view ThemeIndex { L"ThemeIndex" };
	constexpr std::wstring_view InactiveBackdrop { L"InactiveBackdrop" };
	constexpr std::wstring_view Corner { L"Corner" };
	constexpr std::wstring_view UseParentDuration { L"UseParentDuration" };
	constexpr std::wstring_view Duration { L"Duration" };
	constexpr std::wstring_view PositionOrigin { L"PositionOrigin" };
	constexpr std::wstring_view PositionX { L"PositionX" };
	constexpr std::wstring_view PositionY { L"PositionY" };
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;

	using namespace ::winrt::PositiveDesktop::ViewModels::Settings;
}

using namespace winrt::PositiveDesktop::implementation;
using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

NotificationViewModel::NotificationViewModel() noexcept
	: theme_(NotificationTheme::Default)
	, inactiveBackdrop_(nullptr)
	, corner_(NotificationCorner::Default)
	, useParentDuration_(true)
	, duration_(app::storage::kDurationDefaultFloat)
	, positionOrigin_(NotificationPositionOrigin::Default)
	, positionX_(app::storage::kPositionXDefaultFloat)
	, positionY_(app::storage::kPositionYDefaultFloat) {
	LOG_BEGIN(app::logger::ltg_viewmodel);

	com_ptr<App> appXaml { Microsoft::UI::Xaml::Application::Current().try_as<App>() };
	if (appXaml) {
		app::app_t& app { appXaml->Context() };
		Sync(app.loadDefaultDesktop());
	} else {
		LOG_ERROR(nonlocalized::ErrorMessage_LoadDesktopDefaultConfig, E_NOINTERFACE);
	}

	LOG_END_NOLABEL();
}

NotificationViewModel::NotificationViewModel(app::storage::desktop_t config) noexcept
	: theme_(static_cast<winrt::NotificationTheme>(config.theme))
	, inactiveBackdrop_(to_winrt(config.inactiveBackdrop))
	, corner_(static_cast<winrt::NotificationCorner>(config.corner))
	, useParentDuration_(config.duration != 0)
	, duration_(app::storage::actualDuration(config.duration, app::storage::kDurationDefaultFloat))
	, positionOrigin_(static_cast<winrt::NotificationPositionOrigin>(config.positionMode))
	, positionX_(app::storage::actualPosition(config.positionX, app::storage::kPositionXDefaultFloat))
	, positionY_(app::storage::actualPosition(config.positionY, app::storage::kPositionYDefaultFloat)) {
}

void NotificationViewModel::Sync(app::storage::desktop_t config) noexcept {
	theme_ = static_cast<winrt::NotificationTheme>(config.theme);
	inactiveBackdrop_ = to_winrt(config.inactiveBackdrop);
	corner_ = static_cast<winrt::NotificationCorner>(config.corner);
	useParentDuration_ = config.duration != 0;
	duration_ = app::storage::actualDuration(config.duration, app::storage::kDurationDefaultFloat);
	positionOrigin_ = static_cast<winrt::NotificationPositionOrigin>(config.positionMode);
	positionX_ = app::storage::actualPosition(config.positionX, app::storage::kPositionXDefaultFloat);
	positionY_ = app::storage::actualPosition(config.positionY, app::storage::kPositionYDefaultFloat);
}

winrt::SettingsSavedStatus NotificationViewModel::SaveCore() {
	using namespace app::storage;

	std::optional<bool> inactiveBackdrop { inactiveBackdrop_ };
	desktop_t config {
		static_cast<theme_t>(theme_),
		kBackdropDefault,
		inactiveBackdrop.has_value() ? (inactiveBackdrop ? ibd_enabled : ibd_disabled) : ibd_default,
		static_cast<corner_t>(corner_),
		useParentDuration_ ? packedDuration(duration_) : kDurationParent,
		static_cast<position_mode_t>(positionOrigin_),
		packedPosition(positionX_),
		packedPosition(positionY_),
	};

	com_ptr<App> appXaml { Microsoft::UI::Xaml::Application::Current().try_as<App>() };
	if (!appXaml) return SettingsSavedStatus::Failed;

	app::app_t& app { appXaml->Context() };
	app.store(std::move(config));
	return SettingsSavedStatus::Succeeded;
}

void NotificationViewModel::ThemeIndex(int value) noexcept {
	NotificationTheme value2 { static_cast<NotificationTheme>(value + 1) };
	if (theme_ != value2) {
		theme_ = value2;
		RaisePropertyChanged(properties::ThemeIndex);
	}
}

void NotificationViewModel::InactiveBackdrop(IReference<bool> const& value) noexcept {
	if (inactiveBackdrop_ != value) {
		inactiveBackdrop_ = value;
		RaisePropertyChanged(properties::InactiveBackdrop);
	}
}

void NotificationViewModel::Corner(NotificationCorner value) noexcept {
	if (corner_ != value) {
		corner_ = value;
		RaisePropertyChanged(properties::Corner);
	}
}

void NotificationViewModel::UseParentDuration(bool value) noexcept {
	if (useParentDuration_ != value) {
		useParentDuration_ = value;
		RaisePropertyChanged(properties::UseParentDuration);
	}
}

void NotificationViewModel::PositionOrigin(NotificationPositionOrigin value) noexcept {
	if (positionOrigin_ != value) {
		positionOrigin_ = value;
		RaisePropertyChanged(properties::PositionOrigin);
	}
}

void NotificationViewModel::Duration(float value) noexcept {
	if (duration_ != value) {
		duration_ = value;
		RaisePropertyChanged(properties::Duration);
	}
}

void NotificationViewModel::PositionX(float value) noexcept {
	if (positionX_ != value) {
		positionX_ = value;
		RaisePropertyChanged(properties::PositionX);
	}
}

void NotificationViewModel::PositionY(float value) noexcept {
	if (positionY_ != value) {
		positionY_ = value;
		RaisePropertyChanged(properties::PositionY);
	}
}
