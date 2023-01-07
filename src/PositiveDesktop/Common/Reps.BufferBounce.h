#pragma once

// RElationshiPS Library: Micro observer pattern
#include "Common/Reps.h"

#include <winrt/Microsoft.UI.Dispatching.h>

namespace app::ui {

	extern winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue;

}

namespace reps {

#ifdef WINRT_Microsoft_UI_Dispatching_H

	struct timer_sink_t {
	protected:
		template<typename Duration>
		constexpr timer_sink_t(Duration dueTime) noexcept: timer_sink_t(dueTime, app::ui::gDispatchQueue) { }

		template<typename Duration>
		constexpr timer_sink_t(Duration dueTime, winrt::Microsoft::UI::Dispatching::DispatcherQueue const& dispatchQueue) noexcept
			: timer_(nullptr) {
			winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer { dispatchQueue.CreateTimer() };
			timer.Interval(dueTime);
			timer.IsRepeating(false);
			timer_ = timer;
		}

		virtual ~timer_sink_t() {
			winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer { std::exchange(timer_, nullptr) };
			WINRT_ASSERT(timer);
			timer.Stop();
		}

		inline void startTimer() const {
			timer_.Start();
		}

		inline void stopTimer() const {
			timer_.Stop();
		}

	protected:
		winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer_;
	};

	template<typename T>
	struct buffer_debounce_sink_t final: public sink_t<std::vector<T>>, timer_sink_t, basic_observer_t<T> {
	private:
		using sink_type = sink_t<std::vector<T>>;

	public:
		template<typename Duration>
		buffer_debounce_sink_t(observer_t<std::vector<T>> observer, Duration dueTime) noexcept
			: buffer_debounce_sink_t(std::move(observer), dueTime, app::ui::gDispatchQueue) {
		}

		template<typename Duration>
		buffer_debounce_sink_t(
			observer_t<std::vector<T>> observer,
			Duration dueTime,
			winrt::Microsoft::UI::Dispatching::DispatcherQueue const& dispatchQueue) noexcept
			: sink_type(std::move(observer))
			, timer_sink_t(dueTime, dispatchQueue)
			, callbackToken_(timer_.Tick({ this, &buffer_debounce_sink_t<T>::delegate })) {
		}

		virtual ~buffer_debounce_sink_t() {
			winrt::event_token callbackToken { std::exchange(callbackToken_, { }) };
			WINRT_ASSERT(callbackToken.value);
			timer_.Tick(callbackToken);
		}

	private:
		void FASTCALL on(reps::bag_t<T> const& value) noexcept override {
			stopTimer();

			if (value.hr < 0) {
				buffer_.clear();
				sink_type::forwardError(value.hr);
				sink_type::close();
				return;
			}

			switch (value.ev) {
			case reps::event_t::next:
			{
				{
					app::lock_guard lock { locker_ };
					buffer_.emplace_back(std::move(value.data));
				}
				startTimer();
				break;
			}
			case reps::event_t::completed:
			default:
				propagate();
				sink_type::forwardComplete();
				sink_type::close();
				break;
			}
		}

		void delegate(winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer const& /*sender*/, winrt::Windows::Foundation::IInspectable const&) noexcept {
			propagate();
		}

		inline void propagate() noexcept {
			std::vector<T> buffer;
			{
				app::lock_guard lock { locker_ };
				buffer = std::move(buffer_);
			}
			if (!buffer.empty()) {
				sink_type::next(std::move(buffer));
			}
		}

	private:
		app::lock_t locker_;
		winrt::event_token callbackToken_;
		std::vector<T> buffer_;
	};

	template<typename T>
	observer_t<T> make_buffer_debounce(observer_t<std::vector<T>> observer, std::chrono::milliseconds dueTime) noexcept {
		return { new buffer_debounce_sink_t<T>(std::move(observer), std::move(dueTime)), take_ownership };
	}

#endif

}
