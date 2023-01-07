#pragma once

// RElationshiPS Library: Micro observer pattern

namespace reps {

	enum class event_t: unsigned long {
		next = 2,
		completed = 3,
	};

	template<typename T>
	struct bag_t {
		union {
			event_t ev;
			HRESULT hr;
		};
		T data;
	};

	struct take_ownership_t { };
	inline constexpr take_ownership_t take_ownership {};

	template<typename T>
	struct basic_observer_t;

	template<typename T>
	struct observer_t final {
	private:
		enum observer_flag_t { reference, actual };

		observer_t(observer_t<T> const&) = delete;
		observer_t<T>& operator=(observer_t<T> const&) = delete;

	public:
		constexpr observer_t(std::nullptr_t): flag_(reference), observer_(nullptr) { }
		constexpr observer_t(basic_observer_t<T>* observer) noexcept: flag_(reference), observer_(observer) { }
		constexpr observer_t(basic_observer_t<T>* observer, take_ownership_t) noexcept: flag_(actual), observer_(observer) { }

		constexpr observer_t(observer_t<T>&& other) noexcept
			: flag_(other.flag_)
			, observer_(other.observer_) {
			other.flag_ = reference;
			other.observer_ = nullptr;
		}

		constexpr observer_t<T>& operator=(observer_t<T>&& other) noexcept {
			if (&other != this) {
				release();
				flag_ = std::exchange(other.flag_, reference);
				observer_ = std::exchange(other.observer_, nullptr);
			}
			return *this;
		}

		constexpr ~observer_t() {
			release();
		}

		inline void on(bag_t<T> const& value) noexcept {
			observer_->on(value);
		}

		constexpr basic_observer_t<T>* impl() const noexcept {
			return observer_;
		}

		constexpr operator bool() const noexcept {
			return observer_;
		}

		constexpr bool operator ==(observer_t<T> const& other) const noexcept {
			return observer_ == other.observer_;
		}

		constexpr bool operator !=(observer_t<T> const& other) const noexcept {
			return observer_ != other.observer_;
		}

	private:
		constexpr void release() noexcept {
			if (actual == flag_) {
				delete observer_;
			}
		}

	private:
		observer_flag_t flag_;
		basic_observer_t<T>* observer_;
	};

	template<typename T>
	struct basic_observer_t {
		virtual ~basic_observer_t() = default;
		virtual void FASTCALL on(bag_t<T> const& value) noexcept = 0;

		constexpr observer_t<T> observer() noexcept {
			return observer_t<T>(this);
		}
	};

}
