#pragma once
#include "NotificationWindow.g.h"

#include "ViewModels/NotificationWindowViewModel.h"
#include "UI/NotificationPresenter.h"

namespace winrt::PositiveDesktop::implementation {

	struct NotificationWindow: NotificationWindowT<NotificationWindow> {
		NotificationWindow(app::UI::NotificationPresenterHint hint, app::storage::corner_t corner);
		~NotificationWindow();

		void Show(float visibleDuration);

	private:
		void ReleasePrivate();
		void UpdateCorners() noexcept;
		void TrySetSystemBackdrop();
		void SetupSystemBackdropConfiguration();

	private: // - Event delegates
		void WindowActivated(IInspectable const& sender, WindowActivatedEventArgs const& args);
		void WindowClosed(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::WindowEventArgs const& args);

	public:  // - Properties
		ViewModels::NotificationWindowViewModel ViewModel() const noexcept {
			return viewModel_;
		}
		void ViewModel(ViewModels::NotificationWindowViewModel const& value) noexcept;

		app::storage::corner_t corner() const noexcept {
			return corner_;
		}
		void corner(app::storage::corner_t value) noexcept;

		event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(event_token const& token);

	private:
		Microsoft::UI::Xaml::Window::Activated_revoker activatedRovoker_;
		Microsoft::UI::Xaml::Window::Closed_revoker closedRovoker_;
		Microsoft::UI::Dispatching::DispatcherQueueTimer timer_;

		app::UI::NotificationPresenterHint hint_;
		app::storage::corner_t corner_;
		Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration configuration_;
		Windows::Foundation::IInspectable backdropController_;

		ViewModels::NotificationWindowViewModel viewModel_;
		event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged_;
	};

}
