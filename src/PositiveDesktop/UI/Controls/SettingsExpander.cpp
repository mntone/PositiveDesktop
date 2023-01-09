#include "pch.h"
#include "SettingsExpander.h"

#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_SettingsExpander { L"PositiveDesktop.UI.Controls.SettingsExpander" };
}

namespace controls {
	constexpr std::wstring_view ActionIcon { L"ActionIcon" };
	constexpr std::wstring_view CardContentControl { L"CardContentControl" };
	constexpr std::wstring_view Description { L"Description" };
	constexpr std::wstring_view HeaderIcon { L"HeaderIcon" };
	constexpr std::wstring_view Header { L"Header" };
}

namespace states {
	constexpr std::wstring_view Normal { L"Normal" };
	constexpr std::wstring_view PointerOver { L"PointerOver" };
	constexpr std::wstring_view Pressed { L"Pressed" };
	constexpr std::wstring_view Disabled { L"Disabled" };

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
	cardContentControl_ = GetTemplateChild(controls::CardContentControl).as<ContentControl>();
	OnOrientationChanged(Orientation());
	OnDescriptionChanged(Description());
	OnHeaderChanged(Header());
	OnHeaderIconChanged(HeaderIcon());
	VisualStateManager::GoToState(*this, IsEnabled() ? states::Normal : states::Disabled, true);
	RegisterButtonEvents();
	IsEnabledChanged(&SettingsExpander::OnIsEnabledChangedStatic); // The listener is the same lifecycle to the object.
}

void SettingsExpander::RegisterButtonEvents() {
	// The listener is the same lifecycle to the object.
	ContentControl cardContentControl { cardContentControl_ };
	cardContentControl.PointerEntered(&SettingsExpander::OnControlPointerEnteredStatic);
	cardContentControl.PointerPressed(&SettingsExpander::OnControlPointerPressedStatic);
	cardContentControl.PointerReleased(&SettingsExpander::OnControlPointerReleasedStatic);
	cardContentControl.PointerExited(&SettingsExpander::OnControlPointerExitedStatic);
}

void SettingsExpander::OnControlPointerEnteredStatic(IInspectable const& sender, PointerRoutedEventArgs const& /*args*/) {
	VisualStateManager::GoToState(sender.as<Control>(), states::PointerOver, true);
}

void SettingsExpander::OnControlPointerPressedStatic(IInspectable const& sender, PointerRoutedEventArgs const& /*args*/) {
	VisualStateManager::GoToState(sender.as<Control>(), states::PointerOver, true);
}

void SettingsExpander::OnControlPointerReleasedStatic(IInspectable const& sender, PointerRoutedEventArgs const& args) {
	Control proj { sender.as<Control>() };
	winrt::Windows::Foundation::Numerics::float2 point { args.GetCurrentPoint(proj).Position() };
	if (point.x < 0 || point.y < 0 || point.x > proj.ActualWidth() || point.y > proj.ActualHeight()) {
		VisualStateManager::GoToState(proj, states::Normal, true);
	} else {
		VisualStateManager::GoToState(proj, states::PointerOver, true);
	}
}

void SettingsExpander::OnControlPointerExitedStatic(IInspectable const& sender, PointerRoutedEventArgs const& /*args*/) {
	VisualStateManager::GoToState(sender.as<Control>(), states::Normal, true);
}

void SettingsExpander::OnIsEnabledChangedStatic(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args) {
	VisualStateManager::GoToState(sender.as<Control>(), unbox_value<bool>(args.NewValue()) ? states::Normal : states::Disabled, true);
}

void SettingsExpander::OnDescriptionChanged(IInspectable const& newValue) {
	FrameworkElement element { GetTemplateChild(controls::Description).try_as<FrameworkElement>() };
	if (element) {
		if (newValue == nullptr) {
			element.Visibility(Visibility::Collapsed);
		} else {
			std::optional<hstring> newString { newValue.try_as<hstring>() };
			element.Visibility(newString.has_value() && newString.value().empty() ? Visibility::Collapsed : Visibility::Visible);
		}
	}
}

void SettingsExpander::OnHeaderIconChanged(IconElement const& newValue) {
	FrameworkElement element { GetTemplateChild(controls::HeaderIcon).try_as<FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsExpander::OnHeaderChanged(IInspectable const& newValue) {
	FrameworkElement element { GetTemplateChild(controls::Header).try_as<FrameworkElement>() };
	if (element) {
		if (newValue == nullptr) {
			element.Visibility(Visibility::Collapsed);
		} else {
			std::optional<hstring> newString { newValue.try_as<hstring>() };
			element.Visibility(newString.has_value() && newString.value().empty() ? Visibility::Collapsed : Visibility::Visible);
		}
	}
}

void SettingsExpander::OnIsExpandedChanged() {
}

void SettingsExpander::OnOrientationChanged(winrt::Orientation newValue) {
	VisualStateManager::GoToState(*this, newValue == Orientation::Vertical ? states::Vertical : states::Horizontal, true);
}

void SettingsExpander::OnDescriptionChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<PositiveDesktop::UI::Controls::SettingsExpander>())->OnDescriptionChanged(args.NewValue());
}

void SettingsExpander::OnHeaderIconChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<PositiveDesktop::UI::Controls::SettingsExpander>())->OnHeaderIconChanged(
		args.NewValue().as<Microsoft::UI::Xaml::Controls::IconElement>());
}

void SettingsExpander::OnHeaderChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<PositiveDesktop::UI::Controls::SettingsExpander>())->OnHeaderChanged(args.NewValue());
}

void SettingsExpander::OnIsExpandedChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsExpander>(sender.as<PositiveDesktop::UI::Controls::SettingsExpander>())->OnIsExpandedChanged();
}

void SettingsExpander::OnOrientationChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	winrt::Orientation newOrientation { unbox_value<winrt::Orientation>(args.NewValue()) };
	VisualStateManager::GoToState(sender.as<Control>(), newOrientation == Orientation::Vertical ? states::Vertical : states::Horizontal, true);
}
