#pragma once
#include "Services/Storages/config_t.h"

namespace app::ui {

	enum class NotificationPresenterHint {
		Windows10,
		Windows11,
	};

	struct NotificationPresenterData {
		int index;
		winrt::hstring name;
	};

	class INotificationPresenter {
	public:
		virtual ~INotificationPresenter() { }

		virtual void sync(app::storage::config_t const& config) noexcept = 0;
		virtual void show(NotificationPresenterData data) = 0;
		virtual void showSettings() noexcept = 0;
		virtual void closeAll() = 0;
	};

}
