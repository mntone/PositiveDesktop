#pragma once

// RElationshiPS Library: Micro observer pattern
#include "RepsBase.h"

namespace reps {

	static inline void swap(observer_t const*& a, observer_t const*& b) noexcept {
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

		struct lock_t {
			enum state { unlocked = 0, locked };

			inline void lock() noexcept {
				while (state_.exchange(locked, std::memory_order_acquire) == locked) {
					Sleep(0);
				}
			}

			inline void unlock() noexcept {
				state_.store(unlocked, std::memory_order_release);
			}

		private:
			std::atomic<state> state_;
		};


		template <class lock_t>
		class _NODISCARD_LOCK lock_guard {
		public:
			explicit lock_guard(lock_t& locker): locker_(locker) {
				locker_.lock();
			}

			~lock_guard() noexcept {
				locker_.unlock();
			}

			lock_guard(const lock_guard&) = delete;
			lock_guard& operator=(const lock_guard&) = delete;

		private:
			lock_t& locker_;
		};

		struct observer_impl {
		protected:
			inline void _setObserver(observer_t const* observer) noexcept {
				observers_.push_back(observer);
			}

			inline void _clearObserver() noexcept {
				observers_.clear();
			}

			std::vector<observer_t const*> observers_;
		};

		struct single_observer_impl {
		protected:
			inline void _setObserver(observer_t const* observer) noexcept {
				using std::swap;
				swap(observer_, observer);
			}

			inline void _clearObserver() noexcept {
				_setObserver(nullptr);
			}

			observer_t const* observer_;
		};

	}

	struct subject_t: observable_t, __impl::observer_impl {
		void send(bag_t&& value) noexcept override;

		void FASTCALL addObserver(observer_t const& observer) noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_setObserver(&observer);
		}

		void clearObserver() noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		__impl::lock_t locker_;
	};

	struct single_subject_t: observable_t, __impl::single_observer_impl {
	public:
		void send(bag_t&& value) noexcept override;

		void FASTCALL addObserver(observer_t const& observer) noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_setObserver(&observer);
		}

		void clearObserver() noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		__impl::lock_t locker_;
	};

	struct buffered_subject_t: observable_t, __impl::observer_impl {
		buffered_subject_t() = delete;
		buffered_subject_t(bag_t&& init) noexcept: cache_(init) {}

		void send(bag_t&& value) noexcept override;

		template<typename T>
		void FASTCALL addObserver(observer_t const& observer) noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_setObserver(&observer);

			__impl::bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			send(std::move(bag));
		}

		void clearObserver() noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		__impl::lock_t locker_;
		bag_t cache_;
	};

	struct single_buffered_subject_t: observable_t, __impl::single_observer_impl {
		single_buffered_subject_t() = delete;
		single_buffered_subject_t(bag_t&& init) noexcept: cache_(init) { }

		void send(bag_t&& value) noexcept override;

		template<typename T>
		void FASTCALL addObserver(observer_t const& observer) noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_setObserver(&observer);

			__impl::bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			send(std::move(bag));
		}

		void clearObserver() noexcept {
			__impl::lock_guard<__impl::lock_t> lock { locker_ };
			_clearObserver();
		}

	private:
		__impl::lock_t locker_;
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
