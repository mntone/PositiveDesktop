#include "pch.h"
#include "SettingsCard.h"
#if __has_include("./UI/Controls/SettingsCard.g.cpp")
#include "./UI/Controls/SettingsCard.g.cpp"
#endif

#define DELAY_INIT_DP // Delay Init
#include "UI/Helpers/DependencyPropertyHelper.h"

namespace projection { // Need to build
	using namespace ::winrt::PositiveDesktop::UI::Controls;
}

using namespace ::winrt::PositiveDesktop::UI::Controls::implementation;

SettingsCard::DependencyProperties SettingsCard::props_;

void SettingsCard::DependencyProperties::DelayInitIfNeeded() {
	if (initialized_) return;

	DEFINE_PARENT_TYPENAME(SettingsCard);

	DEFINE_DP_METADATA(
		ActionIcon,
		winrt::Microsoft::UI::Xaml::Controls::IconElement,
		SettingsCard,
		winrt::PropertyMetadata(winrt::box_value(winrt::hstring(L"\ue974"))));

	DEFINE_DP_METADATA(
		ActionIconTooltip,
		winrt::hstring,
		SettingsCard,
		winrt::PropertyMetadata(winrt::box_value(winrt::hstring(L"More"))));

	DEFINE_DP_CALLBACK(
		Description,
		winrt::IInspectable,
		SettingsCard,
		&SettingsCard::OnDescriptionChangedStatic);

	DEFINE_DP_CALLBACK(
		HeaderIcon,
		winrt::Microsoft::UI::Xaml::Controls::IconElement,
		SettingsCard,
		&SettingsCard::OnHeaderIconChangedStatic);

	DEFINE_DP_CALLBACK(
		Header,
		winrt::IInspectable,
		SettingsCard,
		&SettingsCard::OnHeaderChangedStatic);

	DEFINE_DP_CALLBACK(
		IsClickEnabled,
		bool,
		SettingsCard,
		&SettingsCard::OnIsClickEnabledChangedStatic);

	DEFINE_DP_METADATA(
		Orientation,
		winrt::Microsoft::UI::Xaml::Controls::Orientation,
		SettingsCard,
		winrt::PropertyMetadata(
			winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
			winrt::PropertyChangedCallback(&SettingsCard::OnOrientationChangedStatic)));

	initialized_ = true;
}
