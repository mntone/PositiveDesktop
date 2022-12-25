#pragma once

#include "Services/Storages/config_t.h"
#include "Services/VirtualDesktopNotification/VirtualDesktopNotificationService.h"

namespace app {

	struct app_t final {
		app_t();
		~app_t() noexcept;

		void initialize();

	private:
		app::storage::config_t config_;
		storage::IConfigManager* configManager_;
		UI::INotificationPresenter* presenter_;
		listener::VirtualDesktopNotificationService* notificationListener_;
	};

}
