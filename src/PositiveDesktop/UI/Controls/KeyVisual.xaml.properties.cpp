#include "pch.h"
#include "KeyVisual.xaml.h"
#if __has_include("./UI/Controls/KeyVisual.g.cpp")
#include "./UI/Controls/KeyVisual.g.cpp"
#endif

#include <winrt/Windows.Foundation.h> // Fix buggy IntelliSense :(
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;

	using namespace ::winrt::Microsoft::UI::Xaml;

	using namespace winrt::PositiveDesktop::UI::Controls;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

KeyVisual::DependencyProperties KeyVisual::props_;

void KeyVisual::DependencyProperties::DelayInitIfNeeded() {
	if (initialized_) return;

	HasError = DependencyProperty::Register(L"HasError", xaml_typename<bool>(), xaml_typename<winrt::KeyVisual>(), nullptr);

	Key = DependencyProperty::Register(
		L"Key",
		xaml_typename<VirtualKey>(),
		xaml_typename<winrt::KeyVisual>(),
		PropertyMetadata(box_value(VirtualKey::None), PropertyChangedCallback(&KeyVisual::OnKeyChangedStatic)));

	initialized_ = true;
}

void KeyVisual::Key(VirtualKey value) const {
	SetValue(props_.Key, box_value(value));
}

void KeyVisual::HasError(bool value) const {
	SetValue(props_.HasError, box_value(value));
}
