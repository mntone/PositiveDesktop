#include "pch.h"
#include "ErrorLogViewModel.h"
#if __has_include("ViewModels/Settings/ErrorLogViewModel.g.cpp")
#include "ViewModels/Settings/ErrorLogViewModel.g.cpp"
#endif
#if __has_include("ViewModels/Settings/ErrorLogsViewModel.g.cpp")
#include "ViewModels/Settings/ErrorLogsViewModel.g.cpp"
#endif

namespace app {
	inline winrt::hstring to_glyph(app::logger::loglvl_t level) noexcept {
		using namespace app::logger;

		switch (level) {
		case llv_trace:
			return L"";
		case llv_debug:
			return L"";
		case llv_info:
			return L"\uE946";
		case llv_warn:
			return L"\uE7BA";
		case llv_error:
			return L"\uEA39";
		case llv_fatal:
			return L"\uEB90";
		case llv_unknown:
		default:
			return L"\uE9CE";
		}
	}
}

namespace winrt {

	inline hstring to_hstring(app::logger::loglvl_t level) noexcept {
		using namespace app::logger;

		switch (level) {
		case llv_trace:
			return L"Trace";
		case llv_debug:
			return L"Debug";
		case llv_info:
			return L"Info";
		case llv_warn:
			return L"Warn";
		case llv_error:
			return L"Error";
		case llv_fatal:
			return L"Fatal";
		case llv_unknown:
		default:
			return L"Unknown";
		}
	}

	inline hstring to_hstring(app::logger::logtag_t tag) noexcept {
		using namespace app::logger;

		switch (tag) {
		case ltg_desktop:
			return L"Desktop";
		case ltg_desktop_listener:
			return L"DesktopListener";
		case ltg_keylistener:
			return L"KeyListener";
		case ltg_logger:
			return L"Logger";
		case ltg_storage:
			return L"Storage";
		case ltg_presenter:
			return L"Presenter";
		case ltg_unknown:
		default:
			return L"Unknown";
		}
	}

	inline hstring to_hstring(app::logger::logop_t op) noexcept {
		using namespace app::logger;

		switch (op) {
		case lop_function_begin:
			return L"Function - Begin";
		case lop_function_end:
			return L"Function - End";
		case lop_unknown:
		default:
			return L"Unknown";
		}
	}

}

#include <winrt/Windows.Foundation.Collections.h>

#include "Common/Reps.BufferBounce.h"
#include "UI/UIHelper.h"

using namespace app::logger;

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

ErrorLogViewModel::ErrorLogViewModel(log_t const& log) noexcept
	: levelIcon_(app::to_glyph(log.level))
	, level_(winrt::to_hstring(log.level))
	, tag_(winrt::to_hstring(log.tag))
	, operation_(winrt::to_hstring(log.op))
	, line_(log.line)
	, datetime_(winrt::clock::from_sys(log.datetime))
	, filename_(winrt::to_hstring(log.filename))
	, funcname_(winrt::to_hstring(log.funcname))
	, message_(winrt::to_hstring(log.message))
	, Header_(winrt::to_hstring(log.message.empty() ? std::format("{} in line {}", log.funcname, log.line) : log.message))
	, Description_(winrt::to_hstring(!log.message.empty() ? std::format("{} in line {}", log.funcname, log.line) : "")) {
}

ErrorLogsViewModel::ErrorLogsViewModel() noexcept
	: logs_(winrt::single_threaded_observable_vector<winrt::PositiveDesktop::ViewModels::Settings::ErrorLogViewModel>()) {
	for (log_t const* ptr = gLogger->first(); ptr != nullptr; gLogger->next(&ptr)) {
		logs_.InsertAt(0, winrt::make<ErrorLogViewModel>(*ptr));
	}
	gLogger->addObserver(reps::make_buffer_debounce(observer(), std::chrono::milliseconds { 350 }));
}

ErrorLogsViewModel::~ErrorLogsViewModel() noexcept {
	gLogger->clearObserver();
}

void FASTCALL ErrorLogsViewModel::on(reps::bag_t<std::vector<app::logger::log_t>> const& value) noexcept {
	app::ui::ensureUIThread();
	for (app::logger::log_t const& item : value.data) {
		logs_.InsertAt(0, winrt::make<ErrorLogViewModel>(item));
	}
}
