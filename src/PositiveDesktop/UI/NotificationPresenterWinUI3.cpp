#include "pch.h"
#include "NotificationPresenterWinUI3.h"

#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

#include "NotificationWindow.xaml.h"
#include "UIHelper.h"

namespace app::ui {

	class NotificationPresenterWinUI3: public INotificationPresenter {
	public:
		NotificationPresenterWinUI3(app::storage::config_t const& config, NotificationPresenterHint hint) noexcept
			: finalize_(false)
			, config_(config)
			, hint_(hint)
			, resourceManager_()
			, resources_(resourceManager_.MainResourceMap().GetSubtree(L"CodeResources"))
			, dispatcherQueue_(nullptr)
			, window_(nullptr) {
		}

		void show(NotificationPresenterData data) noexcept override {
			dispatch(gDispatchQueue, [data, this]() mutable {
				showPrivate(std::move(data));
			});
		}

		void closeAll() noexcept override;

	private:
		void showPrivate(NotificationPresenterData data) noexcept;
		void changeLanguage(winrt::param::hstring const& language) const noexcept;

		inline winrt::hstring GetFormatDesktopMessage(NotificationPresenterData data) noexcept {
			if (data.name.empty()) {
				winrt::hstring messageKey { L"Notification_Message_DesktopIndex" };
				winrt::hstring messageFormat { resources_.GetValue(messageKey).ValueAsString() };
				wchar_t buf[256];
				swprintf_s(buf, messageFormat.c_str(), data.index + 1);
				return winrt::hstring { buf };
			} else {
				winrt::hstring messageKey { L"Notification_Message_DesktopIndexAndNumber" };
				winrt::hstring messageFormat { resources_.GetValue(messageKey).ValueAsString() };
				wchar_t buf[256];
				swprintf_s(buf, messageFormat.c_str(), data.index + 1, data.name.c_str());
				return winrt::hstring { buf };
			}
		}

	private:
		bool finalize_;
		app::storage::config_t const& config_;
		NotificationPresenterHint hint_;
		winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceManager resourceManager_;
		winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceMap resources_;
		winrt::Windows::System::DispatcherQueueController dispatcherQueue_;
		winrt::PositiveDesktop::NotificationWindow window_;
	};

}

using namespace app::storage;
using namespace app::ui;

using namespace winrt::PositiveDesktop::ViewModels;

void NotificationPresenterWinUI3::closeAll() noexcept {
	finalize_ = true;

	winrt::PositiveDesktop::NotificationWindow window = std::exchange(window_, nullptr);
	if (!window) return;

	dispatch(gDispatchQueue, [window]() {
		window.Close();
	});
}

void NotificationPresenterWinUI3::showPrivate(NotificationPresenterData data) noexcept {
	if (finalize_) return; // Exit state

	// Check previous DispatcherQueue (require it for Mica/DesktopAcrylicController)
	if (nullptr == dispatcherQueue_) {
		winrt::Windows::System::DispatcherQueue dispatcherQueue { winrt::Windows::System::DispatcherQueue::GetForCurrentThread() };
		if (nullptr == dispatcherQueue) {
			dispatcherQueue_ = createSystemDispatcherQueueController();
		}
	}

	// Check window
	if (!window_) {
		desktop_t config = config_.defaultDesktop;
		if (config.theme == thm_default) {
			config.theme = thm_system;
		}

		window_ = winrt::make<winrt::PositiveDesktop::implementation::NotificationWindow>(hint_, std::move(config));
	}

	// Set data
	winrt::hstring captionKey { L"Notification_Caption_VirtualDesktopChanged" };
	winrt::hstring caption { resources_.GetValue(captionKey).ValueAsString() };
	winrt::hstring message { GetFormatDesktopMessage(data) };
	NotificationWindowViewModel viewModel = winrt::make<implementation::NotificationWindowViewModel>(caption, message);
	window_.ViewModel(viewModel);

	// Show window
	window_.Show(app::storage::actualDuration(config_.defaultDesktop.duration));
}

void NotificationPresenterWinUI3::changeLanguage(winrt::param::hstring const& language) const noexcept {
	winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceContext resourceContext = resourceManager_.CreateResourceContext();
	resourceContext.QualifierValues().Insert(L"Language", language);
}

INotificationPresenter* CreateWinUI3NotificationPresenter(app::storage::config_t const& config, NotificationPresenterHint hint) {
	return new NotificationPresenterWinUI3(config, hint);
}
