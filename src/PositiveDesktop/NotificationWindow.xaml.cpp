#include "pch.h"
#include "NotificationWindow.xaml.h"
#if __has_include("NotificationWindow.g.cpp")
#include "NotificationWindow.g.cpp"
#endif

#include <dwmapi.h>

#include "Common/Math.h"
#include "UI/WindowHelper.h"

using namespace winrt;

using namespace Microsoft::UI;
using namespace Microsoft::UI::Composition;
using namespace Microsoft::UI::Composition::SystemBackdrops;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Foundation;
using namespace Windows::Graphics;

using namespace PositiveDesktop::implementation;

namespace muw = winrt::Microsoft::UI::Windowing;

NotificationWindow::NotificationWindow(app::UI::NotificationPresenterHint hint, app::storage::corner_t corner)
	: hint_(hint)
	, corner_(corner)
	, viewModel_(make<ViewModels::implementation::NotificationWindowViewModel>())
	, timer_(nullptr) {
	InitializeComponent();

	HWND hWnd { GetHwnd(m_inner) };
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

	muw::AppWindow appWindow = GetAppWindow(hWnd);
	appWindow.IsShownInSwitchers(false);
	appWindow.ResizeClient(Windows::Graphics::SizeInt32 { 500, 100 });

	muw::OverlappedPresenter presenter = appWindow.Presenter().as<muw::OverlappedPresenter>();
	presenter.IsAlwaysOnTop(true);
	presenter.IsMaximizable(false);
	presenter.IsMinimizable(false);
	presenter.IsResizable(false);
	presenter.SetBorderAndTitleBar(false, false);

	if (corner != app::storage::cnr_default) {
		UpdateCorners();
	}
	TrySetSystemBackdrop();
}

NotificationWindow::~NotificationWindow() {
	ReleasePrivate();
}

void NotificationWindow::ReleasePrivate() {
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
	if (hint_ != app::UI::NotificationPresenterHint::Windows11) return;

	HWND hWnd { GetNullableHwnd(m_inner) };
	if (!hWnd) return;

	DWM_WINDOW_CORNER_PREFERENCE preference { GetDwmCornerFromConfigType(corner_) };
	HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
	if (FAILED(hr)) {
		// Rollback if available
		hr = DwmGetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
		if (SUCCEEDED(hr)) {
			corner_ = GetConfigCornerFromDwmType(preference);
		}
	}
}

void NotificationWindow::TrySetSystemBackdrop() {
	if (MicaController::IsSupported()) {
		SetupSystemBackdropConfiguration();

		MicaController controller = MicaController();
		controller.SetSystemBackdropConfiguration(configuration_);
		controller.AddSystemBackdropTarget(try_as<ICompositionSupportsSystemBackdrop>());
		backdropController_ = controller;
	} else if (DesktopAcrylicController::IsSupported()) {
		SetupSystemBackdropConfiguration();

		DesktopAcrylicController controller = DesktopAcrylicController();
		controller.SetSystemBackdropConfiguration(configuration_);
		controller.AddSystemBackdropTarget(try_as<ICompositionSupportsSystemBackdrop>());
		backdropController_ = controller;
	}
}

inline winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme ConvertToSystemBackdropTheme(winrt::Microsoft::UI::Xaml::ElementTheme const& theme) {
	switch (theme) {
	case winrt::Microsoft::UI::Xaml::ElementTheme::Dark:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Dark;
	case winrt::Microsoft::UI::Xaml::ElementTheme::Light:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Light;
	default:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Default;
	}
}

void NotificationWindow::SetupSystemBackdropConfiguration() {
	WINRT_ASSERT(!activatedRovoker_);
	WINRT_ASSERT(!closedRovoker_);

	configuration_ = SystemBackdropConfiguration();
	activatedRovoker_ = Activated(auto_revoke, { this, &NotificationWindow::WindowActivated });
	closedRovoker_ = Closed(auto_revoke, { this, &NotificationWindow::WindowClosed });

	configuration_.IsInputActive(true);

	auto rootElement = Content().try_as<FrameworkElement>();
	if (nullptr != rootElement) {
		auto backdropTheme = ConvertToSystemBackdropTheme(rootElement.ActualTheme());
		configuration_.Theme(backdropTheme);
	}
}

#pragma region Event delegates

void NotificationWindow::WindowActivated(IInspectable const& /*sender*/, WindowActivatedEventArgs const& args) {
	configuration_.IsInputActive(WindowActivationState::Deactivated != args.WindowActivationState());
}

void NotificationWindow::WindowClosed(IInspectable const& /*sender*/, WindowEventArgs const& /*args*/) {
	ReleasePrivate();
}

#pragma endregion

#pragma region Properties 

void NotificationWindow::ViewModel(ViewModels::NotificationWindowViewModel const& value) noexcept {
	if (viewModel_ != value) {
		viewModel_ = value;
		propertyChanged_(*this, Data::PropertyChangedEventArgs { L"ViewModel" });
	}
}

void NotificationWindow::corner(app::storage::corner_t value) noexcept {
	if (corner_ != value) {
		corner_ = value;
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
