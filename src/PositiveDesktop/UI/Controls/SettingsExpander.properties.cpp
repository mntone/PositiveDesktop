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

	DEFINE_DP(CardContent, winrt::IInspectable, SettingsExpander);

	DEFINE_DP_CALLBACK(
		Description,
		winrt::IInspectable,
		SettingsExpander,
		&SettingsExpander::OnDescriptionChangedStatic);

	DEFINE_DP_CALLBACK(
		HeaderIcon,
		winrt::Microsoft::UI::Xaml::Controls::IconElement,
		SettingsExpander,
		&SettingsExpander::OnHeaderIconChangedStatic);

	DEFINE_DP_CALLBACK(
		Header,
		winrt::IInspectable,
		SettingsExpander,
		&SettingsExpander::OnHeaderChangedStatic);

	DEFINE_DP_CALLBACK(
		IsExpanded,
		bool,
		SettingsExpander,
		&SettingsExpander::OnIsExpandedChangedStatic);

	DEFINE_DP_METADATA(
		Orientation,
		winrt::Microsoft::UI::Xaml::Controls::Orientation,
		SettingsExpander,
		winrt::PropertyMetadata(
			winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
			winrt::PropertyChangedCallback(&SettingsExpander::OnOrientationChangedStatic)));

	initialized_ = true;
}
