#include "pch.h"
#include "Reps.h"

using namespace reps;

void subject_t::send(bag_t&& value) noexcept {
	app::lock_guard<app::lock_t> lock { locker_ };
	for (observer_t const* observer : observers_) {
		const_cast<observer_t*>(observer)->on(value);
	}
}

void single_subject_t::send(bag_t&& value) noexcept {
	app::lock_guard<app::lock_t> lock { locker_ };
	if (observer_) {
		const_cast<observer_t*>(observer_)->on(value);
	}
}

void buffered_subject_t::send(bag_t&& value) noexcept {
	app::lock_guard<app::lock_t> lock { locker_ };
	cache_ = std::move(value);
	for (observer_t const* observer : observers_) {
		const_cast<observer_t*>(observer)->on(cache_);
	}
}

void single_buffered_subject_t::send(bag_t&& value) noexcept {
	app::lock_guard<app::lock_t> lock { locker_ };
	cache_ = std::move(value);
	if (observer_) {
		const_cast<observer_t*>(observer_)->on(cache_);
	}
}
