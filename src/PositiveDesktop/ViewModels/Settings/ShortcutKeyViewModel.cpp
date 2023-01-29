#include "pch.h"
#include "ShortcutKeyViewModel.h"
#if __has_include("ViewModels/Settings/ShortcutKeyViewModel.g.cpp")
#include "ViewModels/Settings/ShortcutKeyViewModel.g.cpp"
#endif

namespace winrt {
	using namespace ::winrt::Windows::Foundation;

	using namespace ::winrt::PositiveDesktop::ViewModels::Settings;
}

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

winrt::SettingsSavedStatus ShortcutKeyViewModel::SaveCore() {
	return SettingsSavedStatus::Succeeded;
}
