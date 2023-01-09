#include "pch.h"
#include "SettingsExpander.h"
#if __has_include("./UI/Controls/SettingsExpander.g.cpp")
#include "./UI/Controls/SettingsExpander.g.cpp"
#endif

#define DELAY_INIT_DP // Delay Init
#include "UI/Helpers/DependencyPropertyHelper.h"

namespace projection { // Need to build
	using namespace ::winrt::PositiveDesktop::UI::Controls;
}

using namespace ::winrt::PositiveDesktop::UI::Controls::implementation;

SettingsExpander::DependencyProperties SettingsExpander::props_;

void SettingsExpander::DependencyProperties::DelayInitIfNeeded() {
	if (initialized_) return;

	DEFINE_PARENT_TYPENAME(SettingsExpander);

	DEFINE_DP(Description, winrt::IInspectable, SettingsExpander);

	DEFINE_DP(ExpanderContent, winrt::IInspectable, SettingsExpander);

	DEFINE_DP(HeaderIcon, winrt::Microsoft::UI::Xaml::Controls::IconElement, SettingsExpander);

	DEFINE_DP_METADATA(
		Orientation,
		winrt::Microsoft::UI::Xaml::Controls::Orientation,
		SettingsExpander,
		winrt::PropertyMetadata(
			winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal)));

	initialized_ = true;
}
