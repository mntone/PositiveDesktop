#pragma once

// RElationshiPS Library: Micro observer pattern

namespace reps {

	enum class event_t: unsigned long {
		next = 2,
		completed = 3,
	};

	template<typename T>
	struct bag_t {
		union {
			event_t ev;
			HRESULT hr;
		};
		T data;
	};

	template<typename T>
	struct observer_t {
		virtual void FASTCALL on(bag_t<T> const& value) noexcept = 0;
	};

}
