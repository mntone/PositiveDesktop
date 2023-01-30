#include "pch.h"
#include "ShortcutKeyPage.xaml.h"
#if __has_include("UI/Settings/ShortcutKeyPage.g.cpp")
#include "UI/Settings/ShortcutKeyPage.g.cpp"
#endif

using namespace winrt::PositiveDesktop::UI::Settings::implementation;

ShortcutKeyPage::ShortcutKeyPage() noexcept
	: viewModel_(make<ViewModels::Settings::implementation::ShortcutKeyViewModel>()) {
}
