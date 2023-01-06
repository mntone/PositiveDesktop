#pragma once
#include "lock_t.h"

#include <vector>

// RElationshiPS Library: Micro observer pattern
#include "RepsBase.h"

namespace reps {

	template<typename T>
	struct observable_t { };

	namespace __impl {

		template<typename T>
		static constexpr void swap(observer_t<T> const*& a, observer_t<T> const*& b) noexcept {
			observer_t<T> const* temp = std::move(a);
			a = std::move(b);
			b = std::move(temp);
		}

		template<typename T>
		struct observer_impl {
		protected:
			constexpr void _setObserver(observer_t<T> const* observer) noexcept {
				observers_.push_back(observer);
			}

			constexpr void _clearObserver() noexcept {
				observers_.clear();
			}

			std::vector<observer_t<T> const*> observers_;
		};

		template<typename T>
		struct single_observer_impl {
		protected:
			constexpr single_observer_impl(): observer_(nullptr) { }

			constexpr void _setObserver(observer_t<T> const* observer) noexcept {
				using std::swap;
				swap(observer_, observer);
			}

			constexpr void _clearObserver() noexcept {
				_setObserver(nullptr);
			}

			observer_t<T> const* observer_;
		};

	}

	template<typename T>
	struct subject_t: public observable_t<T>, __impl::observer_impl<T> {
		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			for (observer_t<T> const* observer : __impl::observer_impl<T>::observers_) {
				const_cast<observer_t<T>*>(observer)->on(val);
			}
		}

		inline void next(T val) noexcept {
			bag_t<T> bag {
				event_t::next,
				val,
			};
			send(std::move(bag));
		}

		inline void error(HRESULT hr) noexcept {
			bag_t<T> bag {};
			bag.hr = hr;
			send(std::move(bag));
		}

		inline void complete() noexcept {
			bag_t<T> bag {
				event_t::completed,
			};
			send(std::move(bag));
		}

		inline void addObserver(observer_t<T> const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::observer_impl<T>::_setObserver(&observer);
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			observable_t<T>::complete();
			__impl::observer_impl<T>::_clearObserver();
		}

	private:
		app::lock_t locker_;
	};

	template<typename T>
	struct single_subject_t: public observable_t<T>, __impl::single_observer_impl<T> {
		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			observer_t<T> const* observer = __impl::single_observer_impl<T>::observer_;
			if (observer) {
				const_cast<observer_t<T>*>(observer)->on(val);
			}
		}

		inline void next(T val) noexcept {
			bag_t<T> bag {
				event_t::next,
				val,
			};
			send(std::move(bag));
		}

		inline void error(HRESULT hr) noexcept {
			bag_t<T> bag {};
			bag.hr = hr;
			send(std::move(bag));
		}

		inline void complete() noexcept {
			bag_t<T> bag {
				event_t::completed,
			};
			send(std::move(bag));
		}

		inline void addObserver(observer_t<T> const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::single_observer_impl<T>::_setObserver(&observer);
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			complete();
			__impl::single_observer_impl<T>::_clearObserver();
		}

	private:
		app::lock_t locker_;
	};

	template<typename T>
	struct buffered_subject_t: public observable_t<T>, __impl::observer_impl<T> {
		buffered_subject_t() = delete;
		constexpr buffered_subject_t(bag_t<T>&& init) noexcept: cache_(std::move(init)) { }

		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			cache_ = std::move(val);
			for (observer_t<T> const* observer : __impl::observer_impl<T>::observers_) {
				const_cast<observer_t<T>*>(observer)->on(cache_);
			}
		}

		inline void next(T val) noexcept {
			bag_t<T> bag {
				event_t::next,
				val,
			};
			send(std::move(bag));
		}

		inline void error(HRESULT hr) noexcept {
			bag_t<T> bag {};
			bag.hr = hr;
			send(std::move(bag));
		}

		inline void complete() noexcept {
			bag_t<T> bag {
				event_t::completed,
			};
			send(std::move(bag));
		}

		inline void addObserver(observer_t<T> const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::observer_impl<T>::_setObserver(&observer);

			bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			observer.send(std::move(bag));
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			complete();
			__impl::observer_impl<T>::_clearObserver();
		}

	private:
		app::lock_t locker_;
		bag_t<T> cache_;
	};

	template<typename T>
	struct single_buffered_subject_t: public observable_t<T>, __impl::single_observer_impl<T> {
		single_buffered_subject_t() = delete;
		constexpr single_buffered_subject_t(bag_t<T>&& init) noexcept: cache_(std::move(init)) { }

		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			cache_ = std::move(val);

			observer_t<T>* observer = __impl::single_observer_impl<T>::observer_;
			if (observer) {
				const_cast<observer_t<T>*>(observer)->on(cache_);
			}
		}

		inline void next(T val) noexcept {
			bag_t<T> bag {
				event_t::next,
				val,
			};
			send(std::move(bag));
		}

		inline void error(HRESULT hr) noexcept {
			bag_t<T> bag {};
			bag.hr = hr;
			send(std::move(bag));
		}

		inline void complete() noexcept {
			bag_t<T> bag {
				event_t::completed,
			};
			send(std::move(bag));
		}

		inline void addObserver(observer_t<T> const& observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::single_observer_impl<T>::_setObserver(&observer);

			bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			observer.send(std::move(bag));
		}

		inline void clearObserver() noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			observable_t<T>::complete();
			__impl::single_observer_impl<T>::_clearObserver();
		}

	private:
		app::lock_t locker_;
		bag_t<T> cache_;
	};

	template<typename T>
	inline void next(observable_t<T>& observable, T const value) noexcept {
		observable.next(std::move(value));
	}

	template<typename T>
	inline void error(observable_t<T>& observable, HRESULT hr) noexcept {
		observable.error(hr);
	}

	template<typename T>
	inline void complete(observable_t<T>& observable) noexcept {
		observable.complete();
	}
}
