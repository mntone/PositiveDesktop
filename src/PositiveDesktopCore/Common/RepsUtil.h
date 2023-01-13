#pragma once

#include <functional>

// RElationshiPS Library: Micro observer pattern
#include "Reps.h"

namespace reps {

	template<typename T>
	class listener_t final: public reps::basic_observer_t<T> {
	public:
		listener_t(std::function<void(reps::bag_t<T> const&)> callback) noexcept
			: callback_(callback) {
		}

	private:
		void FASTCALL on(reps::bag_t<T> const& value) noexcept override {
			callback_(value);
		}

	private:
		std::function<void(reps::bag_t<T> const&)> callback_;
	};

	template<typename T>
	observer_t<T> make_listener(std::function<void(reps::bag_t<T> const&)> callback) noexcept {
		return { new listener_t<T>(callback), take_ownership };
	}

}
