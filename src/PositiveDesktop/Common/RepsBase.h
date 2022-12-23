#pragma once

// RElationshiPS Library: Micro observer pattern

namespace reps {

	enum class event_t: unsigned long {
		next = 2,
		completed = 3,
	};

	struct bag_t {
		union {
			event_t ev;
			HRESULT hr;
		};
	};

	namespace __impl {

		template<typename T>
		struct bag_t: reps::bag_t {
			T data;
		};

	}

	struct observer_t {
		virtual void FASTCALL on(bag_t const& value) noexcept = 0;
	};

	template<typename T>
	inline T& data(bag_t& value) noexcept {
		__impl::bag_t<T>& implbag = *reinterpret_cast<__impl::bag_t<T>*>(&value);
		return implbag.data;
	}

	template<typename T>
	inline T const& data(bag_t const& value) noexcept {
		__impl::bag_t<T> const& implbag = *reinterpret_cast<__impl::bag_t<T> const*>(&value);
		return implbag.data;
	}

}
