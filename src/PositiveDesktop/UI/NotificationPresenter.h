#pragma once

namespace app::ui {

	enum class NotificationPresenterHint {
		Windows10,
		Windows11,
	};

	enum class NotificationPresenterType {
		Changed,
		Renamed,
	};

	struct NotificationPresenterData {
		NotificationPresenterType type;
		int index;
		winrt::hstring name;
	};

	class INotificationPresenter {
	public:
		virtual ~INotificationPresenter() { }

		virtual void show(NotificationPresenterData data) = 0;
	};

}
