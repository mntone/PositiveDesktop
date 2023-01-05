#include "pch.h"
#include "log_t.h"

#include <boost/circular_buffer.hpp>

namespace app::logger {

#if _DEBUG
	constexpr std::size_t kMaxLogCount = 1024;
#else
	constexpr std::size_t kMaxLogCount = 64;
#endif

	class Logger: public ILogger {
	public:
		inline Logger(): logs_(kMaxLogCount) { }
		~Logger() override = default;

		void log(log_t&& log) noexcept override {
			logs_.push_back(std::move(log));
		}

	private:
		boost::circular_buffer<log_t> logs_;
	};

	ILogger* gLogger = nullptr;

	void initLogger() noexcept {
		gLogger = new Logger();
	}

}
