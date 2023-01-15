#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace app {

	template<typename Msg>
	struct message_service_t {
	protected:
		void initialize() noexcept {
			std::thread thread { std::bind(&message_service_t::proc, this) };
			procThread_ = std::move(thread);
		}

		void close() noexcept {
			if (terminating_) return;
			{
				std::lock_guard<std::mutex> locker(mutex_);
				terminating_ = true;
			}
			procThread_.join();
		}

		void terminate() noexcept {
			std::lock_guard<std::mutex> locker(mutex_);
			terminating_ = true;
		}

		void terminateWithoutLock() noexcept {
			terminating_ = true;
		}

		void send(Msg msg) {
			{
				std::lock_guard<std::mutex> locker(mutex_);
				queue_.emplace(msg);
			}
			cond_.notify_one();
		}

		virtual void process(Msg msg) noexcept = 0;

	private:
		void proc() noexcept {
			HANDLE hThread { GetCurrentThread() };

			std::unique_lock<std::mutex> locker(mutex_);
			while (!terminating_) {
				winrt::check_bool(SetThreadPriority(hThread, THREAD_MODE_BACKGROUND_BEGIN));
				cond_.wait(locker);
				winrt::check_bool(SetThreadPriority(hThread, THREAD_MODE_BACKGROUND_END));
				while (!queue_.empty()) {
					Msg msg = queue_.front();
					process(std::move(msg));
					queue_.pop();
				}
			}
		}

	private:
		bool terminating_ { false };
		std::queue<Msg> queue_;
		std::mutex mutex_;
		std::condition_variable cond_;
		std::thread procThread_;
	};

}
