#include "pch.h"
#include "KeyInputBox.xaml.h"

#include <winrt/Windows.Foundation.h> // Fix buggy IntelliSense :(
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_KeyInputBox { L"PositiveDesktop.UI.Controls.KeyInputBox" };
	constexpr std::wstring_view PositiveDesktop_UI_Controls_KeyInputBox_Uri { L"ms-appx:///UI/Controls/KeyInputBox.xaml" };
}

namespace controls {
	constexpr std::wstring_view LeftControl { L"LeftControl" };
	constexpr std::wstring_view LeftShift { L"LeftShift" };
	constexpr std::wstring_view LeftMenu { L"LeftMenu" };
	constexpr std::wstring_view LeftWindows { L"LeftWindows" };

	constexpr std::wstring_view Key { L"Key" };
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;
	using namespace ::winrt::Windows::UI::Core;

	using namespace ::winrt::Microsoft::UI::Input;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Input;

	using namespace ::winrt::PositiveDesktop::UI::Controls;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;


KeyInputBox::KeyInputBox() {
	props_.DelayInitIfNeeded();
	DefaultStyleKey(box_value(resources::PositiveDesktop_UI_Controls_KeyInputBox));
	DefaultStyleResourceUri(Uri(resources::PositiveDesktop_UI_Controls_KeyInputBox_Uri));
	//SetValue(props_.InputScope, InputScope());
}

void KeyInputBox::OnApplyTemplate() {
	__super::OnApplyTemplate();

	OnKeyChanged(Key());
	SetAllKeyModifiersVisibilities(VirtualKeyModifiers::None, KeyModifiers());
}

void KeyInputBox::OnKeyDown(KeyRoutedEventArgs const& args) {
	CorePhysicalKeyStatus keyStatus { args.KeyStatus() };
	VirtualKey key { args.OriginalKey() };
	if (keyStatus.RepeatCount == 1) {
		bool ctrlKey { (static_cast<uint32_t>(InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Control)) & 0x1 /* Down */) != 0 };
		bool shiftKey { (static_cast<uint32_t>(InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Shift)) & 0x1 /* Down */) != 0 };
		bool menuKey { (static_cast<uint32_t>(InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Menu)) & 0x1 /* Down */) != 0 };
		bool lwinKey { (static_cast<uint32_t>(InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::LeftWindows)) & 0x1 /* Down */) != 0 };
		bool rwinKey { (static_cast<uint32_t>(InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::RightWindows)) & 0x1 /* Down */) != 0 };

		uint32_t modifiers { 0 };
		if (ctrlKey) modifiers |= 0x1 /* Control */;
		if (shiftKey) modifiers |= 0x4 /* Shift */;
		if (menuKey) modifiers |= 0x2 /* Menu */;
		if (lwinKey || rwinKey) modifiers |= 0x8 /* Windows */;
		KeyModifiers(static_cast<VirtualKeyModifiers>(modifiers));

		switch (key) {
		case VirtualKey::Control:
		case VirtualKey::Menu:
		case VirtualKey::Shift:
			break;
		default:
			Key(key);
			break;
		}
	}

	args.Handled(true);
}

void KeyInputBox::OnKeyUp(KeyRoutedEventArgs const& args) {
	args.Handled(true);
}

void KeyInputBox::OnKeyChanged(VirtualKey newValue) const {
	if (Control key { GetTemplateChild(controls::Key).try_as<Control>() }) {
		key.Visibility(VirtualKey::None == newValue ? Visibility::Collapsed : Visibility::Visible);
	}
}

void KeyInputBox::OnKeyChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<KeyInputBox>(sender.as<winrt::KeyInputBox>())->OnKeyChanged(args.NewValue().as<VirtualKey>());
}

void KeyInputBox::SetKeyModifiersVisibility(param::hstring const& controlName, uint32_t oldValue, uint32_t newValue, uint32_t mask) const {
	Control control { GetTemplateChild(controlName).try_as<Control>() };
	if (control) {
		if (!(oldValue & mask) && (newValue & mask)) {
			control.Visibility(Visibility::Visible);
		} else if ((oldValue & mask) && !(newValue & mask)) {
			control.Visibility(Visibility::Collapsed);
		}
	}
}

void KeyInputBox::SetAllKeyModifiersVisibilities(VirtualKeyModifiers oldValue, VirtualKeyModifiers newValue) const {
	uint32_t oldValue2 { static_cast<uint32_t>(oldValue) };
	uint32_t newValue2 { static_cast<uint32_t>(newValue) };
	SetKeyModifiersVisibility(controls::LeftControl, oldValue2, newValue2, 0x1 /* Control */);
	SetKeyModifiersVisibility(controls::LeftShift, oldValue2, newValue2, 0x4 /* Shift */);
	SetKeyModifiersVisibility(controls::LeftMenu, oldValue2, newValue2, 0x2 /* Menu */);
}

void KeyInputBox::OnKeyModifiersChanged(std::optional<VirtualKeyModifiers> oldValue, VirtualKeyModifiers newValue) const {
	if (oldValue.has_value()) {
		SetAllKeyModifiersVisibilities(oldValue.value(), newValue);
	} else {
		uint32_t newValue2 { static_cast<uint32_t>(newValue) };

		if (Control leftControl { GetTemplateChild(controls::LeftControl).try_as<Control>() }) {
			leftControl.Visibility(newValue2 & 0x1 /* Control */ ? Visibility::Visible : Visibility::Collapsed);
		}

		if (Control leftShift { GetTemplateChild(controls::LeftShift).try_as<Control>() }) {
			leftShift.Visibility(newValue2 & 0x4 /* Shift */ ? Visibility::Visible : Visibility::Collapsed);
		}

		if (Control leftMenu { GetTemplateChild(controls::LeftMenu).try_as<Control>() }) {
			leftMenu.Visibility(newValue2 & 0x2 /* Menu */ ? Visibility::Visible : Visibility::Collapsed);
		}

		if (Control leftWindows { GetTemplateChild(controls::LeftWindows).try_as<Control>() }) {
			leftWindows.Visibility(newValue2 & 0x8 /* Windows */ ? Visibility::Visible : Visibility::Collapsed);
		}
	}
}

void KeyInputBox::OnKeyModifiersChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<KeyInputBox>(sender.as<winrt::KeyInputBox>())->OnKeyModifiersChanged(
		args.OldValue().try_as<VirtualKeyModifiers>(),
		args.NewValue().as<VirtualKeyModifiers>());
}
