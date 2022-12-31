#pragma once

#include <atomic>

namespace app {

	struct lock_t {
		inline void lock() noexcept {
			bool expected = false;
			_ASSERTE(flag_.compare_exchange_strong(expected, true, std::memory_order_acquire));
		}

		inline void unlock() noexcept {
			flag_.store(false, std::memory_order_release);
		}

	private:
		std::atomic_bool flag_;
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

}
