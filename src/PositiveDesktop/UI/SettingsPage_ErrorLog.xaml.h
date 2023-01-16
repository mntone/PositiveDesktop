#pragma once
#include "ViewModels/Settings/ErrorLogViewModel.h"
#include "UI/SettingsPage_ErrorLog.g.h"

namespace winrt::PositiveDesktop::UI::implementation {

	struct SettingsPage_ErrorLog: SettingsPage_ErrorLogT<SettingsPage_ErrorLog> {
		SettingsPage_ErrorLog() noexcept;

	public:  // - Properties
		ViewModels::Settings::ErrorLogsViewModel ViewModel() const noexcept {
			return viewModel_;
		}

	private:
		ViewModels::Settings::ErrorLogsViewModel viewModel_;
	};

}

namespace winrt::PositiveDesktop::UI::factory_implementation {

	struct SettingsPage_ErrorLog: SettingsPage_ErrorLogT<SettingsPage_ErrorLog, implementation::SettingsPage_ErrorLog> {
	};

}
