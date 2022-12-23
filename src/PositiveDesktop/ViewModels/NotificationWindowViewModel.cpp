#include "pch.h"
#include "NotificationWindowViewModel.h"
#if __has_include("NotificationWindowViewModel.g.cpp")
#include "NotificationWindowViewModel.g.cpp"
#endif

using namespace winrt;

using namespace Microsoft::UI::Xaml::Data;

using namespace PositiveDesktop::ViewModels::implementation;

NotificationWindowViewModel::NotificationWindowViewModel() noexcept
	: caption_(L"")
	, message_(L"") {
}

inline hstring GetCaptionFromType(app::UI::NotificationPresenterType type) noexcept {
	switch (type) {
	case app::UI::NotificationPresenterType::Changed:
		return L"Virtual Desktop Changed";
	case app::UI::NotificationPresenterType::Renamed:
		return L"Virtual Desktop Renamed";
	default:
		return L"Unknown event";
	}
}

NotificationWindowViewModel::NotificationWindowViewModel(app::UI::NotificationPresenterData data) noexcept
	: caption_(GetCaptionFromType(data.type))
	, message_(data.message) {
}
