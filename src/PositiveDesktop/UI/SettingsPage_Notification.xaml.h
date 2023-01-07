#pragma once
#include "ViewModels/Settings/ErrorLogViewModel.h"
#include "UI/SettingsPage_Notification.g.h"

namespace winrt::PositiveDesktop::UI::implementation {

	struct SettingsPage_Notification: SettingsPage_NotificationT<SettingsPage_Notification> {
		SettingsPage_Notification() noexcept;

	public:  // - Properties
		ViewModels::Settings::ErrorLogsViewModel ViewModel() const noexcept {
			return viewModel_;
		}

	private:
		ViewModels::Settings::ErrorLogsViewModel viewModel_;
	};

}

namespace winrt::PositiveDesktop::UI::factory_implementation {

	struct SettingsPage_Notification: SettingsPage_NotificationT<SettingsPage_Notification, implementation::SettingsPage_Notification> {
	};

}
