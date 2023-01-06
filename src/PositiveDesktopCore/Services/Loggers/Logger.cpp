#include "pch.h"
#include "log_t.h"

#include <boost/circular_buffer.hpp>

#include "Common/Reps.h"

namespace app::logger {

#if _DEBUG
	constexpr std::size_t kMaxLogCount = 1024;
#else
	constexpr std::size_t kMaxLogCount = 64;
#endif

	class Logger: public ILogger, reps::single_subject_t<log_t> {
		using container_type = boost::circular_buffer<log_t>;

	public:
		inline Logger(): logs_(kMaxLogCount) { }
		~Logger() override = default;

		log_t const* first() const noexcept override {
			return logs_.begin().m_it;
		}

		void next(log_t const** ptr) const noexcept override {
			container_type::const_iterator itr(&logs_, *ptr);
			if (++itr == logs_.cend()) {
				*ptr = nullptr;
			} else {
				*ptr = itr.m_it;
			}
		}

		void addObserver(reps::observer_t<log_t> const& observer) noexcept override {
			reps::single_subject_t<log_t>::addObserver(observer);
		}

		void clearObserver() noexcept override {
			reps::single_subject_t<log_t>::clearObserver();
		}

		void log(log_t&& log) noexcept override {
			logs_.push_back(std::move(log));
			reps::single_subject_t<log_t>::next(log);
		}

	private:
		container_type logs_;
	};

	ILogger* gLogger = nullptr;

	void initLogger() noexcept {
		gLogger = new Logger();
	}

}
