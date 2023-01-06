#include "pch.h"
#include "SettingsWindow.xaml.h"
#if __has_include("SettingsWindow.g.cpp")
#include "SettingsWindow.g.cpp"
#endif

#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h>

#include "UI/UIHelper.h"
#include "UI/WindowHelper.h"
#include "SettingsPage_ErrorLog.xaml.h"

namespace app::ui::resources {

	constexpr std::wstring_view WindowButtonForegroundColor = L"WindowButtonForegroundColor";
	constexpr std::wstring_view WindowButtonHoverBackgroundColor = L"WindowButtonHoverBackgroundColor";
	constexpr std::wstring_view WindowButtonPressedBackgroundColor = L"WindowButtonPressedBackgroundColor";
	constexpr std::wstring_view WindowButtonPressedForegroundColor = L"WindowButtonPressedForegroundColor";
	constexpr std::wstring_view WindowButtonInactiveForegroundColor = L"WindowButtonInactiveForegroundColor";

}

namespace winrt {

	using namespace Microsoft::UI;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Windowing;

	using namespace Windows::Foundation::Collections;
	using namespace Windows::Globalization;
	using namespace Windows::Graphics;

}

using namespace winrt::PositiveDesktop::implementation;

SettingsWindow::SettingsWindow() {
	InitializeComponent();
	ExtendsContentIntoTitleBar(true);
	SetTitleBar(titlebar());

	winrt::Frame frame { rootFrame() };
	WINRT_ASSERT(frame);
	WINRT_ASSERT(!frame.Content());

	winrt::IVectorView<winrt::hstring> languages { winrt::ApplicationLanguages::Languages() };
	frame.Language(languages.GetAt(0));
	frame.Content(make<SettingsPage_ErrorLog>());

	HWND hWnd { GetHwnd(m_inner) };
	SetCenter(hWnd, winrt::SizeInt32 { 1200, 900 });
	Subclass(hWnd);
}

LRESULT SettingsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	if (WM_GETMINMAXINFO == message) {
		MINMAXINFO& minmaxInfo { *reinterpret_cast<MINMAXINFO*>(lParam) };
		minmaxInfo.ptMinTrackSize.x = 320;
		minmaxInfo.ptMinTrackSize.y = 240;
		return FALSE;
	} else if (WM_CLOSE == message) {
		LRESULT result = WindowBase::WndProc(hWnd, message, wParam, lParam);
		WindowBase::ReleaseSubclass(hWnd);
		return result;
	}
	return WindowBase::WndProc(hWnd, message, wParam, lParam);
}

void SettingsWindow::ActualThemeChanged(winrt::FrameworkElement const& sender, winrt::Windows::Foundation::IInspectable const& /*args*/) {
	WINRT_ASSERT(winrt::AppWindowTitleBar::IsCustomizationSupported()); // Note: Return true in Windows SDK 1.2

	winrt::AppWindow appWindow { GetAppWindow(m_inner) };
	winrt::AppWindowTitleBar titlebar { appWindow.TitleBar() };
	ResourceDictionary resources { sender.Resources() };
	titlebar.ForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonForegroundColor));
	titlebar.ButtonForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonForegroundColor));
	titlebar.ButtonHoverBackgroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonInactiveForegroundColor));
	titlebar.ButtonPressedBackgroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonPressedBackgroundColor));
	titlebar.ButtonPressedForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonPressedForegroundColor));
	titlebar.ButtonInactiveForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonInactiveForegroundColor));
}

void SettingsWindow::NavigationViewDisplayModeChanged(winrt::NavigationView const& sender, winrt::NavigationViewDisplayModeChangedEventArgs const& /*args*/) {
	winrt::Border titlebar { this->titlebar() };
	winrt::Thickness margin { titlebar.Margin() };
	if (winrt::NavigationViewDisplayMode::Minimal == sender.DisplayMode()) {
		margin.Left = 2.0 * sender.CompactPaneLength();
	} else {
		margin.Left = sender.CompactPaneLength();
	}
	titlebar.Margin(std::move(margin));

	UpdateTitlebarMargin(sender);
}

void SettingsWindow::NavigationViewPaneClosing(winrt::NavigationView const& sender, winrt::NavigationViewPaneClosingEventArgs const& /*args*/) {
	UpdateTitlebarMargin(sender);
}

void SettingsWindow::NavigationViewPaneOpening(winrt::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& /*args*/) {
	UpdateTitlebarMargin(sender);
}

constexpr int smallLeftIndent = 4;
constexpr int largeLeftIndent = 24;

void SettingsWindow::UpdateTitlebarMargin(winrt::NavigationView const& navigationView) {
	winrt::TextBlock caption { this->caption() };
	caption.TranslationTransition(Vector3Transition());

	winrt::NavigationViewDisplayMode displayMode { navigationView.DisplayMode() };
	if (winrt::NavigationViewDisplayMode::Expanded == displayMode && navigationView.IsPaneOpen()
		|| winrt::NavigationViewDisplayMode::Minimal == displayMode) {
		caption.Translation({ smallLeftIndent, 0, 0 });
	} else {
		caption.Translation({ largeLeftIndent, 0, 0 });
	}
}