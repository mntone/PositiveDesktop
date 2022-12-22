#pragma once

namespace UI {

	enum class NotificationPresenterType {
		Changed,
		Renamed,
	};

	struct NotificationPresenterData {
		NotificationPresenterType type;
		winrt::hstring message;
	};

	template<class TImpl>
	class NotificationPresenter: public std::false_type {
	public:
		static_assert(!value, "this class has to be implemented for desired type");

		template<typename TFn>
		void sync(TFn fn) const { }

		void show(NotificationPresenter data) { }

		TImpl native() const { }
	};

}
