#pragma once

// RElationshiPS Library: Micro observer pattern
#include "Common/Reps.h"

#include <winrt/Microsoft.UI.Dispatching.h>

namespace app::ui {

	extern winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue;

}

namespace reps {

	template<typename T>
	struct buffer_debounce_sink_t final: public sink_t<std::vector<T>>, basic_observer_t<T> {
	private:
		using sink_type = sink_t<std::vector<T>>;

	public:
		buffer_debounce_sink_t(observer_t<std::vector<T>> observer, std::chrono::milliseconds dueTime) noexcept
			: buffer_debounce_sink_t(std::move(observer), dueTime, app::ui::gDispatchQueue) {
		}

		buffer_debounce_sink_t(
			observer_t<std::vector<T>> observer,
			std::chrono::milliseconds dueTime,
			winrt::Microsoft::UI::Dispatching::DispatcherQueue dispatchQueue) noexcept
			: sink_type(std::move(observer))
			, dispatchQueue_(dispatchQueue)
			, timer_(nullptr)
			, callbackToken_()
			, dueTime_(dueTime) {
		}

		~buffer_debounce_sink_t() {
			closeTimer();
		}

	private:
		void FASTCALL on(reps::bag_t<T> const& value) noexcept override {
			if (value.hr < 0) {
				buffer_.clear();
				sink_type::forwardError(value.hr);
				sink_type::close();
				return;
			}

			switch (value.ev) {
			case reps::event_t::next:
			{
				app::lock_guard lock { locker_ };
				buffer_.emplace_back(std::move(value.data));
			}
			createTimer();
			break;
			case reps::event_t::completed:
			default:
				closeTimer();
				propagate();
				sink_type::forwardComplete();
				sink_type::close();
				break;
			}
		}

		inline void createTimer() noexcept {
			winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer { dispatchQueue_.CreateTimer() };
			timer.Interval(dueTime_);
			timer.IsRepeating(false);
			callbackToken_ = timer.Tick({ this, &buffer_debounce_sink_t<T>::delegate });
			timer.Start();
			timer_ = timer;
		}

		inline void closeTimer() noexcept {
			winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer { std::exchange(timer_, nullptr) };
			if (timer) {
				timer.Stop();

				winrt::event_token callbackToken { std::exchange(callbackToken_, { }) };
				if (callbackToken) {
					timer.Tick(callbackToken);
				}
			}
		}

		void delegate(winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer const& sender, winrt::Windows::Foundation::IInspectable const&) noexcept {
			winrt::event_token callbackToken { std::exchange(callbackToken_, { }) };
			if (callbackToken) {
				sender.Tick(callbackToken);
			}
			timer_ = nullptr;

			propagate();
		}

		inline void propagate() noexcept {
			app::lock_guard lock { locker_ };
			std::vector<T> buffer { std::move(buffer_) };
			if (!buffer.empty()) {
				sink_type::next(std::move(buffer));
			}
		}

	private:
		app::lock_t locker_;
		winrt::Microsoft::UI::Dispatching::DispatcherQueue dispatchQueue_;
		winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer_;
		winrt::event_token callbackToken_;
		std::chrono::milliseconds dueTime_;
		std::vector<T> buffer_;
	};

	template<typename T>
	observer_t<T> make_buffer_debounce(observer_t<std::vector<T>> observer, std::chrono::milliseconds dueTime) noexcept {
		return { new buffer_debounce_sink_t<T>(std::move(observer), std::move(dueTime)), take_ownership };
	}

}
