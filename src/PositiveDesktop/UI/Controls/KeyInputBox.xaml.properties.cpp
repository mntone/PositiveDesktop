#include "pch.h"
#include "KeyInputBox.xaml.h"
#if __has_include("./UI/Controls/KeyInputBox.g.cpp")
#include "./UI/Controls/KeyInputBox.g.cpp"
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

KeyInputBox::DependencyProperties KeyInputBox::props_;

void KeyInputBox::DependencyProperties::DelayInitIfNeeded() {
	if (initialized_) return;

	//InputScope = DependencyProperty::Register(
	//	L"InputScope",
	//	xaml_typename<Input::IInputScope>(),
	//	xaml_typename<winrt::KeyInputBox>(),
	//	nullptr);

	Key = DependencyProperty::Register(
		L"Key",
		xaml_typename<VirtualKey>(),
		xaml_typename<winrt::KeyInputBox>(),
		PropertyMetadata(box_value(VirtualKey::None), PropertyChangedCallback(&KeyInputBox::OnKeyChangedStatic)));

	KeyModifiers = DependencyProperty::Register(
		L"KeyModifiers",
		xaml_typename<VirtualKeyModifiers>(),
		xaml_typename<winrt::KeyInputBox>(),
		PropertyMetadata(box_value(VirtualKeyModifiers::None), PropertyChangedCallback(&KeyInputBox::OnKeyModifiersChangedStatic)));

	initialized_ = true;
}

void KeyInputBox::Key(VirtualKey value) const {
	SetValue(props_.Key, box_value(value));
}

void KeyInputBox::KeyModifiers(VirtualKeyModifiers value) const {
	SetValue(props_.KeyModifiers, box_value(value));
}
