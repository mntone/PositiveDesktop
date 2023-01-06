#pragma once

#include <cstddef>
#include <chrono>
#include <string_view>

#include <winerror.h>
#include <unknwn.h>
#include <winrt/base.h>

#include "Common/RepsBase.h"

namespace app::logger {

	enum loglvl_t: unsigned int {
		llv_unknown,
		llv_trace, // Show this level and upper on UI for debug build
		llv_debug,
		llv_info,  // Show this level and upper on UI for release build
		llv_warn,  // Write this level and upper to the log file for debug build
		llv_error, // Write this level and upper to the log file for release build
		llv_fatal,
	};

	enum logtag_t: unsigned int {
		ltg_unknown,
		ltg_desktop,
		ltg_desktop_listener,
		ltg_keylistener,
		ltg_logger,
		ltg_storage,
		ltg_presenter,
	};

	enum logop_t: unsigned int {
		lop_unknown,
		lop_function_begin,
		lop_function_end,
	};

	struct log_t final {
		loglvl_t level : 3;
		logtag_t tag : 3;
		logop_t op : 2;
		unsigned short line;
		HRESULT hr;
		std::chrono::system_clock::time_point datetime;
		std::string_view filename;
		std::string_view funcname;
		std::string_view message;
	};

	class ILogger {
	public:
		virtual ~ILogger() = default;

		virtual log_t const* first() const noexcept = 0;
		virtual void next(log_t const** ptr) const noexcept = 0;
		virtual void addObserver(reps::observer_t<log_t> observer) noexcept = 0;
		virtual void clearObserver() noexcept = 0;

		virtual void log(log_t&& log) noexcept = 0;

#if _DEBUG
		inline void trace(logtag_t tag, logop_t op, long line, std::string_view filename, std::string_view funcname, std::string_view message) noexcept {
			log_t data {
				llv_trace,
				tag,
				op,
				static_cast<unsigned short>(line),
				S_OK,
				std::chrono::system_clock::now(),
				filename,
				funcname,
				message,
			};
			log(std::move(data));
		}

		inline void debug(logtag_t tag, long line, std::string_view filename, std::string_view funcname, std::string_view message) noexcept {
			log_t data {
				llv_debug,
				tag,
				lop_unknown,
				static_cast<unsigned short>(line),
				S_OK,
				std::chrono::system_clock::now(),
				filename,
				funcname,
				message,
			};
			log(std::move(data));
		}
#endif

		inline void log(loglvl_t type, logtag_t tag, HRESULT hr, long line, std::string_view filename, std::string_view funcname, std::string_view message) noexcept {
			log_t data {
				type,
				tag,
				lop_unknown,
				static_cast<unsigned short>(line),
				hr,
				std::chrono::system_clock::now(),
				filename,
				funcname,
				message,
			};
			log(std::move(data));
		}
	};

	extern ILogger* gLogger;

	void initLogger() noexcept;

#define __LABEL_FINALIZE finalize

#if _DEBUG
#define LOG_TRACE(__OP__, __MSG__) ::app::logger::gLogger->trace(__tag, __OP__, __LINE__, __FILE__, __FUNCTION__, __MSG__)
#define LOG_DEBUG(__MSG__)         ::app::logger::gLogger->debug(__tag, __LINE__, __FILE__, __FUNCTION__, __MSG__)
#else
#define LOG_TRACE(__OP__, __MSG__) static_cast<void>(0)
#define LOG_DEBUG(__MSG__)         static_cast<void>(0)
#endif

#define LOG_TRACE_BEGIN(__TAG__) constexpr ::app::logger::logtag_t __tag = __TAG__; LOG_TRACE(::app::logger::lop_function_begin, "")
#define LOG_TRACE_END()          __LABEL_FINALIZE: LOG_TRACE(::app::logger::lop_function_end, "")
#define LOG_TRACE_END_NOLABEL()  LOG_TRACE(::app::logger::lop_function_end, "")
#define LOG_TRACE_BEGIN_INTHROW(__TAG__) constexpr ::app::logger::logtag_t __tag = __TAG__; LOG_TRACE(::app::logger::lop_function_begin, ""); try { static_cast<void>(0)
#define LOG_TRACE_END_INTHROW()          LOG_TRACE(::app::logger::lop_function_end, ""); } catch (...) { LOG_TRACE(::app::logger::lop_function_end, ""); throw; } static_cast<void>(0)

#define LOG_WARN(__MSG__, __HR__)  ::app::logger::gLogger->warn(__tag, __HR__, __LINE__, __FILE__, __FUNCTION__, __MSG__)
#define LOG_ERROR(__MSG__, __HR__) ::app::logger::gLogger->error(__tag, __HR__, __LINE__, __FILE__, __FUNCTION__, __MSG__)

#define __CHECK_BOOL_PASS(__LVL__, __RET__, __MSG__) \
	if (!(__RET__)) { \
		HRESULT hr = ::winrt::impl::hresult_from_win32(GetLastError()); \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
	} \
	static_cast<void>(0)
#define CHECK_INFO_BOOL_PASS(__RET__, __MSG__)  __CHECK_BOOL_PASS(info, __RET__, __MSG__)
#define CHECK_WARN_BOOL_PASS(__RET__, __MSG__)  __CHECK_BOOL_PASS(warn, __RET__, __MSG__)
#define CHECK_ERROR_BOOL_PASS(__RET__, __MSG__) __CHECK_BOOL_PASS(error, __RET__, __MSG__)
#define CHECK_FATAL_BOOL_PASS(__RET__, __MSG__) __CHECK_BOOL_PASS(fatal, __RET__, __MSG__)

#define __CHECK_BOOL_THROW(__LVL__, __RET__, __MSG__) \
	if (!(__RET__)) { \
		HRESULT hr = ::winrt::impl::hresult_from_win32(GetLastError()); \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		::winrt::throw_hresult(hr); \
	} \
	static_cast<void>(0)
#define CHECK_INFO_BOOL_THROW(__RET__, __MSG__)  __CHECK_BOOL_THROW(info, __RET__, __MSG__)
#define CHECK_WARN_BOOL_THROW(__RET__, __MSG__)  __CHECK_BOOL_THROW(warn, __RET__, __MSG__)
#define CHECK_ERROR_BOOL_THROW(__RET__, __MSG__) __CHECK_BOOL_THROW(error, __RET__, __MSG__)
#define CHECK_FATAL_BOOL_THROW(__RET__, __MSG__) __CHECK_BOOL_THROW(fatal, __RET__, __MSG__)

#define __CHECK_BOOL_GOTO(__LVL__, __RET__, __MSG__) \
	if (!(__RET__)) { \
		HRESULT hr = ::winrt::impl::hresult_from_win32(GetLastError()); \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		goto __LABEL_FINALIZE; \
	} \
	static_cast<void>(0)
#define CHECK_INFO_BOOL_GOTO(__RET__, __MSG__)  __CHECK_BOOL_GOTO(info, __RET__, __MSG__)
#define CHECK_WARN_BOOL_GOTO(__RET__, __MSG__)  __CHECK_BOOL_GOTO(warn, __RET__, __MSG__)
#define CHECK_ERROR_BOOL_GOTO(__RET__, __MSG__) __CHECK_BOOL_GOTO(error, __RET__, __MSG__)
#define CHECK_FATAL_BOOL_GOTO(__RET__, __MSG__) __CHECK_BOOL_GOTO(fatal, __RET__, __MSG__)

#define __CHECK_HRESULT_PASS(__LVL__, __RET__, __MSG__) \
	{ \
		HRESULT hr = __RET__; \
		if (FAILED(hr)) { \
			::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		} \
	} \
	static_cast<void>(0)
#define CHECK_INFO_HRESULT_PASS(__RET__, __MSG__)  __CHECK_HRESULT_PASS(info, __RET__, __MSG__)
#define CHECK_WARN_HRESULT_PASS(__RET__, __MSG__)  __CHECK_HRESULT_PASS(warn, __RET__, __MSG__)
#define CHECK_ERROR_HRESULT_PASS(__RET__, __MSG__) __CHECK_HRESULT_PASS(error, __RET__, __MSG__)
#define CHECK_FATAL_HRESULT_PASS(__RET__, __MSG__) __CHECK_HRESULT_PASS(fatal, __RET__, __MSG__)

#define __CHECK_HRESULT_GOTO(__LVL__, __RET__, __MSG__) \
	{ \
		HRESULT hr = __RET__; \
		if (FAILED(hr)) { \
			::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
			goto __LABEL_FINALIZE; \
		} \
	} \
	static_cast<void>(0)
#define CHECK_INFO_HRESULT_GOTO(__RET__, __MSG__)  __CHECK_HRESULT_GOTO(info, __RET__, __MSG__)
#define CHECK_WARN_HRESULT_GOTO(__RET__, __MSG__)  __CHECK_HRESULT_GOTO(warn, __RET__, __MSG__)
#define CHECK_ERROR_HRESULT_GOTO(__RET__, __MSG__) __CHECK_HRESULT_GOTO(error, __RET__, __MSG__)
#define CHECK_FATAL_HRESULT_GOTO(__RET__, __MSG__) __CHECK_HRESULT_GOTO(fatal, __RET__, __MSG__)

#define __CHECK_HRESULT_THROW(__LVL__, __RET__, __MSG__) \
	{ \
		HRESULT hr = __RET__; \
		if (FAILED(hr)) { \
			::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
			::winrt::throw_hresult(hr); \
		} \
	} \
	static_cast<void>(0)
#define CHECK_INFO_HRESULT_THROW(__RET__, __MSG__)  __CHECK_HRESULT_THROW(info, __RET__, __MSG__)
#define CHECK_WARN_HRESULT_THROW(__RET__, __MSG__)  __CHECK_HRESULT_THROW(warn, __RET__, __MSG__)
#define CHECK_ERROR_HRESULT_THROW(__RET__, __MSG__) __CHECK_HRESULT_THROW(error, __RET__, __MSG__)
#define CHECK_FATAL_HRESULT_THROW(__RET__, __MSG__) __CHECK_HRESULT_THROW(fatal, __RET__, __MSG__)

}
