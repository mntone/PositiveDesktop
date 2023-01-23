#include "pch.h"
#include "NotificationPresenterWinUI3.h"

#include "Services/Storages/config_t.h"

#include "NotificationWindow.xaml.h"
#include "SettingsWindow.xaml.h"

#include "Helpers/DispatcherQueueSupport.h"
#include "Helpers/ResourceManager.h"
#include "Helpers/UIHelper.h"

namespace app::ui {

	class NotificationPresenterWinUI3: public INotificationPresenter {
	public:
		NotificationPresenterWinUI3(std::shared_ptr<storage::DesktopConfig> config) noexcept
			: finalize_(false)
			, config_(config)
			, resourceManager_(L"CodeResources")
			, window_(nullptr) {
		}

		void changeLanguage(winrt::param::hstring const& language) noexcept {
			resourceManager_.ChangeLanguage(language);
		}

		void show(NotificationPresenterData data) noexcept override {
			winrt::PositiveDesktop::UI::Helpers::implementation::Dispatch([data, this]() mutable {
				showPrivate(std::move(data));
			});
		}

		void showSettings() noexcept override {
			winrt::PositiveDesktop::UI::Helpers::implementation::Dispatch([]() {
				auto window = winrt::make<winrt::PositiveDesktop::UI::implementation::SettingsWindow>();
				window.Activate();
			});
		}

		void closeAll() noexcept override;

	private:
		void showPrivate(NotificationPresenterData data) noexcept;

		inline winrt::hstring GetFormatDesktopMessage(NotificationPresenterData data) noexcept {
			if (data.name.empty()) {
				winrt::hstring messageFormat { resourceManager_.Get<winrt::PositiveDesktop::UI::Helpers::implementation::R::Notification_Message_DesktopIndex>(data.index + 1) };
				return messageFormat;
			} else {
				winrt::hstring messageFormat { resourceManager_.Get<winrt::PositiveDesktop::UI::Helpers::implementation::R::Notification_Message_DesktopIndexAndNumber>(data.index + 1, data.name.c_str()) };
				return messageFormat;
			}
		}

	private:
		bool finalize_;
		std::shared_ptr<storage::DesktopConfig> config_;
		winrt::PositiveDesktop::UI::Helpers::implementation::ResourceManager resourceManager_;
		winrt::com_ptr<winrt::PositiveDesktop::UI::implementation::NotificationWindow> window_;
	};

}

using namespace app::storage;
using namespace app::ui;

using namespace winrt::PositiveDesktop::UI::implementation;
using namespace winrt::PositiveDesktop::UI::Helpers::implementation;
using namespace winrt::PositiveDesktop::ViewModels;

void NotificationPresenterWinUI3::closeAll() noexcept {
	finalize_ = true;

	auto window = std::exchange(window_, nullptr);
	if (!window) return;

	winrt::PositiveDesktop::UI::Helpers::implementation::Dispatch([window]() {
		window->Close();
	});
}

void NotificationPresenterWinUI3::showPrivate(NotificationPresenterData data) noexcept {
	if (finalize_) return; // Exit state

	// Check previous DispatcherQueue (require it for Mica/DesktopAcrylicController)
	DispatcherQueueSupport::EnsureDispatcherQueueController();

	// Check window
	winrt::com_ptr<NotificationWindow> window { window_ };
	if (!window) {
		window = winrt::make_self<NotificationWindow>(config_);
		window_ = window;
	}

	// Set data
	winrt::hstring caption { resourceManager_.Get<R::Notification_Caption_VirtualDesktopChanged>() };
	winrt::hstring message { GetFormatDesktopMessage(data) };
	NotificationWindowViewModel viewModel { winrt::make<implementation::NotificationWindowViewModel>(caption, message) };
	window->ViewModel(viewModel);

	// Show window
	window->Show(config_->duration());
}

INotificationPresenter* CreateWinUI3NotificationPresenter(std::shared_ptr<app::storage::DesktopConfig> config) {
	return new NotificationPresenterWinUI3(config);
}
