#include "pch.h"
#include "KeyViewModel.h"
#if __has_include("ViewModels/Settings/KeyViewModel.g.cpp")
#include "ViewModels/Settings/KeyViewModel.g.cpp"
#endif

namespace properties {
	constexpr std::wstring_view IsValid { L"IsValid" };
	constexpr std::wstring_view Key { L"Key" };
	constexpr std::wstring_view Key2 { L"Key2" };
	constexpr std::wstring_view KeyModifiers { L"KeyModifiers" };
	constexpr std::wstring_view KeyModifiers2 { L"KeyModifiers2" };
}

namespace winrt {
	using namespace ::winrt::Windows::System;
}

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

winrt::VirtualKeyModifiers to_vkmodifiers(app::storage::key_t const& key) {
	uint32_t modifiers(0);
	if (key.leftCtrl() || key.rightCtrl()) modifiers |= static_cast<uint32_t>(winrt::VirtualKeyModifiers::Control);
	if (key.leftAlt() || key.rightAlt()) modifiers |= static_cast<uint32_t>(winrt::VirtualKeyModifiers::Menu);
	if (key.leftWin() || key.rightWin()) modifiers |= static_cast<uint32_t>(winrt::VirtualKeyModifiers::Windows);
	return static_cast<winrt::VirtualKeyModifiers>(modifiers);
}

void KeyViewModel::Bind(app::storage::keymap_t const& keymap) noexcept {
	key_ = static_cast<VirtualKey>(keymap.key1.key());
	keyModifiers_ = to_vkmodifiers(keymap.key1);
	key2_ = static_cast<VirtualKey>(keymap.key2.key());
	keyModifiers2_ = to_vkmodifiers(keymap.key2);
}

void KeyViewModel::Key(VirtualKey value) noexcept {
	if (key_ != value) {
		key_ = value;
		RaisePropertyChanged(properties::Key);
		RaisePropertyChanged(properties::IsValid);
	}
}

void KeyViewModel::KeyModifiers(VirtualKeyModifiers value) noexcept {
	if (keyModifiers_ != value) {
		keyModifiers_ = value;
		RaisePropertyChanged(properties::KeyModifiers);
	}
}

void KeyViewModel::Key2(VirtualKey value) noexcept {
	if (key2_ != value) {
		key2_ = value;
		RaisePropertyChanged(properties::Key2);
		RaisePropertyChanged(properties::IsValid);
	}
}

void KeyViewModel::KeyModifiers2(VirtualKeyModifiers value) noexcept {
	if (keyModifiers2_ != value) {
		keyModifiers2_ = value;
		RaisePropertyChanged(properties::KeyModifiers2);
	}
}
