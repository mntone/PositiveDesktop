#pragma once

namespace app::UI {

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
		winrt::hstring message;
	};

	class INotificationPresenter {
	public:
		virtual ~INotificationPresenter() { }

		virtual void show(NotificationPresenterData data) = 0;
	};

}
