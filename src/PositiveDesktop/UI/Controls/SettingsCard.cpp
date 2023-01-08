#include "pch.h"
#include "SettingsCard.h"
#if __has_include("./UI/Controls/SettingsCard.g.cpp")
#include "./UI/Controls/SettingsCard.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_SettingsCard { L"PositiveDesktop.UI.Controls.SettingsCard" };
}

namespace controls {
	constexpr std::wstring_view ActionIcon { L"ActionIcon" };
	constexpr std::wstring_view Description { L"Description" };
	constexpr std::wstring_view HeaderIcon { L"HeaderIcon" };
	constexpr std::wstring_view HeaderPresenter { L"HeaderPresenter" };
}

namespace states {
	constexpr std::wstring_view NormalState { L"Normal" };
	constexpr std::wstring_view PointerOverState { L"PointerOver" };
	constexpr std::wstring_view PressedState { L"Pressed" };
	constexpr std::wstring_view DisabledState { L"Disabled" };

	constexpr std::wstring_view TextAndIconState { L"TextAndIconState" };
	constexpr std::wstring_view TextOnlyState { L"TextOnlyState" };

	constexpr std::wstring_view Vertical { L"Vertical" };
	constexpr std::wstring_view Horizontal { L"Horizontal" };
}

namespace winrt {
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Controls;
	using namespace winrt::Microsoft::UI::Xaml::Input;
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::System;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

SettingsCard::SettingsCard() noexcept {
	DefaultStyleKey(box_value(resources::PositiveDesktop_UI_Controls_SettingsCard));
}

void SettingsCard::OnApplyTemplate() {
	OnOrientationChanged(Orientation());
	OnDescriptionChanged(Description());
	OnHeaderChanged(Header());
	OnHeaderIconChanged(HeaderIcon());
	OnIsClickEnabledChanged();
	VisualStateManager::GoToState(*this, IsEnabled() ? states::NormalState : states::DisabledState, true);
	isEnabledChangedRevoker_ = IsEnabledChanged(auto_revoke, { this, &SettingsCard::OnIsEnabledChanged });
}

void SettingsCard::OnIsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e) {
	VisualStateManager::GoToState(*this, IsEnabled() ? states::NormalState : states::DisabledState, true);
}

void SettingsCard::RegisterButtonEvents() {
	UnregisterButtonEvents();

	pointerEnteredRevoker_ = PointerEntered(auto_revoke, { this, &SettingsCard::OnControlPointerEntered });
	pointerExitedRevoker_ = PointerExited(auto_revoke, { this, &SettingsCard::OnControlPointerExited });
	previewKeyDownRevoker_ = PreviewKeyDown(auto_revoke, { this, &SettingsCard::OnControlPreviewKeyDown });
	previewKeyUpRevoker_ = PreviewKeyUp(auto_revoke, { this, &SettingsCard::OnControlPreviewKeyUp });
}

void SettingsCard::UnregisterButtonEvents() noexcept {
	previewKeyUpRevoker_.revoke();
	previewKeyDownRevoker_.revoke();
	pointerExitedRevoker_.revoke();
	pointerEnteredRevoker_.revoke();
}

void SettingsCard::OnControlPointerEntered(IInspectable const& sender, PointerRoutedEventArgs const& args) {
	VisualStateManager::GoToState(*this, states::PointerOverState, true);
}

void SettingsCard::OnControlPointerExited(IInspectable const& sender, PointerRoutedEventArgs const& args) {
	VisualStateManager::GoToState(*this, states::NormalState, true);
}

void SettingsCard::OnControlPreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& args) const {
	switch (args.Key()) {
	case VirtualKey::Enter:
	case VirtualKey::Space:
	case VirtualKey::GamepadA:
		VisualStateManager::GoToState(*this, states::PressedState, true);
		break;
	}
}

void SettingsCard::OnControlPreviewKeyUp(IInspectable const& sender, KeyRoutedEventArgs const& args) const {
	switch (args.Key()) {
	case VirtualKey::Enter:
	case VirtualKey::Space:
	case VirtualKey::GamepadA:
		VisualStateManager::GoToState(*this, states::NormalState, true);
		break;
	}
}

void SettingsCard::OnPointerPressed(Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerPressed(args);
		VisualStateManager::GoToState(*this, states::PressedState, true);
	}
}

void SettingsCard::OnPointerReleased(Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerReleased(args);
		VisualStateManager::GoToState(*this, states::NormalState, true);
	}
}

void SettingsCard::OnButtonIconChanged(bool isClickEnabled) {
	FrameworkElement element { GetTemplateChild(controls::ActionIcon).try_as<FrameworkElement>() };
	if (element) {
		element.Visibility(isClickEnabled ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsCard::OnDescriptionChanged(IInspectable const& newValue) {
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

void SettingsCard::OnHeaderIconChanged(IconElement const& newValue) {
	//VisualStateManager::GoToState(*this, newValue != nullptr ? states::TextAndIconState : states::TextOnlyState, true);
	FrameworkElement element { GetTemplateChild(controls::HeaderIcon).try_as<FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsCard::OnHeaderChanged(IInspectable const& newValue) {
	FrameworkElement element { GetTemplateChild(controls::HeaderPresenter).try_as<FrameworkElement>() };
	if (element) {
		if (newValue == nullptr) {
			element.Visibility(Visibility::Collapsed);
		} else {
			std::optional<hstring> newString { newValue.try_as<hstring>() };
			element.Visibility(newString.has_value() && newString.value().empty() ? Visibility::Collapsed : Visibility::Visible);
		}
	}
}

void SettingsCard::OnIsClickEnabledChanged() {
	if (IsClickEnabled()) {
		OnButtonIconChanged(true);
		RegisterButtonEvents();
	} else {
		OnButtonIconChanged(false);
		UnregisterButtonEvents();
	}
}

void SettingsCard::OnOrientationChanged(winrt::Orientation newValue) {
	VisualStateManager::GoToState(*this, newValue == Orientation::Vertical ? states::Vertical : states::Horizontal, true);
}

void SettingsCard::OnDescriptionChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsCard>(sender.as<PositiveDesktop::UI::Controls::SettingsCard>())->OnDescriptionChanged(args.NewValue());
}

void SettingsCard::OnHeaderIconChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsCard>(sender.as<PositiveDesktop::UI::Controls::SettingsCard>())->OnHeaderIconChanged(
		args.NewValue().as<Microsoft::UI::Xaml::Controls::IconElement>());
}

void SettingsCard::OnHeaderChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsCard>(sender.as<PositiveDesktop::UI::Controls::SettingsCard>())->OnHeaderChanged(args.NewValue());
}

void SettingsCard::OnIsClickEnabledChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsCard>(sender.as<PositiveDesktop::UI::Controls::SettingsCard>())->OnIsClickEnabledChanged();
}

void SettingsCard::OnOrientationChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<SettingsCard>(sender.as<PositiveDesktop::UI::Controls::SettingsCard>())->OnOrientationChanged(
		winrt::unbox_value<winrt::Orientation>(args.NewValue()));
}

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::ActionIconProperty_ {
	winrt::DependencyProperty::Register(
		L"ActionIcon",
		winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconElement>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata { winrt::box_value(winrt::hstring(L"\ue974")) })
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::ActionIconTooltipProperty_ {
	winrt::DependencyProperty::Register(
		L"ActionIconTooltip",
		winrt::xaml_typename<winrt::hstring>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata { winrt::box_value(winrt::hstring(L"More")) })
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::DescriptionProperty_ {
	winrt::DependencyProperty::Register(
		L"Description",
		winrt::xaml_typename<winrt::IInspectable>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata(winrt::IInspectable { nullptr }, winrt::PropertyChangedCallback(&SettingsCard::OnDescriptionChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::HeaderIconProperty_ {
	winrt::DependencyProperty::Register(
		L"HeaderIcon",
		winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconElement>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata(winrt::Microsoft::UI::Xaml::Controls::IconElement { nullptr }, winrt::PropertyChangedCallback(&SettingsCard::OnHeaderIconChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::HeaderProperty_ {
	winrt::DependencyProperty::Register(
		L"Header",
		winrt::xaml_typename<winrt::IInspectable>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata(winrt::IInspectable { nullptr }, winrt::PropertyChangedCallback(&SettingsCard::OnHeaderChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::IsClickEnabledProperty_ {
	winrt::DependencyProperty::Register(
		L"IsClickEnabled",
		winrt::xaml_typename<bool>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata(winrt::box_value(false), winrt::PropertyChangedCallback(&SettingsCard::OnIsClickEnabledChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::OrientationProperty_ {
	winrt::DependencyProperty::Register(
		L"Orientation",
		winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata { winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal), winrt::PropertyChangedCallback(&SettingsCard::OnOrientationChangedStatic) })
};
