#pragma once
#include "NotificationWindow.g.h"

#include "ViewModels/NotificationWindowViewModel.h"
#include "UI/register_value_t.h"
#include "UI/NotificationPresenter.h"

namespace winrt::PositiveDesktop::implementation {

	struct NotificationWindow: NotificationWindowT<NotificationWindow> {
		NotificationWindow(app::ui::NotificationPresenterHint hint, app::storage::desktop_t config);
		~NotificationWindow();

		void Show(float visibleDuration);

	private:
		void ReleasePrivate();
		void UpdatePosition();
		void UpdateCorners() noexcept;
		void UpdateTheme(Microsoft::UI::Xaml::FrameworkElement rootElement);

		void TrySetSystemBackdrop(Microsoft::UI::Xaml::FrameworkElement rootElement);
		void SetupSystemBackdropConfiguration(Microsoft::UI::Xaml::FrameworkElement rootElement);
		void ApplyThemeForMica(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;
		void ApplyThemeForAcrylic(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;
		void ApplyThemeForPlain(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;

		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

	public:  // - Event delegates
		void WindowSizeChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::WindowSizeChangedEventArgs const& args);

	private:
		void SystemSettingsChanged();
		void HighContrastChanged();
		void ContentThemeChanged(Microsoft::UI::Xaml::FrameworkElement const& sender, Windows::Foundation::IInspectable const& args);
		void WindowActivated(IInspectable const& sender, WindowActivatedEventArgs const& args);
		void WindowClosed(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::WindowEventArgs const& args);

	public:  // - Properties
		Microsoft::UI::Xaml::Media::Brush Background() const noexcept {
			return background_;
		}
		void Background(Microsoft::UI::Xaml::Media::Brush const& value) noexcept;

		Microsoft::UI::Xaml::Media::Brush Border() const noexcept {
			return border_;
		}
		void Border(Microsoft::UI::Xaml::Media::Brush const& value) noexcept;

		Microsoft::UI::Xaml::Thickness BorderThickness() const noexcept {
			return borderThickness_;
		}
		void BorderThickness(Microsoft::UI::Xaml::Thickness const& value) noexcept;

		ViewModels::NotificationWindowViewModel ViewModel() const noexcept {
			return viewModel_;
		}
		void ViewModel(ViewModels::NotificationWindowViewModel const& value) noexcept;

		constexpr app::storage::theme_t theme() const noexcept {
			return config_.theme;
		}
		void theme(app::storage::theme_t value) noexcept;

		constexpr app::storage::corner_t corner() const noexcept {
			return config_.corner;
		}
		void corner(app::storage::corner_t value) noexcept;

		event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(event_token const& token);

	private:
		Microsoft::UI::Xaml::Window::Activated_revoker activatedRovoker_;
		Microsoft::UI::Xaml::Window::Closed_revoker closedRovoker_;
		Microsoft::UI::Xaml::FrameworkElement::ActualThemeChanged_revoker actualThemeChangedRevoker_;
		Microsoft::UI::Dispatching::DispatcherQueueTimer timer_;

		app::ui::NotificationPresenterHint hint_;
		app::storage::desktop_t config_;
		WNDPROC nextWndProc_;
		Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration configuration_;
		Windows::Foundation::IInspectable backdropController_;
		void (NotificationWindow::*applyTheme_)(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;

		UINT dpiX_, dpiY_;
		app::ui::system_uses_light_theme_t systemUsesLightTheme_;
		app::ui::color_prevalence_t colorPrevalence_;
		app::ui::enable_transparency_t enableTransparency_;
		bool transparencyEnabled_;

		Microsoft::UI::Xaml::Media::Brush background_;
		Microsoft::UI::Xaml::Media::Brush border_;
		Microsoft::UI::Xaml::Thickness borderThickness_;
		ViewModels::NotificationWindowViewModel viewModel_;
		event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged_;
	};

}
