#include "pch.h"
#include "SettingsPage_ErrorLog.xaml.h"
#if __has_include("SettingsPage_ErrorLog.g.cpp")
#include "SettingsPage_ErrorLog.g.cpp"
#endif

using namespace winrt::PositiveDesktop::implementation;

SettingsPage_ErrorLog::SettingsPage_ErrorLog() noexcept
	: viewModel_(winrt::make<winrt::PositiveDesktop::ViewModels::Settings::implementation::ErrorLogsViewModel>()) {
	InitializeComponent();
}
