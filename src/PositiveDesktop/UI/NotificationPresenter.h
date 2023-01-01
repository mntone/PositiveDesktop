#pragma once

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

		virtual void show(NotificationPresenterData data) = 0;
		virtual void closeAll() = 0;
	};

}
