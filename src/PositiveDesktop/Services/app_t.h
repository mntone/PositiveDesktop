#pragma once

#include "Common/message_service_t.h"
#include "Common/RepsBase.h"

#include "Services/Storages/config_t.h"
#include "Services/KeyListeners/kbevent_t.h"
#include "Services/KeyListeners/KeyListenerService.h"
#include "Services/Desktops/DesktopService.h"

#include "UI/NotificationPresenter.h"

namespace app {

	class app_t final
		: public message_service_t<app::keylistener::kbevent_t>
		, reps::basic_observer_t<app::keylistener::kbevent_t>
		, reps::basic_observer_t<app::desktop::vdevent_t> {
	public:
		app_t();
		~app_t() noexcept;

		void initialize();
		void close() noexcept;

	private:
		void FASTCALL on(reps::bag_t<app::keylistener::kbevent_t> const& value) noexcept;
		void FASTCALL on(reps::bag_t<app::desktop::vdevent_t> const& value) noexcept;
		void process(app::keylistener::kbevent_t ev) noexcept override;

	private:
		app::storage::config_t config_;
		storage::IConfigManager* configManager_;
		ui::INotificationPresenter* presenter_;
		keylistener::KeysListenerService* keysLitener_;
		desktop::DesktopService* desktop_;
	};

}
