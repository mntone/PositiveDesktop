#include "pch.h"
#include "NotificationPresenterWinUI3.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

#include "NotificationWindow.xaml.h"
#include "UIHelper.h"

namespace app::UI {

	class NotificationPresenterWinUI3: public INotificationPresenter {
	public:
		NotificationPresenterWinUI3(NotificationPresenterHint hint) noexcept
			: hint_(hint)
			, resourceManager_()
			, resources_(resourceManager_.MainResourceMap().GetSubtree(L"CodeResources"))
			, dispatcherQueue_(nullptr)
			, window_(nullptr) {
		}

		void show(NotificationPresenterData data) noexcept override {
			Dispatch(gDispatchQueue, [data, this]() mutable {
				showPrivate(std::move(data));
			});
		}

	private:
		void showPrivate(NotificationPresenterData data) noexcept;
		void changeLanguage(winrt::param::hstring const& language) const noexcept;

	private:
		NotificationPresenterHint hint_;
		winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceManager resourceManager_;
		winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceMap resources_;
		winrt::Windows::System::DispatcherQueueController dispatcherQueue_;
		winrt::PositiveDesktop::NotificationWindow window_;
	};

	inline winrt::hstring GetResourceNameFromType(NotificationPresenterType type) noexcept {
		switch (type) {
		case NotificationPresenterType::Changed:
			return L"Notification_Caption_VirtualDesktopChanged";
		default:
			return L"Notification_Caption_Unknown";
		}
	}

}

using namespace app::UI;

using namespace winrt::PositiveDesktop::ViewModels;

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
	winrt::hstring captionKey { GetResourceNameFromType(data.type) };
	winrt::hstring caption { resources_.GetValue(captionKey).ValueAsString() };
	NotificationWindowViewModel viewModel = winrt::make<implementation::NotificationWindowViewModel>(caption, data.message);
	window_.ViewModel(viewModel);

	// Show window
	window_.Show(true);
}

void NotificationPresenterWinUI3::changeLanguage(winrt::param::hstring const& language) const noexcept {
	winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceContext resourceContext = resourceManager_.CreateResourceContext();
	resourceContext.QualifierValues().Insert(L"Language", language);
}

INotificationPresenter* CreateWinUI3NotificationPresenter(NotificationPresenterHint hint) {
	return new NotificationPresenterWinUI3(hint);
}
