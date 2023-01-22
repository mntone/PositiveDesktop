#pragma once
#include "ViewModels/Settings/SettingsSavedEventArgs.g.h"

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct SettingsSavedEventArgs: SettingsSavedEventArgsT<SettingsSavedEventArgs> {
		SettingsSavedEventArgs(SettingsSavedStatus status) noexcept: status_(status) { }

	public:  // - Properties
		constexpr SettingsSavedStatus Status() const noexcept {
			return status_;
		}

	private:
		SettingsSavedStatus status_;
	};

}
