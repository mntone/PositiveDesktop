#pragma once
#include "ViewModels/Settings/NotificationViewModel.h"
#include "UI/SettingsPage_Notification.g.h"

namespace winrt::PositiveDesktop::UI::implementation {

	struct SettingsPage_Notification: SettingsPage_NotificationT<SettingsPage_Notification> {
		SettingsPage_Notification() noexcept;

	public:  // - Properties
		ViewModels::Settings::NotificationViewModel ViewModel() const noexcept {
			return viewModel_;
		}

	private:
		ViewModels::Settings::NotificationViewModel viewModel_;
	};

}

namespace winrt::PositiveDesktop::UI::factory_implementation {

	struct SettingsPage_Notification: SettingsPage_NotificationT<SettingsPage_Notification, implementation::SettingsPage_Notification> {
	};

}
