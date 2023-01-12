#include "pch.h"
#include "SettingsWindow.xaml.h"
#if __has_include("SettingsWindow.g.cpp")
#include "SettingsWindow.g.cpp"
#endif

#include <winrt/Microsoft.UI.Windowing.h>

#include "UI/Helpers/NavigationHelper.h"
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
	using namespace ::winrt::Windows::Foundation::Collections;
	using namespace ::winrt::Windows::Globalization;
	using namespace ::winrt::Windows::Graphics;
	using namespace ::winrt::Windows::UI::Xaml::Interop;

	using namespace ::winrt::Microsoft::UI;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Windowing;

	using namespace ::winrt::PositiveDesktop::UI::Helpers;
}

#include "UI/SettingsPage_Notification.xaml.h"

using namespace winrt::PositiveDesktop::implementation;

SettingsWindow::SettingsWindow() {
	InitializeComponent();
	ExtendsContentIntoTitleBar(true);
	SetTitleBar(titlebar());

	Frame frame { rootFrame() };
	WINRT_ASSERT(frame);
	WINRT_ASSERT(!frame.Content());
	frame.CacheSize(2);

	IVectorView<hstring> languages { ApplicationLanguages::Languages() };
	frame.Language(languages.GetAt(0));
	frame.Content(make<PositiveDesktop::UI::implementation::SettingsPage_Notification>());

	HWND hWnd { GetHwnd(m_inner) };
	SetCenter(hWnd, SizeInt32 { 1200, 900 });
	Subclass(hWnd);
}

LRESULT SettingsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	if (WM_GETMINMAXINFO == message) {
		MINMAXINFO& minmaxInfo { *reinterpret_cast<MINMAXINFO*>(lParam) };
		minmaxInfo.ptMinTrackSize.x = 360;
		minmaxInfo.ptMinTrackSize.y = 270;
		return FALSE;
	} else if (WM_CLOSE == message) {
		LRESULT result = WindowBase::WndProc(hWnd, message, wParam, lParam);
		WindowBase::ReleaseSubclass(hWnd);
		return result;
	}
	return WindowBase::WndProc(hWnd, message, wParam, lParam);
}

void SettingsWindow::ActualThemeChanged(FrameworkElement const& sender, IInspectable const& /*args*/) {
#if WINDOWSAPPSDK_RELEASE_MAJORMINOR >= 0x00010002  // Include WindowsAppSDK-VersionInfo.h
	WINRT_ASSERT(AppWindowTitleBar::IsCustomizationSupported()); // Note: Return true in Windows SDK 1.2
#else
	if (!AppWindowTitleBar::IsCustomizationSupported()) return;
#endif

	AppWindow appWindow { GetAppWindow(m_inner) };
	AppWindowTitleBar titlebar { appWindow.TitleBar() };
	ResourceDictionary resources { sender.Resources() };
	titlebar.ForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonForegroundColor));
	titlebar.ButtonForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonForegroundColor));
	titlebar.ButtonHoverBackgroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonInactiveForegroundColor));
	titlebar.ButtonPressedBackgroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonPressedBackgroundColor));
	titlebar.ButtonPressedForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonPressedForegroundColor));
	titlebar.ButtonInactiveForegroundColor(app::ui::getColor(resources, app::ui::resources::WindowButtonInactiveForegroundColor));
}

void SettingsWindow::NavigationViewDisplayModeChanged(NavigationView const& sender, NavigationViewDisplayModeChangedEventArgs const& /*args*/) {
	Border titlebar { this->titlebar() };
	Thickness margin { titlebar.Margin() };
	if (NavigationViewDisplayMode::Minimal == sender.DisplayMode()) {
		margin.Left = 2.0 * sender.CompactPaneLength();
		navigationView().IsPaneToggleButtonVisible(true);
	} else {
		margin.Left = sender.CompactPaneLength();
		navigationView().IsPaneToggleButtonVisible(false);
	}
	titlebar.Margin(std::move(margin));

	UpdateTitlebarMargin(sender);
}

void SettingsWindow::NavigationViewPaneClosing(NavigationView const& sender, NavigationViewPaneClosingEventArgs const& /*args*/) {
	UpdateTitlebarMargin(sender);
}

void SettingsWindow::NavigationViewPaneOpening(NavigationView const& sender, IInspectable const& /*args*/) {
	UpdateTitlebarMargin(sender);
}

constexpr int smallLeftIndent = 4;
constexpr int largeLeftIndent = 24;

void SettingsWindow::UpdateTitlebarMargin(NavigationView const& navigationView) {
	TextBlock caption { this->caption() };
	caption.TranslationTransition(Vector3Transition());

	NavigationViewDisplayMode displayMode { navigationView.DisplayMode() };
	if (NavigationViewDisplayMode::Expanded == displayMode && navigationView.IsPaneOpen()
		|| NavigationViewDisplayMode::Minimal == displayMode) {
		caption.Translation({ smallLeftIndent, 0, 0 });
	} else {
		caption.Translation({ largeLeftIndent, 0, 0 });
	}
}

void SettingsWindow::NavigationViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args) {
	LOG_BEGIN(app::logger::ltg_presenter);

	NavigationViewItem item { args.SelectedItem().as<NavigationViewItem>() };
	if (item) {
		TypeName pageTypeName { NavigationHelper::GetPageType(item) };
		rootFrame().Navigate(pageTypeName);
		LOG_DEBUG(std::format("Navigated to {}.", to_string(pageTypeName.Name)));
	}

	LOG_END_NOLABEL();
}
