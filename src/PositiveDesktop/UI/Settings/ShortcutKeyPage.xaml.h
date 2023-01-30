#pragma once
#include "ViewModels/Settings/ShortcutKeyViewModel.h"
#include "UI/Settings/ShortcutKeyPage.g.h"

namespace winrt::PositiveDesktop::UI::Settings::implementation {

	struct ShortcutKeyPage: ShortcutKeyPageT<ShortcutKeyPage> {
		ShortcutKeyPage() noexcept;

	public:  // - Properties
		ViewModels::Settings::ShortcutKeyViewModel ViewModel() const noexcept {
			return viewModel_;
		}

	private:
		ViewModels::Settings::ShortcutKeyViewModel viewModel_;
	};

}

namespace winrt::PositiveDesktop::UI::Settings::factory_implementation {

	struct ShortcutKeyPage: ShortcutKeyPageT<ShortcutKeyPage, implementation::ShortcutKeyPage> {
	};

}
