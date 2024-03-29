#include "pch.h"
#include "NotificationWindow.xaml.h"
#if __has_include("UI/NotificationWindow.g.cpp")
#include "UI/NotificationWindow.g.cpp"
#endif

#include <ShellScalingApi.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Mntone.AngelUmbrella.Composition.SystemBackdrops.h>

#include "Common/Math.h"
#include "Common/VersionHelper.h"

#include "Helpers/ThemeHelper.h"
#include "Helpers/UIHelper.h"
#include "Helpers/WindowHelper.h"

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

namespace resources {

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
	constexpr std::wstring_view AcrylicWindowStrokeColorBrush_Dark = L"AcrylicWindowStrokeColorBrush_Dark";
	constexpr std::wstring_view AcrylicWindowStrokeColorBrush_Light = L"AcrylicWindowStrokeColorBrush_Light";
	constexpr std::wstring_view AcrylicWindowStrokeColorBrush_Accent = L"AcrylicWindowStrokeColorBrush_Accent";

	// Windows 11 (plain) resources
	constexpr std::wstring_view Windows11WindowFillColorBrush = L"Windows11WindowFillColorBrush";
	constexpr std::wstring_view Windows11WindowStrokeColorBrush = L"Windows11WindowStrokeColorBrush";

	// MicaWindow resources
	constexpr std::wstring_view MicaWindowStrokeColorBrush = L"MicaWindowStrokeColorBrush";

}

#pragma warning(push)
#pragma warning(disable: 4324)
extern std::pair<app::int32x2_t, app::double4> getPositionAndThickness(
	app::storage::DesktopConfig const* config,
	app::int32x4_t workArea,
	app::int32x4_t outerBounds,
	app::int32x2_t size,
	bool isSquareCorner);
#pragma warning(pop)


namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::Graphics;
	using namespace ::winrt::Windows::UI::ViewManagement;

	using namespace ::winrt::Microsoft::UI;
	using namespace ::winrt::Microsoft::UI::Composition;
	using namespace ::winrt::Microsoft::UI::Composition::SystemBackdrops;
	using namespace ::winrt::Microsoft::UI::Windowing;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;

	using namespace ::winrt::Mntone::AngelUmbrella::Composition::SystemBackdrops;
}

using namespace winrt::PositiveDesktop::UI::implementation;
using namespace winrt::PositiveDesktop::UI::Helpers::implementation;

NotificationWindow::NotificationWindow(std::shared_ptr<app::storage::DesktopConfig> config)
	: config_(std::move(config))
	, token_(nullptr)
	, configuration_(nullptr)
	, backdropController_(nullptr)
	, applyTheme_(nullptr)
	, dpiX_(USER_DEFAULT_SCREEN_DPI)
	, dpiY_(USER_DEFAULT_SCREEN_DPI)
	, cornerPreference_(GetDwmCornerFromConfigType(config_->corner()))
	, transparencyEnabled_(false)
	, background_(nullptr)
	, border_(nullptr)
	, borderThickness_({ 1, 1, 1, 1 })
	, viewModel_(nullptr)
	, timer_(nullptr) {
	InitializeComponent();

	HWND hWnd { GetHwnd(m_inner) };

	// Get DPI
	HMONITOR hMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
	WINRT_ASSERT(hMonitor);
	check_hresult(GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX_, &dpiY_));

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
	::winrt::AppWindow appWindow { GetAppWindow(hWnd) };
	appWindow.IsShownInSwitchers(false);
	appWindow.ResizeClient(Windows::Graphics::SizeInt32 { 500, 95 });

	OverlappedPresenter presenter { appWindow.Presenter().as<OverlappedPresenter>() };
	presenter.IsAlwaysOnTop(true);
	presenter.IsMaximizable(false);
	presenter.IsMinimizable(false);
	presenter.IsResizable(false);
	presenter.SetBorderAndTitleBar(false, false);

	// Subclassing
	Subclass(hWnd);

	// Init theme, backdrop, and etc...
	FrameworkElement rootElement = Content().as<FrameworkElement>();
	TrySetSystemBackdrop(rootElement);
	UpdateTheme(rootElement); /* PLEASE call this after calling TrySetSystemBackdrop because of using configuration_ */
	(this->*applyTheme_)(rootElement);
	if (config_->corner() != app::storage::cnr_default) {
		UpdateCorners();
	}

	// Hook config
	token_ = config_->addObserver(observer());
}

void NotificationWindow::ReleasePrivate() {
	LOG_BEGIN(app::logger::ltg_presenter);

	// Don't receive WM_CLOSE when calling AppWindow::Close.
	// So, we catch WindowClosed bacause of releasing subclass decently.
	HWND hWnd { GetHwnd(m_inner) };
	WindowBase::ReleaseSubclass(hWnd);

	if (token_) {
		config_->removeObserver(token_);
		token_ = nullptr;
	}

	actualThemeChangedRevoker_.revoke();
	activatedRovoker_.revoke();
	closedRovoker_.revoke();

	LOG_END_NOLABEL();
}

void NotificationWindow::Show(float visibleDuration) {
	// TODO: Observe the WorkArea and update position.
	winrt::Microsoft::UI::Windowing::AppWindow appWindow = GetAppWindow(m_inner);
	winrt::Microsoft::UI::DisplayId primaryDisplayId = GetPrimaryDisplayId();
	winrt::Microsoft::UI::Windowing::DisplayArea displayArea = winrt::Microsoft::UI::Windowing::DisplayArea::GetFromDisplayId(primaryDisplayId);

	// Set ideal size
	app::int32x4_t workArea = displayArea.WorkArea();
	app::int32x4_t outerBounds = displayArea.OuterBounds();
	bool isSquareCorner = true;
	if (app::VersionHelper::isWindows11()) {
		switch (cornerPreference_) {
		case DWMWCP_DEFAULT:
		case DWMWCP_ROUND:
			isSquareCorner = false;
			workArea.addPadding(12);
			outerBounds.addPadding(12);
			break;
		case DWMWCP_ROUNDSMALL:
			isSquareCorner = false;
			workArea.addPadding(4);
			outerBounds.addPadding(4);
			break;
		}
	}

	UIElement content = Content();
	app::float2 invScale = app::float2(USER_DEFAULT_SCREEN_DPI) / app::float2(static_cast<float>(dpiX_), static_cast<float>(dpiY_));
	app::float2 workSizeFloat = static_cast<app::float2>(workArea.size()) * invScale;
	content.Measure(workSizeFloat);

	// Adjust window size
	Size expectedSize = content.DesiredSize();
	appWindow.ResizeClient(SizeInt32 {
		500,
		app::lceil(expectedSize.Height),
	});

	// Calc position
	std::pair<app::int32x2_t, app::double4> calcData = getPositionAndThickness(
		config_.get(),
		workArea,
		outerBounds,
		appWindow.Size(),
		isSquareCorner);
	BorderThickness(calcData.second);
	appWindow.Move(calcData.first);
	if (!appWindow.IsVisible()) {
		timer_ = nullptr;
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

void FASTCALL NotificationWindow::on(reps::bag_t<app::storage::desktop_update_t> const& value) noexcept {
	using namespace app::storage;

	if (value.hr < 0) {
		// error
		return;
	}

	if (reps::event_t::completed == value.ev) {
		if (token_) {
			config_->removeObserver(token_);
			token_ = nullptr;
		}
		return;
	}

	bool shouldUpdatePosition { false };
	desktop_update_t const& config { value.data };
	if (config.property & prp_theme) {
		FrameworkElement rootElement { Content().as<FrameworkElement>() };
		UpdateTheme(rootElement);
	}
	if (config.property & prp_corner) {
		if (UpdateCorners()) {
			shouldUpdatePosition = true;
		}
	}

	winrt::Microsoft::UI::Windowing::AppWindow appWindow { GetAppWindow(m_inner) };
	if (appWindow.IsVisible()) {
		if (shouldUpdatePosition || config.property & prp_position) {
			UpdatePosition();
		}
	}
}

LRESULT NotificationWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	if (WM_THEMECHANGED == message) {
		HighContrastChanged();
	} else if (WM_SETTINGCHANGE == message) {
		SystemSettingsChanged();
	} else if (WM_DPICHANGED == message) {
		dpiX_ = LOWORD(wParam);
		dpiY_ = HIWORD(wParam);
	}
	return WindowBase::WndProc(hWnd, message, wParam, lParam);
}

void NotificationWindow::UpdatePosition() {
	// TODO: Observe the WorkArea and update position.
	winrt::Microsoft::UI::Windowing::AppWindow appWindow = GetAppWindow(m_inner);
	winrt::Microsoft::UI::DisplayId primaryDisplayId = GetPrimaryDisplayId();
	winrt::Microsoft::UI::Windowing::DisplayArea displayArea = winrt::Microsoft::UI::Windowing::DisplayArea::GetFromDisplayId(primaryDisplayId);

	// Set ideal size
	app::int32x4_t workArea = displayArea.WorkArea();
	app::int32x4_t outerBounds = displayArea.OuterBounds();
	bool isSquareCorner = true;
	if (app::VersionHelper::isWindows11()) {
		switch (cornerPreference_) {
		case DWMWCP_DEFAULT:
		case DWMWCP_ROUND:
			isSquareCorner = false;
			workArea.addPadding(12);
			outerBounds.addPadding(12);
			break;
		case DWMWCP_ROUNDSMALL:
			isSquareCorner = false;
			workArea.addPadding(4);
			outerBounds.addPadding(4);
			break;
		}
	}

	// Calc position
	std::pair<app::int32x2_t, app::double4> calcData = getPositionAndThickness(
		config_.get(),
		workArea,
		outerBounds,
		appWindow.Size(),
		isSquareCorner);
	appWindow.Move(calcData.first);
}

bool NotificationWindow::UpdateCorners() noexcept {
	if (!app::VersionHelper::isWindows11()) return false;

	app::storage::corner_t corner { config_->corner() };
	DWM_WINDOW_CORNER_PREFERENCE cornerPreference { GetDwmCornerFromConfigType(corner) };
	if (cornerPreference == cornerPreference_) return false;

	HWND hWnd { GetNullableHwnd(m_inner) };
	if (!hWnd) return false;

	HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPreference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
	if (FAILED(hr)) {
		// Rollback if available
		hr = DwmGetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPreference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
		if (SUCCEEDED(hr)) {
			cornerPreference_ = cornerPreference;
		}
		return false;
	} else {
		cornerPreference_ = cornerPreference;
	}
	return true;
}

void NotificationWindow::UpdateTheme(FrameworkElement rootElement) {
	ElementTheme elementTheme;
	if (AccessibilitySettings().HighContrast()) {
		elementTheme = ElementTheme::Default;
	} else {
		app::storage::theme_t theme { config_->theme() };
		switch (theme) {
		case app::storage::thm_dark:
		case app::storage::thm_accent:
			elementTheme = ElementTheme::Dark;
			break;
		case app::storage::thm_light:
			elementTheme = ElementTheme::Light;
			break;
		case app::storage::thm_system:
		default:
		{
			DWORD preserveColor = colorPrevalence_.value();
			if (preserveColor) {
				elementTheme = ElementTheme::Dark;
			} else {
				DWORD useLight = systemUsesLightTheme_.value();
				if (useLight) {
					elementTheme = ElementTheme::Light;
				} else {
					elementTheme = ElementTheme::Dark;
				}
			}
			break;
		}
		}
	}
	if (rootElement.RequestedTheme() != elementTheme) {
		rootElement.RequestedTheme(elementTheme);
	} else {
		(this->*applyTheme_)(rootElement);
	}
}

void NotificationWindow::TrySetSystemBackdrop(FrameworkElement rootElement) {
	applyTheme_ = nullptr;
	backdropController_ = nullptr;
	configuration_ = nullptr;
	transparencyEnabled_ = false;

	if (!AccessibilitySettings().HighContrast()) {
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
	WINRT_ASSERT(!AccessibilitySettings().HighContrast());
	WINRT_ASSERT(backdropController_);

	if (colorPrevalence_.value()) {
	} else {
		winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme backdropTheme = ConvertToSystemBackdropTheme(rootElement.ActualTheme());
		configuration_.Theme(backdropTheme);
	}
}

void NotificationWindow::ApplyThemeForAcrylic(FrameworkElement rootElement) noexcept {
	WINRT_ASSERT(!AccessibilitySettings().HighContrast());
	WINRT_ASSERT(backdropController_);
	if (!rootElement) {
		rootElement = Content().as<FrameworkElement>();
	}

	Media::Brush border { nullptr };
	app::storage::theme_t theme { config_->theme() };
	DesktopAcrylicController controller { backdropController_.as<DesktopAcrylicController>() };
	if (app::storage::thm_accent == theme || app::storage::thm_default == theme && colorPrevalence_.value()) {
		DesktopAcrylicHelper::SetColors(controller, DesktopAcrylicTheme::AccentDark, ::winrt::Mntone::AngelUmbrella::Composition::SystemBackdrops::DesktopAcrylicKind::Default);
		configuration_.Theme(SystemBackdropTheme::Dark);
		border = GetBrush(rootElement.Resources(), resources::AcrylicWindowStrokeColorBrush_Accent);
	} else {
		switch (rootElement.ActualTheme()) {
		case ElementTheme::Dark:
			DesktopAcrylicHelper::SetColors(controller, DesktopAcrylicTheme::Dark, ::winrt::Mntone::AngelUmbrella::Composition::SystemBackdrops::DesktopAcrylicKind::Default);
			configuration_.Theme(SystemBackdropTheme::Dark);
			border = GetBrush(rootElement.Resources(), resources::AcrylicWindowStrokeColorBrush_Dark);
			break;
		case ElementTheme::Light:
		default:
			DesktopAcrylicHelper::SetColors(controller, DesktopAcrylicTheme::Light, ::winrt::Mntone::AngelUmbrella::Composition::SystemBackdrops::DesktopAcrylicKind::Base);
			configuration_.Theme(SystemBackdropTheme::Light);
			border = GetBrush(rootElement.Resources(), resources::AcrylicWindowStrokeColorBrush_Light);
			break;
		}
	}

	Border(border);
}

void NotificationWindow::ApplyThemeForPlain(FrameworkElement rootElement) noexcept {
	WINRT_ASSERT(!backdropController_);
	if (!rootElement) {
		rootElement = Content().as<FrameworkElement>();
	}

	ResourceDictionary res { rootElement.Resources() };
	Media::Brush background { nullptr }, border { nullptr };
	if (AccessibilitySettings().HighContrast()) {
		background = GetBrush(res, resources::WindowFillColorBrush_HighContrast);
		border = GetBrush(res, resources::WindowStrokeColorBrush_HighContrast);
	} else if (app::VersionHelper::isWindows11()) {
		background = GetBrush(res, resources::Windows11WindowFillColorBrush);
		border = GetBrush(res, resources::Windows11WindowStrokeColorBrush);
	} else {
		if (colorPrevalence_.value()) {
			background = GetBrush(res, resources::Windows10WindowFillColorBrush_Accent);
			border = GetBrush(res, resources::Windows10WindowStrokeColorBrush_Accent);
		} else {
			switch (rootElement.ActualTheme()) {
			case ElementTheme::Dark:
				background = GetBrush(res, resources::Windows10WindowFillColorBrush_Dark);
				border = GetBrush(res, resources::Windows10WindowStrokeColorBrush_Dark);
				break;
			case ElementTheme::Light:
			default:
				background = GetBrush(res, resources::Windows10WindowFillColorBrush_Light);
				border = GetBrush(res, resources::Windows10WindowStrokeColorBrush_Light);
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

	app::storage::theme_t theme { config_->theme() };
	if (app::storage::thm_system == theme) {
		ElementTheme elementTheme;
		DWORD preserveColor = colorPrevalence_.value();
		if (preserveColor) {
			elementTheme = ElementTheme::Dark;
		} else {
			DWORD useLight = systemUsesLightTheme_.value();
			if (useLight) {
				elementTheme = ElementTheme::Light;
			} else {
				elementTheme = ElementTheme::Dark;
			}
		}

		if (rootElement.RequestedTheme() != elementTheme) {
			rootElement.RequestedTheme(elementTheme);
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
	app::storage::inactive_backdrop_t inactiveBackdrop { config_->inactiveBackdrop() };
	if (app::storage::ibd_disabled == inactiveBackdrop) {
		configuration_.IsInputActive(WindowActivationState::Deactivated != args.WindowActivationState());
	}
}

void NotificationWindow::WindowClosed(IInspectable const& /*sender*/, WindowEventArgs const& /*args*/) {
	ReleasePrivate();
}

void NotificationWindow::WindowSizeChanged(IInspectable const& /*sender*/, WindowSizeChangedEventArgs const& /*args*/) {
	UpdatePosition();
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

winrt::event_token NotificationWindow::PropertyChanged(Data::PropertyChangedEventHandler const& handler) {
	return propertyChanged_.add(handler);
}

void NotificationWindow::PropertyChanged(event_token const& token) {
	propertyChanged_.remove(token);
}

#pragma endregion
