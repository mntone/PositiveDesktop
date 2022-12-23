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