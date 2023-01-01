#pragma once

#include <functional>

// RElationshiPS Library: Micro observer pattern
#include "Reps.h"

namespace reps {

	class listener_t final: public reps::observer_t {
	public:
		listener_t(std::function<void(reps::bag_t const&)> callback) noexcept: callback_(callback) { }

	private:
		void FASTCALL on(reps::bag_t const& value) noexcept override {
			callback_(value);
		}

	private:
		std::function<void(reps::bag_t const&)> callback_;
	};

	class dangerous_listener_t final: public reps::observer_t {
	public:
		dangerous_listener_t(std::function<void(reps::bag_t const&)> callback) noexcept: callback_(callback) { }

	private:
		void FASTCALL on(reps::bag_t const& value) noexcept override {
			callback_(value);
			if (reps::event_t::completed == value.ev) {
				delete this;
			}
		}

	private:
		std::function<void(reps::bag_t const&)> callback_;
	};

}
