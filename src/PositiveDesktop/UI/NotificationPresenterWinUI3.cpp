#include "pch.h"
#include "NotificationPresenterWinUI3.h"

#include "NotificationWindow.xaml.h"
#include "UIHelper.h"

namespace app::UI {

	class NotificationPresenterWinUI3: public INotificationPresenter {
	public:
		NotificationPresenterWinUI3(NotificationPresenterHint hint) noexcept
			: hint_(hint)
			, dispatcherQueue_(nullptr)
			, window_(nullptr)
		{ }

		void show(NotificationPresenterData data) noexcept override;

	private:
		void showPrivate(NotificationPresenterData data) noexcept;

	private:
		NotificationPresenterHint hint_;
		winrt::Windows::System::DispatcherQueueController dispatcherQueue_;
		winrt::PositiveDesktop::NotificationWindow window_;
	};

}

using namespace app::UI;

void NotificationPresenterWinUI3::show(NotificationPresenterData data) noexcept {
	Dispatch(gDispatchQueue, [data, this]() mutable {
		showPrivate(std::move(data));
	});
}

void NotificationPresenterWinUI3::showPrivate(NotificationPresenterData data) noexcept {
	// Check previous DispatcherQueue (require it for Mica/DesktopAcrylicController)
	if (nullptr == dispatcherQueue_) {
		winrt::Windows::System::DispatcherQueue dispatcherQueue { winrt::Windows::System::DispatcherQueue::GetForCurrentThread() };
		if (nullptr == dispatcherQueue) {
			dispatcherQueue_ = CreateSystemDispatcherQueueController();
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

INotificationPresenter* CreateWinUI3NotificationPresenter(NotificationPresenterHint hint) {
	return new NotificationPresenterWinUI3(hint);
}
