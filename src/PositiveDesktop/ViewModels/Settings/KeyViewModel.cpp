#include "pch.h"
#include "KeyViewModel.h"
#if __has_include("ViewModels/Settings/KeyViewModel.g.cpp")
#include "ViewModels/Settings/KeyViewModel.g.cpp"
#endif

namespace properties {
	constexpr std::wstring_view Key { L"Key" };
	constexpr std::wstring_view Key2 { L"Key2" };
	constexpr std::wstring_view KeyModifiers { L"KeyModifiers" };
	constexpr std::wstring_view KeyModifiers2 { L"KeyModifiers2" };
}

namespace winrt {
	using namespace ::winrt::Windows::System;
}

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

void KeyViewModel::Key(VirtualKey value) noexcept {
	if (key_ != value) {
		key_ = value;
		RaisePropertyChanged(properties::Key);
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
	}
}

void KeyViewModel::KeyModifiers2(VirtualKeyModifiers value) noexcept {
	if (keyModifiers2_ != value) {
		keyModifiers2_ = value;
		RaisePropertyChanged(properties::KeyModifiers2);
	}
}
