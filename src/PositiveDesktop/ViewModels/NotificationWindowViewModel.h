#pragma once
#include "ViewModels.NotificationWindowViewModel.g.h"

#include "UI/NotificationPresenter.h"

namespace winrt::PositiveDesktop::ViewModels::implementation {

	struct NotificationWindowViewModel: NotificationWindowViewModelT<NotificationWindowViewModel> {
		NotificationWindowViewModel() noexcept;
		NotificationWindowViewModel(UI::NotificationPresenterData data) noexcept;

		winrt::hstring Caption() const noexcept {
			return caption_;
		}

		winrt::hstring Message() const noexcept {
			return message_;
		}

	private:
		winrt::hstring caption_;
		winrt::hstring message_;
	};

}
