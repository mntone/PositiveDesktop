#include "pch.h"
#include "NotificationWindowViewModel.h"
#if __has_include("ViewModels/NotificationWindowViewModel.g.cpp")
#include "ViewModels/NotificationWindowViewModel.g.cpp"
#endif

using namespace winrt::PositiveDesktop::ViewModels::implementation;

NotificationWindowViewModel::NotificationWindowViewModel() noexcept
	: caption_(L"")
	, message_(L"") {
}
