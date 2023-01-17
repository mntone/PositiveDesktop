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
		ltg_viewmodel,
		ltg_presenter,
	};

	enum logop_t: unsigned int {
		lop_unknown,
		lop_info,
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
#if _DEBUG
		std::string message;
#else
		std::string_view message;
#endif
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
				std::string { message },
			};
			log(std::move(data));
		}

		inline void debug(logtag_t tag, long line, std::string_view filename, std::string_view funcname, std::string message) noexcept {
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
#if _DEBUG
				std::string { message },
#else
				message,
#endif
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

#define LOG_TAG(__TAG__)   constexpr ::app::logger::logtag_t __tag = __TAG__; static_cast<void>(0)
#define LOG_BEGIN(__TAG__) constexpr ::app::logger::logtag_t __tag = __TAG__; LOG_TRACE(::app::logger::lop_function_begin, "")
#define LOG_END()          __LABEL_FINALIZE: LOG_TRACE(::app::logger::lop_function_end, "")
#define LOG_END_NOLABEL()  LOG_TRACE(::app::logger::lop_function_end, "")
#define LOG_BEGIN_INTHROW(__TAG__) constexpr ::app::logger::logtag_t __tag = __TAG__; LOG_TRACE(::app::logger::lop_function_begin, ""); try { static_cast<void>(0)
#define LOG_END_INTHROW()          LOG_TRACE(::app::logger::lop_function_end, ""); } catch (...) { LOG_TRACE(::app::logger::lop_function_end, ""); throw; } static_cast<void>(0)

#define LOG_WARN(__MSG__, __HR__)  ::app::logger::gLogger->log(::app::logger::llv_warn, __tag, __HR__, __LINE__, __FILE__, __FUNCTION__, __MSG__)
#define LOG_ERROR(__MSG__, __HR__) ::app::logger::gLogger->log(::app::logger::llv_error, __tag, __HR__, __LINE__, __FILE__, __FUNCTION__, __MSG__)

#define __LOG_IF_BOOL(__LVL__, __FLAG__, __MSG__) \
	if (!(__FLAG__)) { \
		HRESULT hr = ::winrt::impl::hresult_from_win32(GetLastError()); \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
	} \
	static_cast<void>(0)
#define LOG_IF_BOOL_INFO(__FLAG__, __MSG__)  __LOG_IF_BOOL(info, __FLAG__, __MSG__)
#define LOG_IF_BOOL_WARN(__FLAG__, __MSG__)  __LOG_IF_BOOL(warn, __FLAG__, __MSG__)
#define LOG_IF_BOOL_ERROR(__FLAG__, __MSG__) __LOG_IF_BOOL(error, __FLAG__, __MSG__)
#define LOG_IF_BOOL_FATAL(__FLAG__, __MSG__) __LOG_IF_BOOL(fatal, __FLAG__, __MSG__)

#define __THROW_IF_BOOL(__LVL__, __FLAG__, __MSG__) \
	if (!(__FLAG__)) { \
		HRESULT hr = ::winrt::impl::hresult_from_win32(GetLastError()); \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		::winrt::throw_hresult(hr); \
	} \
	static_cast<void>(0)
#define THROW_IF_BOOL_INFO(__FLAG__, __MSG__)  __THROW_IF_BOOL(info, __FLAG__, __MSG__)
#define THROW_IF_BOOL_WARN(__FLAG__, __MSG__)  __THROW_IF_BOOL(warn, __FLAG__, __MSG__)
#define THROW_IF_BOOL_ERROR(__FLAG__, __MSG__) __THROW_IF_BOOL(error, __FLAG__, __MSG__)
#define THROW_IF_BOOL_FATAL(__FLAG__, __MSG__) __THROW_IF_BOOL(fatal, __FLAG__, __MSG__)

#define __GOTO_IF_BOOL(__LVL__, __RET__, __MSG__) \
	if (!(__RET__)) { \
		HRESULT hr = ::winrt::impl::hresult_from_win32(GetLastError()); \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		goto __LABEL_FINALIZE; \
	} \
	static_cast<void>(0)
#define GOTO_IF_BOOL_INFO(__RET__, __MSG__)  __GOTO_IF_BOOL(info, __RET__, __MSG__)
#define GOTO_IF_BOOL_WARN(__RET__, __MSG__)  __GOTO_IF_BOOL(warn, __RET__, __MSG__)
#define GOTO_IF_BOOL_ERROR(__RET__, __MSG__) __GOTO_IF_BOOL(error, __RET__, __MSG__)
#define GOTO_IF_BOOL_FATAL(__RET__, __MSG__) __GOTO_IF_BOOL(fatal, __RET__, __MSG__)

#define __LOG_HRESULT(__LVL__, __HR__, __MSG__) \
	::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, __HR__, __LINE__, __FILE__, __FUNCTION__, __MSG__)
#define LOG_HRESULT_INFO(__HR__, __MSG__)  __LOG_HRESULT(info, __HR__, __MSG__)
#define LOG_HRESULT_WARN(__HR__, __MSG__)  __LOG_HRESULT(warn, __HR__, __MSG__)
#define LOG_HRESULT_ERROR(__HR__, __MSG__) __LOG_HRESULT(error, __HR__, __MSG__)
#define LOG_HRESULT_FATAL(__HR__, __MSG__) __LOG_HRESULT(fatal, __HR__, __MSG__)

#define __LOG_IF_HRESULT(__LVL__, __RET__, __MSG__) \
	{ \
		HRESULT hr = __RET__; \
		if (FAILED(hr)) { \
			::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		} \
	} \
	static_cast<void>(0)
#define LOG_IF_HRESULT_INFO(__RET__, __MSG__)  __LOG_IF_HRESULT(info, __RET__, __MSG__)
#define LOG_IF_HRESULT_WARN(__RET__, __MSG__)  __LOG_IF_HRESULT(warn, __RET__, __MSG__)
#define LOG_IF_HRESULT_ERROR(__RET__, __MSG__) __LOG_IF_HRESULT(error, __RET__, __MSG__)
#define LOG_IF_HRESULT_FATAL(__RET__, __MSG__) __LOG_IF_HRESULT(fatal, __RET__, __MSG__)

#define __GOTO_IF_HRESULT(__LVL__, __RET__, __MSG__) \
	{ \
		HRESULT hr = __RET__; \
		if (FAILED(hr)) { \
			::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
			goto __LABEL_FINALIZE; \
		} \
	} \
	static_cast<void>(0)
#define GOTO_IF_HRESULT_INFO(__RET__, __MSG__)  __GOTO_IF_HRESULT(info, __RET__, __MSG__)
#define GOTO_IF_HRESULT_WARN(__RET__, __MSG__)  __GOTO_IF_HRESULT(warn, __RET__, __MSG__)
#define GOTO_IF_HRESULT_ERROR(__RET__, __MSG__) __GOTO_IF_HRESULT(error, __RET__, __MSG__)
#define GOTO_IF_HRESULT_FATAL(__RET__, __MSG__) __GOTO_IF_HRESULT(fatal, __RET__, __MSG__)

#define __THROW_HRESULT(__LVL__, __HR__, __MSG__) \
	{ \
		HRESULT hr = __HR__; \
		::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
		::winrt::throw_hresult(hr); \
	} \
	static_cast<void>(0)
#define THROW_HRESULT_INFO(__HR__, __MSG__)  __THROW_HRESULT(info, __HR__, __MSG__)
#define THROW_HRESULT_WARN(__HR__, __MSG__)  __THROW_HRESULT(warn, __HR__, __MSG__)
#define THROW_HRESULT_ERROR(__HR__, __MSG__) __THROW_HRESULT(error, __HR__, __MSG__)
#define THROW_HRESULT_FATAL(__HR__, __MSG__) __THROW_HRESULT(fatal, __HR__, __MSG__)

#define __THROW_IF_HRESULT(__LVL__, __RET__, __MSG__) \
	{ \
		HRESULT hr = __RET__; \
		if (FAILED(hr)) { \
			::app::logger::gLogger->log(::app::logger::llv_##__LVL__, __tag, hr, __LINE__, __FILE__, __FUNCTION__, __MSG__); \
			::winrt::throw_hresult(hr); \
		} \
	} \
	static_cast<void>(0)
#define THROW_IF_HRESULT_INFO(__RET__, __MSG__)  __THROW_IF_HRESULT(info, __RET__, __MSG__)
#define THROW_IF_HRESULT_WARN(__RET__, __MSG__)  __THROW_IF_HRESULT(warn, __RET__, __MSG__)
#define THROW_IF_HRESULT_ERROR(__RET__, __MSG__) __THROW_IF_HRESULT(error, __RET__, __MSG__)
#define THROW_IF_HRESULT_FATAL(__RET__, __MSG__) __THROW_IF_HRESULT(fatal, __RET__, __MSG__)

}
