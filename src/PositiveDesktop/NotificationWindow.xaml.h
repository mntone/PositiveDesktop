#pragma once
#include "NotificationWindow.g.h"

#include "ViewModels/NotificationWindowViewModel.h"

namespace winrt::PositiveDesktop::implementation {

	struct NotificationWindow: NotificationWindowT<NotificationWindow> {
		NotificationWindow();
		~NotificationWindow();

		void Show(bool autoHide);

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

#include "UI/UIHelper.h"
#include "UI/NotificationPresenter.h"

extern winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue;

namespace UI {

	// TODO: [maybe] remove template?
	template<>
	class NotificationPresenter<winrt::PositiveDesktop::NotificationWindow>: public std::true_type {
	public:
		NotificationPresenter(): dispatcherQueue_(nullptr), window_(nullptr) { }

		template<typename TFn>
		void sync(TFn fn) const {
			Dispatch(gDispatchQueue, fn);
		}

		void show(NotificationPresenterData data) {
			// Check previous DispatcherQueue (require it for Mica/DesktopAcrylicController)
			if (nullptr == dispatcherQueue_) {
				winrt::Windows::System::DispatcherQueue dispatcherQueue { winrt::Windows::System::DispatcherQueue::GetForCurrentThread() };
				if (nullptr == dispatcherQueue) {
					dispatcherQueue_ = NotificationPresenter::CreateSystemDispatcherQueueController();
				}
			}

			// Check window
			if (!window_) {
				window_ = winrt::make<winrt::PositiveDesktop::implementation::NotificationWindow>();
			}

			// Set data
			winrt::PositiveDesktop::ViewModels::NotificationWindowViewModel viewModel = winrt::make<winrt::PositiveDesktop::ViewModels::implementation::NotificationWindowViewModel>(data);
			window_.ViewModel(viewModel);

			// Show window
			window_.Show(true);
		}

		winrt::PositiveDesktop::NotificationWindow native() const {
			return window_;
		}

	private:
		static inline winrt::Windows::System::DispatcherQueueController CreateSystemDispatcherQueueController() {
			DispatcherQueueOptions options {
				sizeof(DispatcherQueueOptions),
				DQTYPE_THREAD_CURRENT,
				DQTAT_COM_NONE,
			};

			ABI::Windows::System::IDispatcherQueueController* dispatchQueueController = nullptr;
			winrt::check_hresult(CreateDispatcherQueueController(options, &dispatchQueueController));
			return { dispatchQueueController, winrt::take_ownership_from_abi };
		}

	private:
		winrt::Windows::System::DispatcherQueueController dispatcherQueue_;
		winrt::PositiveDesktop::NotificationWindow window_;
	};

}
