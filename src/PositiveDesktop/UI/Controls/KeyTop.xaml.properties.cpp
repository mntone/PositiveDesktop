#include "pch.h"
#include "KeyTop.xaml.h"
#if __has_include("./UI/Controls/KeyTop.g.cpp")
#include "./UI/Controls/KeyTop.g.cpp"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;

	using namespace ::winrt::Microsoft::UI::Xaml;

	using namespace winrt::PositiveDesktop::UI::Controls;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

KeyTop::DependencyProperties KeyTop::props_;

void KeyTop::DependencyProperties::DelayInitIfNeeded() {
	if (initialized_) return;

	HasError = DependencyProperty::Register(L"HasError", xaml_typename<bool>(), xaml_typename<winrt::KeyTop>(), nullptr);

	Key = DependencyProperty::Register(
		L"Key",
		xaml_typename<VirtualKey>(),
		xaml_typename<winrt::KeyTop>(),
		PropertyMetadata(box_value(VirtualKey::None), PropertyChangedCallback(&KeyTop::OnKeyTopChangedStatic)));

	initialized_ = true;
}

void KeyTop::Key(VirtualKey value) const {
	SetValue(props_.Key, box_value(value));
}

void KeyTop::HasError(bool value) const {
	SetValue(props_.HasError, box_value(value));
}
