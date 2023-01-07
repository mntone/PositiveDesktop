#include "pch.h"
#include "NavigationHelper.h"
#if __has_include("./UI/Helpers/NavigationHelper.g.cpp")
#include "./UI/Helpers/NavigationHelper.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt {
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::System;
}

using namespace winrt::PositiveDesktop::UI::Helpers::implementation;

winrt::DependencyProperty NavigationHelper::PageTypeProperty_ {
	winrt::DependencyProperty::RegisterAttached(
		L"PageType",
		winrt::xaml_typename<winrt::Windows::UI::Xaml::Interop::TypeName>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Helpers::NavigationHelper>(),
		nullptr)
};
