#pragma once
#include "UI/NotificationWindow.g.h"
#include "NotificationPresenter.h"

#include <dwmapi.h>

#include "Services/Storages/DesktopConfig.h"

#include "Helpers/register_value_t.h"
#include "Helpers/WindowBase.h"

#include "ViewModels/NotificationWindowViewModel.h"

namespace winrt::PositiveDesktop::UI::implementation {

	struct NotificationWindow:
		NotificationWindowT<NotificationWindow>,
		Helpers::implementation::WindowBase,
		reps::basic_observer_t<app::storage::desktop_update_t> {
		NotificationWindow(std::shared_ptr<app::storage::DesktopConfig> config);

		void Show(float visibleDuration);

	private:
		void ReleasePrivate();
		void UpdatePosition();
		bool UpdateCorners() noexcept;
		void UpdateTheme(Microsoft::UI::Xaml::FrameworkElement rootElement);

		void TrySetSystemBackdrop(Microsoft::UI::Xaml::FrameworkElement rootElement);
		void SetupSystemBackdropConfiguration(Microsoft::UI::Xaml::FrameworkElement rootElement);
		void ApplyThemeForMica(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;
		void ApplyThemeForAcrylic(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;
		void ApplyThemeForPlain(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;

		void FASTCALL on(reps::bag_t<app::storage::desktop_update_t> const& value) noexcept;

		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept override final;

	public:  // - Event delegates
		void WindowSizeChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::WindowSizeChangedEventArgs const& args);

	private:
		void SystemSettingsChanged();
		void HighContrastChanged();
		void ContentThemeChanged(Microsoft::UI::Xaml::FrameworkElement const& sender, Windows::Foundation::IInspectable const& args);
		void WindowActivated(IInspectable const& sender, Microsoft::UI::Xaml::WindowActivatedEventArgs const& args);
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

		event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(event_token const& token);

	private:
		Microsoft::UI::Xaml::Window::Activated_revoker activatedRovoker_;
		Microsoft::UI::Xaml::Window::Closed_revoker closedRovoker_;
		Microsoft::UI::Xaml::FrameworkElement::ActualThemeChanged_revoker actualThemeChangedRevoker_;
		Microsoft::UI::Dispatching::DispatcherQueueTimer timer_;

		std::shared_ptr<app::storage::DesktopConfig> config_;
		reps::observer_token token_;

		Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration configuration_;
		Windows::Foundation::IInspectable backdropController_;
		void (NotificationWindow::*applyTheme_)(Microsoft::UI::Xaml::FrameworkElement rootElement) noexcept;

		UINT dpiX_, dpiY_;
		DWM_WINDOW_CORNER_PREFERENCE cornerPreference_;
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
