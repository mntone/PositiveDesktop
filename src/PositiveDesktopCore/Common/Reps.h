#pragma once
#include "lock_t.h"

#include <vector>

// RElationshiPS Library: Micro observer pattern
#include "RepsBase.h"

namespace reps {

	static constexpr void swap(observer_t const*& a, observer_t const*& b) noexcept {
		observer_t const* temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	struct observable_t {
		virtual void send(bag_t&& value) noexcept = 0;

		inline void error(HRESULT hr) noexcept {
			bag_t bag {};
			bag.hr = hr;
			send(std::move(bag));
		}

		inline void complete() noexcept {
			bag_t bag {
				event_t::completed,
			};
			send(std::move(bag));
		}
	};

	namespace __impl {

		struct observer_impl {
		protected:
			constexpr void _setObserver(observer_t const* observer) noexcept {
				observers_.push_back(observer);
			}

			constexpr void _clearObserver() noexcept {
				observers_.clear();
			}

			std::vector<observer_t const*> observers_;
		};

		struct single_observer_impl {
		protected:
			constexpr void _setObserver(observer_t const* observer) noexcept {
				using std::swap;
				swap(observer_, observer);
			}

			constexpr void _clearObserver() noexcept {
				_setObserver(nullptr);
			}

			observer_t const* observer_;
		};

	}

	struct subject_t: observable_t, __impl::observer_impl {
		void send(bag_t&& value) noexcept override;

		inline void addObserver(observer_t const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_setObserver(&observer);
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		app::lock_t locker_;
	};

	struct single_subject_t: observable_t, __impl::single_observer_impl {
	public:
		void send(bag_t&& value) noexcept override;

		inline void addObserver(observer_t const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_setObserver(&observer);
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		app::lock_t locker_;
	};

	struct buffered_subject_t: observable_t, __impl::observer_impl {
		buffered_subject_t() = delete;
		constexpr buffered_subject_t(bag_t&& init) noexcept: cache_(std::move(init)) {}

		void send(bag_t&& value) noexcept override;

		template<typename T>
		inline void addObserver(observer_t const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_setObserver(&observer);

			__impl::bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			send(std::move(bag));
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		app::lock_t locker_;
		bag_t cache_;
	};

	struct single_buffered_subject_t: observable_t, __impl::single_observer_impl {
		single_buffered_subject_t() = delete;
		constexpr single_buffered_subject_t(bag_t&& init) noexcept: cache_(std::move(init)) { }

		void send(bag_t&& value) noexcept override;

		template<typename T>
		inline void addObserver(observer_t const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_setObserver(&observer);

			__impl::bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			send(std::move(bag));
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		app::lock_t locker_;
		bag_t cache_;
	};

	template<typename T>
	inline void next(observable_t& observable, T const value) noexcept {
		__impl::bag_t<T> bag {
			event_t::next,
			value,
		};
		observable.send(std::move(bag));
	}

	inline void error(observable_t& observable, HRESULT hr) noexcept {
		observable.error(hr);
	}

	inline void complete(observable_t& observable) noexcept {
		observable.complete();
	}
}