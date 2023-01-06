#pragma once
#include "SettingsWindow.g.h"
#include "UI/WindowBase.h"

namespace winrt::PositiveDesktop::implementation {

	struct SettingsWindow: SettingsWindowT<SettingsWindow>, WindowBase {
		SettingsWindow();

		void ActualThemeChanged(winrt::Microsoft::UI::Xaml::FrameworkElement const& sender, winrt::Windows::Foundation::IInspectable const& args);
		void NavigationViewDisplayModeChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewDisplayModeChangedEventArgs const& args);
		void NavigationViewPaneClosing(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewPaneClosingEventArgs const& args);
		void NavigationViewPaneOpening(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args);

	private:
		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept override final;

	private:
		void UpdateTitlebarMargin(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& navigationView);
	};

}
