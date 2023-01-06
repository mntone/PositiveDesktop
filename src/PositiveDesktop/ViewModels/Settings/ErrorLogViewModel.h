#pragma once
#include "ViewModels/Settings/ErrorLogViewModel.g.h"
#include "ViewModels/Settings/ErrorLogsViewModel.g.h"

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct ErrorLogViewModel: ErrorLogViewModelT<ErrorLogViewModel> {
		ErrorLogViewModel(app::logger::log_t const& log) noexcept;

	public:  // - Properties
		inline winrt::hstring Level() const noexcept {
			return level_;
		}

		inline winrt::hstring Tag() const noexcept {
			return tag_;
		}

		inline winrt::hstring Operation() const noexcept {
			return operation_;
		}

		constexpr short Line() const noexcept {
			return line_;
		}

		constexpr winrt::Windows::Foundation::DateTime DateTime() const noexcept {
			return datetime_;
		}

		inline winrt::hstring FileName() const noexcept {
			return filename_;
		}

		inline winrt::hstring FuncName() const noexcept {
			return funcname_;
		}

		inline winrt::hstring Message() const noexcept {
			return message_;
		}

	private:
		winrt::hstring level_;
		winrt::hstring tag_;
		winrt::hstring operation_;
		short line_;
		winrt::Windows::Foundation::DateTime datetime_;
		winrt::hstring filename_;
		winrt::hstring funcname_;
		winrt::hstring message_;
	};

	struct ErrorLogsViewModel: ErrorLogsViewModelT<ErrorLogsViewModel>, reps::observer_t<app::logger::log_t> {
		ErrorLogsViewModel() noexcept;
		~ErrorLogsViewModel() noexcept;

	private:
		void FASTCALL on(reps::bag_t<app::logger::log_t> const& value) noexcept override;

	public:  // - Properties
		inline winrt::Windows::Foundation::Collections::IObservableVector<winrt::PositiveDesktop::ViewModels::Settings::ErrorLogViewModel> Logs() const noexcept {
			return logs_;
		}

	private:
		winrt::Windows::Foundation::Collections::IObservableVector<winrt::PositiveDesktop::ViewModels::Settings::ErrorLogViewModel> logs_;
	};

}
