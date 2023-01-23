#pragma once
#include "Services/Storages/config_t.h"

namespace app::ui {

	struct NotificationPresenterData {
		int index;
		winrt::hstring name;
	};

	class INotificationPresenter {
	public:
		virtual ~INotificationPresenter() { }

		virtual void show(NotificationPresenterData data) = 0;
		virtual void showSettings() noexcept = 0;
		virtual void closeAll() = 0;
	};

}
