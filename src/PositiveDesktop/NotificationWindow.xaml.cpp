#include "pch.h"
#include "NotificationWindow.xaml.h"
#if __has_include("NotificationWindow.g.cpp")
#include "NotificationWindow.g.cpp"
#endif

#include <dwmapi.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include "Common/Math.h"
#include "UI/UIHelper.h"
#include "UI/WindowHelper.h"

namespace app::ui::resources {

	constexpr std::wstring_view PositiveDesktop_NotificationWindow_ClassPointer = L"PositiveDesktop.NotificationWindow.ClassPointer";

	// High contrast resources
	constexpr std::wstring_view WindowFillColorBrush_HighContrast = L"WindowFillColorBrush_HighContrast";
	constexpr std::wstring_view WindowStrokeColorBrush_HighContrast = L"WindowStrokeColorBrush_HighContrast";

	// Windows 10 (plain) resources
	constexpr std::wstring_view Windows10WindowFillColor_Dark = L"Windows10WindowFillColor_Dark";
	constexpr std::wstring_view Windows10WindowFillColor_Light = L"Windows10WindowFillColor_Light";
	constexpr std::wstring_view Windows10WindowFillColor_Accent = L"Windows10WindowFillColor_Accent";

	constexpr std::wstring_view Windows10WindowFillColorBrush_Dark = L"Windows10WindowFillColorBrush_Dark";
	constexpr std::wstring_view Windows10WindowFillColorBrush_Light = L"Windows10WindowFillColorBrush_Light";
	constexpr std::wstring_view Windows10WindowFillColorBrush_Accent = L"Windows10WindowFillColorBrush_Accent";

	constexpr std::wstring_view Windows10WindowStrokeColorBrush_Dark = L"Windows10WindowStrokeColorBrush_Dark";
	constexpr std::wstring_view Windows10WindowStrokeColorBrush_Light = L"Windows10WindowStrokeColorBrush_Light";
	constexpr std::wstring_view Windows10WindowStrokeColorBrush_Accent = L"Windows10WindowStrokeColorBrush_Accent";

	// AcrylicWindow resources
	constexpr std::wstring_view AcrylicWindowFillTintColorBrush_Dark = L"AcrylicWindowFillTintColorBrush_Dark";
	constexpr std::wstring_view AcrylicWindowFillTintColorBrush_Light = L"AcrylicWindowFillTintColorBrush_Light";
	constexpr std::wstring_view AcrylicWindowFillTintColorBrush_Accent = L"AcrylicWindowFillTintColorBrush_Accent";

	constexpr std::wstring_view AcrylicWindowFillLuminosityOpacity_Dark = L"AcrylicWindowFillLuminosityOpacity_Dark";
	constexpr std::wstring_view AcrylicWindowFillLuminosityOpacity_Light = L"AcrylicWindowFillLuminosityOpacity_Light";
	constexpr std::wstring_view AcrylicWindowFillLuminosityOpacity_Accent = L"AcrylicWindowFillLuminosityOpacity_Accent";

	constexpr std::wstring_view AcrylicWindowStrokeColorBrush_Dark = L"AcrylicWindowStrokeColorBrush_Dark";
	constexpr std::wstring_view AcrylicWindowStrokeColorBrush_Light = L"AcrylicWindowStrokeColorBrush_Light";
	constexpr std::wstring_view AcrylicWindowStrokeColorBrush_Accent = L"AcrylicWindowStrokeColorBrush_Accent";

	// Windows 11 (plain) resources
	constexpr std::wstring_view Windows11WindowFillColorBrush = L"Windows11WindowFillColorBrush";
	constexpr std::wstring_view Windows11WindowStrokeColorBrush = L"Windows11WindowStrokeColorBrush";

	// MicaWindow resources
	constexpr std::wstring_view MicaWindowStrokeColorBrush = L"MicaWindowStrokeColorBrush";

}

using namespace winrt;

using namespace Microsoft::UI;
using namespace Microsoft::UI::Composition;
using namespace Microsoft::UI::Composition::SystemBackdrops;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Foundation;
using namespace Windows::Graphics;
using namespace Windows::UI;

using namespace app::ui;

using namespace PositiveDesktop::implementation;

namespace muw = winrt::Microsoft::UI::Windowing;

NotificationWindow::NotificationWindow(NotificationPresenterHint hint, app::storage::desktop_t config)
	: hint_(hint)
	, config_(config)
	, configuration_(nullptr)
	, backdropController_(nullptr)
	, applyTheme_(nullptr)
	, transparencyEnabled_(false)
	, background_(nullptr)
	, border_(nullptr)
	, borderThickness_(ThicknessHelper::FromUniformLength(1))
	, viewModel_(nullptr)
	, timer_(nullptr) {
	InitializeComponent();

	HWND hWnd { GetHwnd(m_inner) };

	// Hook WndProc
	check_bool(SetPropW(hWnd, resources::PositiveDesktop_NotificationWindow_ClassPointer.data(), this)); // Set class pointer
	LONG_PTR nextWndProc = SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&NotificationWindow::WndProcStatic));
	check_bool(nextWndProc);
	nextWndProc_ = reinterpret_cast<WNDPROC>(nextWndProc);

	// Override window styles
	LONG currentStyle = GetWindowLongW(hWnd, GWL_STYLE);
	if (0 == currentStyle) {
		throw_last_error();
	}

	if (0 == (currentStyle & WS_POPUP)) {
		currentStyle |= WS_POPUP;
	}

	LONG result = SetWindowLongW(hWnd, GWL_STYLE, currentStyle);
	if (0 == result) {
		throw_last_error();
	}

	// Set window configs
	muw::AppWindow appWindow = GetAppWindow(hWnd);
	appWindow.IsShownInSwitchers(false);
	appWindow.ResizeClient(Windows::Graphics::SizeInt32 { 500, 95 });

	muw::OverlappedPresenter presenter = appWindow.Presenter().as<muw::OverlappedPresenter>();
	presenter.IsAlwaysOnTop(true);
	presenter.IsMaximizable(false);
	presenter.IsMinimizable(false);
	presenter.IsResizable(false);
	presenter.SetBorderAndTitleBar(false, false);

	// Init theme, backdrop, and etc...
	FrameworkElement rootElement = Content().as<FrameworkElement>();
	TrySetSystemBackdrop(rootElement);
	UpdateTheme(rootElement); /* PLEASE call this after calling TrySetSystemBackdrop because of using configuration_ */
	(this->*applyTheme_)(rootElement);
	if (config.corner != app::storage::cnr_default) {
		UpdateCorners();
	}
}

NotificationWindow::~NotificationWindow() {
	ReleasePrivate();
}

void NotificationWindow::ReleasePrivate() {
	HWND hWnd { GetHwnd(m_inner) };
	RemovePropW(hWnd, resources::PositiveDesktop_NotificationWindow_ClassPointer.data());

	actualThemeChangedRevoker_.revoke();
	activatedRovoker_.revoke();
	closedRovoker_.revoke();
}

void NotificationWindow::Show(float visibleDuration) {
	// TODO: Observe the WorkArea and update position.
	winrt::Microsoft::UI::Windowing::AppWindow appWindow = GetAppWindow(m_inner);
	winrt::Microsoft::UI::DisplayId primaryDisplayId = GetPrimaryDisplayId();
	winrt::Microsoft::UI::Windowing::DisplayArea displayArea = winrt::Microsoft::UI::Windowing::DisplayArea::GetFromDisplayId(primaryDisplayId);

	// Set ideal size
	RectInt32 workArea = displayArea.WorkArea();
	UIElement content = Content();
	content.Measure(app::sizeAsFloat(workArea));

	// Adjust window size
	Size expectedSize = content.DesiredSize();
	RectInt32 outerBounds = displayArea.OuterBounds();
	SizeInt32 size = appWindow.ClientSize();
	PointInt32 position = (app::size(outerBounds) - size) >> 1;
	appWindow.MoveAndResize(RectInt32 {
		position.X,
		position.Y,
		500,
		app::lceil(expectedSize.Height),
	});
	if (!appWindow.IsVisible()) {
		appWindow.Show();
	}

	if (visibleDuration > 0.f) {
		timer_ = DispatcherQueue().CreateTimer();
		timer_.Interval(std::chrono::milliseconds { std::lround(1000.f * visibleDuration) });
		timer_.IsRepeating(false);
		timer_.Tick([appWindow](auto&&, IInspectable const&) {
			appWindow.Hide();
		});
		timer_.Start();
	}
}

LRESULT NotificationWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	if (WM_THEMECHANGED == message) {
		HighContrastChanged();
	} else if (WM_SETTINGCHANGE == message) {
		SystemSettingsChanged();
	}

	if (nextWndProc_) {
		return CallWindowProcW(nextWndProc_, hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

LRESULT NotificationWindow::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	NotificationWindow* that = static_cast<NotificationWindow*>(GetPropW(hWnd, resources::PositiveDesktop_NotificationWindow_ClassPointer.data()));
	if (that) {
		return that->WndProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

inline DWM_WINDOW_CORNER_PREFERENCE GetDwmCornerFromConfigType(app::storage::corner_t value) noexcept {
	switch (value) {
	case app::storage::cnr_rounded:
		return DWMWCP_ROUND;
	case app::storage::cnr_rounded_small:
		return DWMWCP_ROUNDSMALL;
	case app::storage::cnr_squared:
		return DWMWCP_DONOTROUND;
	case app::storage::cnr_default:
	default:
		return DWMWCP_DEFAULT;
	}
}

inline app::storage::corner_t GetConfigCornerFromDwmType(DWM_WINDOW_CORNER_PREFERENCE value) noexcept {
	switch (value) {
	case DWMWCP_ROUND:
		return app::storage::cnr_rounded;
	case DWMWCP_ROUNDSMALL:
		return app::storage::cnr_rounded_small;
	case DWMWCP_DONOTROUND:
		return app::storage::cnr_squared;
	case DWMWCP_DEFAULT:
	default:
		return app::storage::cnr_default;
	}
}

void NotificationWindow::UpdateCorners() noexcept {
	if (hint_ != app::ui::NotificationPresenterHint::Windows11) return;

	HWND hWnd { GetNullableHwnd(m_inner) };
	if (!hWnd) return;

	DWM_WINDOW_CORNER_PREFERENCE preference { GetDwmCornerFromConfigType(config_.corner) };
	HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
	if (FAILED(hr)) {
		// Rollback if available
		hr = DwmGetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
		if (SUCCEEDED(hr)) {
			config_.corner = GetConfigCornerFromDwmType(preference);
		}
	}
}

inline winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme ConvertToSystemBackdropTheme(ElementTheme const& theme) {
	switch (theme) {
	case ElementTheme::Dark:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Dark;
	case ElementTheme::Light:
	default:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Light;
	}
}

void NotificationWindow::UpdateTheme(FrameworkElement rootElement) {
	ElementTheme theme;
	if (ViewManagement::AccessibilitySettings().HighContrast()) {
		theme = ElementTheme::Default;
	} else {
		switch (config_.theme) {
		case app::storage::thm_dark:
		case app::storage::thm_accent:
			theme = ElementTheme::Dark;
			break;
		case app::storage::thm_light:
			theme = ElementTheme::Light;
			break;
		case app::storage::thm_system:
		default:
		{
			DWORD preserveColor = colorPrevalence_.value();
			if (preserveColor) {
				theme = ElementTheme::Dark;
			} else {
				DWORD useLight = systemUsesLightTheme_.value();
				if (useLight) {
					theme = ElementTheme::Light;
				} else {
					theme = ElementTheme::Dark;
				}
			}
			break;
		}
		}
	}
	if (rootElement.RequestedTheme() != theme) {
		rootElement.RequestedTheme(theme);
	} else {
		(this->*applyTheme_)(rootElement);
	}
}

void NotificationWindow::TrySetSystemBackdrop(FrameworkElement rootElement) {
	applyTheme_ = nullptr;
	backdropController_ = nullptr;
	configuration_ = nullptr;
	transparencyEnabled_ = false;

	if (!ViewManagement::AccessibilitySettings().HighContrast()) {
		DWORD transparencyEnabled = enableTransparency_.value();
		if (transparencyEnabled) {
			if (MicaController::IsSupported()) {
				WINRT_ASSERT(!backdropController_);
				applyTheme_ = &NotificationWindow::ApplyThemeForMica;
				transparencyEnabled_ = true;

				SetupSystemBackdropConfiguration(rootElement);

				MicaController controller = MicaController();
				controller.SetSystemBackdropConfiguration(configuration_);
				controller.AddSystemBackdropTarget(try_as<ICompositionSupportsSystemBackdrop>());
				backdropController_ = controller;
				return;
			}

			if (DesktopAcrylicController::IsSupported()) {
				WINRT_ASSERT(!backdropController_);
				applyTheme_ = &NotificationWindow::ApplyThemeForAcrylic;
				transparencyEnabled_ = true;

				SetupSystemBackdropConfiguration(rootElement);

				DesktopAcrylicController controller = DesktopAcrylicController();
				controller.SetSystemBackdropConfiguration(configuration_);
				controller.TintColor(winrt::Microsoft::UI::Colors::Transparent());
				controller.TintOpacity(0);
				controller.AddSystemBackdropTarget(try_as<ICompositionSupportsSystemBackdrop>());
				backdropController_ = controller;
				return;
			}
		}
	}

	applyTheme_ = &NotificationWindow::ApplyThemeForPlain;
	closedRovoker_.revoke();
	activatedRovoker_.revoke();
}

void NotificationWindow::SetupSystemBackdropConfiguration(FrameworkElement rootElement) {
	WINRT_ASSERT(!activatedRovoker_);
	WINRT_ASSERT(!closedRovoker_);

	configuration_ = SystemBackdropConfiguration();
	activatedRovoker_ = Activated(auto_revoke, { this, &NotificationWindow::WindowActivated });
	closedRovoker_ = Closed(auto_revoke, { this, &NotificationWindow::WindowClosed });

	configuration_.IsInputActive(true);

	if (!rootElement) {
		rootElement = Content().as<FrameworkElement>();
	}
	actualThemeChangedRevoker_ = rootElement.ActualThemeChanged(auto_revoke, { this, &NotificationWindow::ContentThemeChanged });
}

void NotificationWindow::ApplyThemeForMica(FrameworkElement rootElement) noexcept {
	WINRT_ASSERT(!ViewManagement::AccessibilitySettings().HighContrast());
	WINRT_ASSERT(backdropController_);

	if (colorPrevalence_.value()) {
	} else {
		winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme backdropTheme = ConvertToSystemBackdropTheme(rootElement.ActualTheme());
		configuration_.Theme(backdropTheme);
	}
}

void NotificationWindow::ApplyThemeForAcrylic(FrameworkElement rootElement) noexcept {
	WINRT_ASSERT(!ViewManagement::AccessibilitySettings().HighContrast());
	WINRT_ASSERT(backdropController_);
	if (!rootElement) {
		rootElement = Content().as<FrameworkElement>();
	}

	ResourceDictionary res { rootElement.Resources() };
	Media::Brush background { nullptr }, border { nullptr };
	Color fallbackColor;
	double luminosityOpacity;
	winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme backdropTheme;
	if (colorPrevalence_.value()) {
		background = getBrush(res, resources::AcrylicWindowFillTintColorBrush_Accent);
		border = getBrush(res, resources::AcrylicWindowStrokeColorBrush_Accent);
		luminosityOpacity = getDouble(res, resources::AcrylicWindowFillLuminosityOpacity_Accent);
		fallbackColor = getColor(res, resources::Windows10WindowFillColor_Accent);
		backdropTheme = winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Dark;
	} else {
		switch (rootElement.ActualTheme()) {
		case ElementTheme::Dark:
			background = getBrush(res, resources::AcrylicWindowFillTintColorBrush_Dark);
			border = getBrush(res, resources::AcrylicWindowStrokeColorBrush_Dark);
			luminosityOpacity = getDouble(res, resources::AcrylicWindowFillLuminosityOpacity_Dark);
			fallbackColor = getColor(res, resources::Windows10WindowFillColor_Dark);
			backdropTheme = winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Dark;
			break;
		case ElementTheme::Light:
		default:
			background = getBrush(res, resources::AcrylicWindowFillTintColorBrush_Light);
			border = getBrush(res, resources::AcrylicWindowStrokeColorBrush_Light);
			luminosityOpacity = getDouble(res, resources::AcrylicWindowFillLuminosityOpacity_Light);
			fallbackColor = getColor(res, resources::Windows10WindowFillColor_Light);
			backdropTheme = winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Light;
			break;
		}
	}

	Background(background);
	Border(border);

	DesktopAcrylicController controller = backdropController_.as<DesktopAcrylicController>();
	controller.LuminosityOpacity(static_cast<float>(luminosityOpacity));
	controller.FallbackColor(fallbackColor);
	configuration_.Theme(backdropTheme);
}

void NotificationWindow::ApplyThemeForPlain(FrameworkElement rootElement) noexcept {
	WINRT_ASSERT(!backdropController_);
	if (!rootElement) {
		rootElement = Content().as<FrameworkElement>();
	}

	ResourceDictionary res { rootElement.Resources() };
	Media::Brush background { nullptr }, border { nullptr };
	if (ViewManagement::AccessibilitySettings().HighContrast()) {
		background = getBrush(res, resources::WindowFillColorBrush_HighContrast);
		border = getBrush(res, resources::WindowStrokeColorBrush_HighContrast);
	} else if (hint_ == NotificationPresenterHint::Windows11) {
		background = getBrush(res, resources::Windows11WindowFillColorBrush);
		border = getBrush(res, resources::Windows11WindowStrokeColorBrush);
	} else {
		if (colorPrevalence_.value()) {
			background = getBrush(res, resources::Windows10WindowFillColorBrush_Accent);
			border = getBrush(res, resources::Windows10WindowStrokeColorBrush_Accent);
		} else {
			switch (rootElement.ActualTheme()) {
			case ElementTheme::Dark:
				background = getBrush(res, resources::Windows10WindowFillColorBrush_Dark);
				border = getBrush(res, resources::Windows10WindowStrokeColorBrush_Dark);
				break;
			case ElementTheme::Light:
			default:
				background = getBrush(res, resources::Windows10WindowFillColorBrush_Light);
				border = getBrush(res, resources::Windows10WindowStrokeColorBrush_Light);
				break;
			}
		}
	}

	Background(background);
	Border(border);
}

#pragma region Event delegates

void NotificationWindow::SystemSettingsChanged() {
	// Force update
	systemUsesLightTheme_.reset();
	colorPrevalence_.reset();
	enableTransparency_.reset();

	FrameworkElement rootElement = Content().as<FrameworkElement>();
	if (transparencyEnabled_ != static_cast<bool>(enableTransparency_.value())) {
		TrySetSystemBackdrop(rootElement);
	}

	if (app::storage::thm_system == config_.theme) {
		ElementTheme theme;
		DWORD preserveColor = colorPrevalence_.value();
		if (preserveColor) {
			theme = ElementTheme::Dark;
		} else {
			DWORD useLight = systemUsesLightTheme_.value();
			if (useLight) {
				theme = ElementTheme::Light;
			} else {
				theme = ElementTheme::Dark;
			}
		}

		if (rootElement.RequestedTheme() != theme) {
			rootElement.RequestedTheme(theme);
		} else {
			(this->*applyTheme_)(rootElement);
		}
	}
}

void NotificationWindow::HighContrastChanged() {
	FrameworkElement rootElement = Content().as<FrameworkElement>();
	if (transparencyEnabled_ && !enableTransparency_.value()) {
		TrySetSystemBackdrop(rootElement);
	}

	UpdateTheme(rootElement);
}

void NotificationWindow::ContentThemeChanged(FrameworkElement const& sender, IInspectable const& /*args*/) {
	WINRT_ASSERT(applyTheme_);
	(this->*applyTheme_)(sender);
}

void NotificationWindow::WindowActivated(IInspectable const& /*sender*/, WindowActivatedEventArgs const& args) {
	//configuration_.IsInputActive(WindowActivationState::Deactivated != args.WindowActivationState());
}

void NotificationWindow::WindowClosed(IInspectable const& /*sender*/, WindowEventArgs const& /*args*/) {
	ReleasePrivate();
}

#pragma endregion

#pragma region Properties

void NotificationWindow::Background(Microsoft::UI::Xaml::Media::Brush const& value) noexcept {
	if (background_ != value) {
		background_ = value;
		propertyChanged_(*this, Data::PropertyChangedEventArgs { L"Background" });
	}
}

void NotificationWindow::Border(Microsoft::UI::Xaml::Media::Brush const& value) noexcept {
	if (border_ != value) {
		border_ = value;
		propertyChanged_(*this, Data::PropertyChangedEventArgs { L"Border" });
	}
}

void NotificationWindow::BorderThickness(Microsoft::UI::Xaml::Thickness const& value) noexcept {
	if (borderThickness_ != value) {
		borderThickness_ = value;
		propertyChanged_(*this, Data::PropertyChangedEventArgs { L"BorderThickness" });
	}
}

void NotificationWindow::ViewModel(ViewModels::NotificationWindowViewModel const& value) noexcept {
	if (viewModel_ != value) {
		viewModel_ = value;
		propertyChanged_(*this, Data::PropertyChangedEventArgs { L"ViewModel" });
	}
}

void NotificationWindow::theme(app::storage::theme_t value) noexcept {
	if (config_.theme != value) {
		config_.theme = value;
		UpdateTheme(nullptr);
	}
}

void NotificationWindow::corner(app::storage::corner_t value) noexcept {
	if (config_.corner != value) {
		config_.corner = value;
		UpdateCorners();
	}
}

event_token NotificationWindow::PropertyChanged(Data::PropertyChangedEventHandler const& handler) {
	return propertyChanged_.add(handler);
}

void NotificationWindow::PropertyChanged(event_token const& token) {
	propertyChanged_.remove(token);
}

#pragma endregion
