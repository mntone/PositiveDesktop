#pragma once
#include "ViewModels/NotificationWindowViewModel.g.h"

namespace winrt::PositiveDesktop::ViewModels::implementation {

	struct NotificationWindowViewModel: NotificationWindowViewModelT<NotificationWindowViewModel> {
		NotificationWindowViewModel() noexcept;
		NotificationWindowViewModel(winrt::hstring caption, winrt::hstring message) noexcept
			: caption_(std::move(caption))
			, message_(std::move(message))
		{ }

		inline winrt::hstring Caption() const noexcept {
			return caption_;
		}

		inline winrt::hstring Message() const noexcept {
			return message_;
		}

	private:
		winrt::hstring caption_;
		winrt::hstring message_;
	};

}
