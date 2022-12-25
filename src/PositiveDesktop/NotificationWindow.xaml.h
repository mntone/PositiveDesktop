#pragma once
#include "NotificationWindow.g.h"

#include "ViewModels/NotificationWindowViewModel.h"

namespace winrt::PositiveDesktop::implementation {

	struct NotificationWindow: NotificationWindowT<NotificationWindow> {
		NotificationWindow();
		~NotificationWindow();

		void Show(float visibleDuration);

	private:
		void ReleasePrivate();
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

		winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(winrt::event_token const& token);

	private:
		winrt::event_token activated_, closed_;

		Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration configuration_;
		Windows::Foundation::IInspectable backdropController_;

		ViewModels::NotificationWindowViewModel viewModel_;
		event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged_;

		Microsoft::UI::Dispatching::DispatcherQueueTimer timer_;
	};

}
