#include "pch.h"
#include "SettingsWindow.xaml.h"
#if __has_include("UI/SettingsWindow.g.cpp")
#include "UI/SettingsWindow.g.cpp"
#endif

#include "SettingsPage_ErrorLog.xaml.h"

#include <winrt/Mntone.AngelUmbrella.Navigation.h>
#include <winrt/Microsoft.UI.Windowing.h>

#include "Helpers/UIHelper.h"
#include "Helpers/WindowHelper.h"

namespace resources {
	constexpr std::wstring_view WindowButtonForegroundColor { L"WindowButtonForegroundColor" };
	constexpr std::wstring_view WindowButtonHoverBackgroundColor { L"WindowButtonHoverBackgroundColor" };
	constexpr std::wstring_view WindowButtonPressedBackgroundColor { L"WindowButtonPressedBackgroundColor" };
	constexpr std::wstring_view WindowButtonPressedForegroundColor { L"WindowButtonPressedForegroundColor" };
	constexpr std::wstring_view WindowButtonInactiveForegroundColor { L"WindowButtonInactiveForegroundColor" };
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

	using namespace ::winrt::Mntone::AngelUmbrella::Navigation;
}

#include "UI/SettingsPage_Notification.xaml.h"

using namespace winrt::PositiveDesktop::UI::implementation;
using namespace winrt::PositiveDesktop::UI::Helpers::implementation;

SettingsWindow::SettingsWindow() {
	NavigationProperties::Initialize();
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
	micaSupport_.trysetSystemBackdrop(*this);
	Subclass(hWnd);
}

LRESULT SettingsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	if (WM_GETMINMAXINFO == message) {
		MINMAXINFO& minmaxInfo { *reinterpret_cast<MINMAXINFO*>(lParam) };
		minmaxInfo.ptMinTrackSize.x = 360;
		minmaxInfo.ptMinTrackSize.y = 270;
		return FALSE;
	} else if (WM_CLOSE == message) {
		micaSupport_.close();
		LRESULT result = WindowBase::WndProc(hWnd, message, wParam, lParam);
		WindowBase::ReleaseSubclass(hWnd);
		return result;
	} else if (WM_THEMECHANGED == message /* high contrast changed*/
		|| WM_SETTINGCHANGE == message) {
		micaSupport_.onSystemSettingsChanged(*this);
	}
	return WindowBase::WndProc(hWnd, message, wParam, lParam);
}

void SettingsWindow::ActualThemeChanged(FrameworkElement const& sender, IInspectable const& /*args*/) {
#if WINDOWSAPPSDK_RELEASE_MAJORMINOR >= 0x00010002  // Include WindowsAppSDK-VersionInfo.h
	WINRT_ASSERT(AppWindowTitleBar::IsCustomizationSupported()); // Note: Return true in Windows SDK 1.2
#else
	if (!AppWindowTitleBar::IsCustomizationSupported()) return;
#endif

	AppWindowTitleBar titlebar { AppWindow().TitleBar() };
	ResourceDictionary resources { sender.Resources() };
	titlebar.ForegroundColor(GetColor(resources, resources::WindowButtonForegroundColor));
	titlebar.ButtonForegroundColor(GetColor(resources, resources::WindowButtonForegroundColor));
	titlebar.ButtonHoverBackgroundColor(GetColor(resources, resources::WindowButtonInactiveForegroundColor));
	titlebar.ButtonPressedBackgroundColor(GetColor(resources, resources::WindowButtonPressedBackgroundColor));
	titlebar.ButtonPressedForegroundColor(GetColor(resources, resources::WindowButtonPressedForegroundColor));
	titlebar.ButtonInactiveForegroundColor(GetColor(resources, resources::WindowButtonInactiveForegroundColor));
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

void SettingsWindow::NavigationViewSelectionChanged(NavigationView const& /*sender*/, NavigationViewSelectionChangedEventArgs const& args) {
	LOG_BEGIN(app::logger::ltg_presenter);

	NavigationViewItem item { args.SelectedItem().as<NavigationViewItem>() };
	if (item) {
		TypeName pageTypeName { NavigationProperties::GetPageType(item) };
		rootFrame().Navigate(pageTypeName);
		LOG_DEBUG(std::format("Navigated to {}.", to_string(pageTypeName.Name)));
	}

	LOG_END_NOLABEL();
}
