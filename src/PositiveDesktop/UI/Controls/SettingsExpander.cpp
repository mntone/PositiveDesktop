#include "pch.h"
#include "SettingsExpander.h"

#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_SettingsExpander { L"PositiveDesktop.UI.Controls.SettingsExpander" };
}

namespace controls {
	constexpr std::wstring_view Description { L"Description" };
	constexpr std::wstring_view HeaderIcon { L"HeaderIcon" };
	constexpr std::wstring_view Header { L"Header" };
}

namespace states {
	constexpr std::wstring_view Normal { L"Normal" };
	constexpr std::wstring_view PointerOver { L"PointerOver" };
	constexpr std::wstring_view Pressed { L"Pressed" };
	constexpr std::wstring_view Disabled { L"Disabled" };

	constexpr std::wstring_view HeaderOnly { L"HeaderOnly" };
	constexpr std::wstring_view HeaderAndDescription { L"HeaderAndDescription" };

	constexpr std::wstring_view Vertical { L"Vertical" };
	constexpr std::wstring_view Horizontal { L"Horizontal" };
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;

	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Input;

	using namespace ::winrt::PositiveDesktop::UI::Controls;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

SettingsExpander::SettingsExpander() noexcept {
	props_.DelayInitIfNeeded();
	DefaultStyleKey(box_value(resources::PositiveDesktop_UI_Controls_SettingsExpander));
}

void SettingsExpander::OnApplyTemplate() {
	__super::OnApplyTemplate();

	OnOrientationChanged(Orientation());
	OnDescriptionChanged(Description());
	OnHeaderIconChanged(HeaderIcon());
	//IsEnabledChanged(&SettingsExpander::OnIsEnabledChangedStatic); // The listener is the same lifecycle to the object.
}

void SettingsExpander::OnPointerEntered(PointerRoutedEventArgs const& args) const {
	__super::OnPointerEntered(args);

	VisualStateManager::GoToState(*this, states::PointerOver, true);
}

void SettingsExpander::OnPointerExited(PointerRoutedEventArgs const& args) const {
	__super::OnPointerExited(args);

	VisualStateManager::GoToState(*this, states::Normal, true);
}

void SettingsExpander::OnDescriptionChanged(IInspectable const& newValue) {
	FrameworkElement element { GetTemplateChild(controls::Description).try_as<FrameworkElement>() };
	if (element) {
		if (newValue == nullptr) {
			element.Visibility(Visibility::Collapsed);
			VisualStateManager::GoToState(*this, states::HeaderOnly, true);
		} else {
			std::optional<hstring> newString { newValue.try_as<hstring>() };
			if (newString.has_value() && newString.value().empty()) {
				element.Visibility(Visibility::Collapsed);
				VisualStateManager::GoToState(*this, states::HeaderOnly, true);
			} else {
				element.Visibility(Visibility::Visible);
				VisualStateManager::GoToState(*this, states::HeaderAndDescription, true);
			}
		}
	}
}

void SettingsExpander::OnHeaderIconChanged(IconElement const& newValue) {
	FrameworkElement element { GetTemplateChild(controls::HeaderIcon).try_as<FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsExpander::OnOrientationChanged(winrt::Orientation newValue) {
	VisualStateManager::GoToState(*this, newValue == Orientation::Vertical ? states::Vertical : states::Horizontal, true);
}

void SettingsExpander::OnDescriptionChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<winrt::SettingsExpander>())->OnDescriptionChanged(args.NewValue());
}

void SettingsExpander::OnHeaderIconChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<winrt::SettingsExpander>())->OnHeaderIconChanged(
		args.NewValue().as<Microsoft::UI::Xaml::Controls::IconElement>());
}

void SettingsExpander::OnOrientationChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<winrt::SettingsExpander>())->OnOrientationChanged(
		winrt::unbox_value<winrt::Orientation>(args.NewValue()));
}
