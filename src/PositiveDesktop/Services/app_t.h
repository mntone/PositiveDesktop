#pragma once

#include "Common/RepsBase.h"

#include "Services/Storages/config_t.h"
#include "Services/KeyListeners/KeyListenerService.h"
#include "Services/VirtualDesktopNotification/VirtualDesktopNotificationService.h"

namespace app {

	class app_t final: reps::observer_t {
	public:
		app_t();
		~app_t() noexcept;

		void initialize();

	private:
		void FASTCALL on(reps::bag_t const& value) noexcept override;

	private:
		app::storage::config_t config_;
		storage::IConfigManager* configManager_;
		ui::INotificationPresenter* presenter_;
		keylisteners::KeysListenerService* keysLitener_;
		listener::VirtualDesktopNotificationService* notificationListener_;
	};

}
