#pragma once
#include <winrt/impl/Microsoft.UI.Xaml.0.h>
#include <winrt/impl/Microsoft.UI.Composition.SystemBackdrops.0.h>

constexpr winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme ConvertToSystemBackdropTheme(winrt::Microsoft::UI::Xaml::ElementTheme const& theme) {
	switch (theme) {
	case winrt::Microsoft::UI::Xaml::ElementTheme::Dark:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Dark;
	case winrt::Microsoft::UI::Xaml::ElementTheme::Light:
	default:
		return winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme::Light;
	}
}
