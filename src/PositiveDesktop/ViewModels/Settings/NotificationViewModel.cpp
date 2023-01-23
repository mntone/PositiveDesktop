#include "pch.h"
#include "NotificationViewModel.h"
#if __has_include("ViewModels/Settings/NotificationViewModel.g.cpp")
#include "ViewModels/Settings/NotificationViewModel.g.cpp"
#endif

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

inline ::app::storage::inactive_backdrop_t to_app(::winrt::Windows::Foundation::IReference<bool> config) noexcept {
	std::optional<bool> config2 { config };
	return config2.has_value()
		? (config2 ? ::app::storage::ibd_enabled : ::app::storage::ibd_disabled)
		: ::app::storage::ibd_default;
}

namespace properties {
	constexpr std::wstring_view ThemeIndex { L"ThemeIndex" };
	constexpr std::wstring_view Backdrop { L"Backdrop" };
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

using namespace app::storage;

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

NotificationViewModel::NotificationViewModel(std::shared_ptr<app::storage::DesktopConfig> config) noexcept
	: config_(std::move(config)) {
}

winrt::SettingsSavedStatus NotificationViewModel::SaveCore() {
	config_->store();
	return SettingsSavedStatus::Succeeded;
}

void NotificationViewModel::ThemeIndex(int value) noexcept {
	theme_t newValue { static_cast<theme_t>(value + 1) };
	if (config_->theme(newValue)) {
		RaisePropertyChanged(properties::ThemeIndex);
	}
}

void NotificationViewModel::Backdrop(NotificationBackdrop value) noexcept {
	backdrop_t newValue { static_cast<backdrop_t>(value) };
	if (config_->backdrop(newValue)) {
		RaisePropertyChanged(properties::Backdrop);
	}
}

winrt::IReference<bool> NotificationViewModel::InactiveBackdrop() const noexcept {
	return to_winrt(config_->inactiveBackdrop());
}

void NotificationViewModel::InactiveBackdrop(IReference<bool> const& value) noexcept {
	inactive_backdrop_t newValue { to_app(value) };
	if (config_->inactiveBackdrop(newValue)) {
		RaisePropertyChanged(properties::InactiveBackdrop);
	}
}

void NotificationViewModel::Corner(NotificationCorner value) noexcept {
	corner_t newValue { static_cast<corner_t>(value) };
	if (config_->corner(newValue)) {
		RaisePropertyChanged(properties::ThemeIndex);
	}
}

void NotificationViewModel::UseParentDuration(bool value) noexcept {
	//if (useParentDuration_ != value) {
	//	useParentDuration_ = value;
	//	RaisePropertyChanged(properties::UseParentDuration);
	//}
}

void NotificationViewModel::Duration(float value) noexcept {
	if (config_->duration(value)) {
		RaisePropertyChanged(properties::ThemeIndex);
	}
}

void NotificationViewModel::PositionOrigin(NotificationPositionOrigin value) noexcept {
	position_origin_t newValue { static_cast<position_origin_t>(value) };
	if (config_->positionOrigin(newValue)) {
		RaisePropertyChanged(properties::PositionOrigin);
	}
}

void NotificationViewModel::PositionX(float value) noexcept {
	if (config_->positionX(value)) {
		RaisePropertyChanged(properties::PositionX);
	}
}

void NotificationViewModel::PositionY(float value) noexcept {
	if (config_->positionY(value)) {
		RaisePropertyChanged(properties::PositionY);
	}
}
