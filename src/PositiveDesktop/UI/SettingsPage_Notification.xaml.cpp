#include "pch.h"
#include "SettingsPage_Notification.xaml.h"
#if __has_include("UI/SettingsPage_Notification.g.cpp")
#include "UI/SettingsPage_Notification.g.cpp"
#endif

using namespace winrt::PositiveDesktop::UI::implementation;

SettingsPage_Notification::SettingsPage_Notification() noexcept
	: viewModel_(winrt::make<winrt::PositiveDesktop::ViewModels::Settings::implementation::NotificationViewModel>()) {
	InitializeComponent();
}
