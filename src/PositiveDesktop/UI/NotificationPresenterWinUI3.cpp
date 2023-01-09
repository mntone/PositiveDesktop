#include "pch.h"
#include "NotificationPresenterWinUI3.h"

#include "Services/Storages/config_t.h"

#include "NotificationWindow.xaml.h"
#include "SettingsWindow.xaml.h"
#include "ResourceManager.h"
#include "UIHelper.h"

namespace app::ui {

	class NotificationPresenterWinUI3: public INotificationPresenter {
	public:
		NotificationPresenterWinUI3(app::storage::config_t const& config, NotificationPresenterHint hint) noexcept
			: finalize_(false)
			, config_(config)
			, hint_(hint)
			, resourceManager_(L"CodeResources")
			, dispatcherQueue_(nullptr)
			, window_(nullptr) {
		}

		void changeLanguage(winrt::param::hstring const& language) noexcept {
			resourceManager_.changeLanguage(language);
		}

		void show(NotificationPresenterData data) noexcept override {
			dispatch([data, this]() mutable {
				showPrivate(std::move(data));
			});
		}

		void showSettings() noexcept override {
			dispatch([]() {
				auto window = winrt::make<winrt::PositiveDesktop::implementation::SettingsWindow>();
				window.Activate();
			});
		}

		void closeAll() noexcept override;

	private:
		void showPrivate(NotificationPresenterData data) noexcept;

		inline winrt::hstring GetFormatDesktopMessage(NotificationPresenterData data) noexcept {
			if (data.name.empty()) {
				winrt::hstring messageFormat { resourceManager_.get<resource::R::Notification_Message_DesktopIndex>(data.index + 1) };
				return messageFormat;
			} else {
				winrt::hstring messageFormat { resourceManager_.get<resource::R::Notification_Message_DesktopIndexAndNumber>(data.index + 1, data.name.c_str()) };
				return messageFormat;
			}
		}

	private:
		bool finalize_;
		app::storage::config_t const& config_;
		NotificationPresenterHint hint_;
		resource::ResourceManager resourceManager_;
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
	winrt::hstring caption { resourceManager_.get<resource::R::Notification_Caption_VirtualDesktopChanged>() };
	winrt::hstring message { GetFormatDesktopMessage(data) };
	NotificationWindowViewModel viewModel = winrt::make<implementation::NotificationWindowViewModel>(caption, message);
	window_.ViewModel(viewModel);

	// Show window
	window_.Show(app::storage::actualDuration(config_.defaultDesktop.duration));
}

INotificationPresenter* CreateWinUI3NotificationPresenter(app::storage::config_t const& config, NotificationPresenterHint hint) {
	return new NotificationPresenterWinUI3(config, hint);
}
