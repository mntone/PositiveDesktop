#include "pch.h"
#include "SettingsPage_ErrorLog.xaml.h"
#if __has_include("UI/SettingsPage_ErrorLog.g.cpp")
#include "UI/SettingsPage_ErrorLog.g.cpp"
#endif

using namespace winrt::PositiveDesktop::UI::implementation;

SettingsPage_ErrorLog::SettingsPage_ErrorLog() noexcept
	: viewModel_(make<::winrt::PositiveDesktop::ViewModels::Settings::implementation::ErrorLogsViewModel>()) {
	InitializeComponent();
}
