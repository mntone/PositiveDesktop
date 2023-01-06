#pragma once
#include "lock_t.h"

#include <vector>

// RElationshiPS Library: Micro observer pattern
#include "RepsBase.h"

namespace reps {

	template<typename T>
	struct observable_t { };

	using observer_token = void*;

	namespace __impl {

		template<typename T>
		struct observer_impl {
		protected:
			using item_type = observer_t<T>;
			using container_type = std::vector<item_type>;

		protected:
			constexpr observer_token _addObserver(item_type observer) noexcept {
				observer_token token { observer.impl() };
				observers_.emplace_back(std::move(observer));
				return token;
			}

			constexpr void _removeObserver(observer_token token) noexcept {
				typename container_type::const_iterator itr = std::find([](observer_t<T> const& ob) { return ob.impl(); }, observers_.cbegin(), observers_.cend(), token);
				if (itr != observers_.cend()) {
					observers_.erase(itr);
				}
			}

			constexpr void _clearObserver() noexcept {
				observers_.clear();
			}

			container_type observers_;
		};

		template<typename T>
		struct single_observer_impl {
		protected:
			using item_type = observer_t<T>;

		protected:
			constexpr single_observer_impl(): observer_(nullptr) { }

			constexpr observer_token _setObserver(item_type observer) noexcept {
				observer_ = std::move(observer);
				return observer_.impl();
			}

			constexpr void _removeObserver(observer_token token) noexcept {
				if (token == static_cast<observer_token>(observer_.impl())) {
					_clearObserver();
				}
			}

			constexpr void _clearObserver() noexcept {
				item_type nullItem { nullptr };
				observer_ = std::move(nullItem);
			}

			item_type observer_;
		};

	}

	template<typename T>
	struct subject_t: observable_t<T>, __impl::observer_impl<T> {
		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			for (observer_t<T> const& observer : __impl::observer_impl<T>::observers_) {
				observer.on(val);
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

		inline observer_token addObserver(observer_t<T> observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			return __impl::observer_impl<T>::_addObserver(std::move(observer));
		}

		inline void removeObserver(observer_token token) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::observer_impl<T>::_removeObserver(token);
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
	struct single_subject_t: observable_t<T>, __impl::single_observer_impl<T> {
		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			observer_t<T>& observer = __impl::single_observer_impl<T>::observer_;
			if (observer) {
				observer.on(val);
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

		inline observer_token addObserver(observer_t<T> observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			return __impl::single_observer_impl<T>::_setObserver(std::move(observer));
		}

		inline void removeObserver(observer_token token) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::observer_impl<T>::_removeObserver(token);
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
	struct buffered_subject_t: observable_t<T>, __impl::observer_impl<T> {
		buffered_subject_t() = delete;
		constexpr buffered_subject_t(bag_t<T>&& init) noexcept: cache_(std::move(init)) { }

		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			cache_ = std::move(val);
			for (observer_t<T> const& observer : __impl::observer_impl<T>::observers_) {
				observer.on(cache_);
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

		inline observer_token addObserver(observer_t<T> observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			observer_token token { __impl::observer_impl<T>::_setObserver(std::move(observer)) };

			bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			observer.send(std::move(bag));
			return token;
		}

		inline void removeObserver(observer_token token) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::observer_impl<T>::_removeObserver(token);
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
	struct single_buffered_subject_t: observable_t<T>, __impl::single_observer_impl<T> {
		single_buffered_subject_t() = delete;
		constexpr single_buffered_subject_t(bag_t<T>&& init) noexcept: cache_(std::move(init)) { }

		inline void send(bag_t<T>&& val) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			cache_ = std::move(val);

			observer_t<T>& observer = __impl::single_observer_impl<T>::observer_;
			if (observer) {
				observer.on(cache_);
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

		inline observer_token addObserver(observer_t<T> observer) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			observer_token token { __impl::single_observer_impl<T>::_setObserver(observer) };

			bag_t<T> bag;
			memcpy(&bag, &cache_, sizeof(T));
			observer.send(std::move(bag));
			return token;
		}

		inline void removeObserver(observer_token token) noexcept {
			app::lock_guard<app::lock_t> lock { locker_ };
			__impl::observer_impl<T>::_removeObserver(token);
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
	struct sink_t: single_subject_t<T> {
	private:
		using subject_type = single_subject_t<T>;

	public:
		sink_t(observer_t<T> observer) noexcept {
			subject_type::addObserver(std::move(observer));
		}

		~sink_t() {
			subject_type::clearObserver();
		}

	protected:
		inline void forwardNext(T&& val) noexcept {
			bag_t<T> bag {
				event_t::next,
				val,
			};
			subject_type::send(std::move(bag));
		}

		inline void forwardError(HRESULT hr) noexcept {
			bag_t<T> bag {};
			bag.hr = hr;
			subject_type::send(std::move(bag));
		}

		inline void forwardComplete() noexcept {
			bag_t<T> bag {
				event_t::completed,
			};
			subject_type::send(std::move(bag));
		}

		constexpr bool closed() const noexcept { return closed_; }

	protected:
		constexpr void close() noexcept { closed_ = true; }

	private:
		bool closed_;
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
