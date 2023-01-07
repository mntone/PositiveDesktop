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
	constexpr std::wstring_view DescriptionPresenter { L"DescriptionPresenter" };
	constexpr std::wstring_view HeaderIconPresenter { L"HeaderIconPresenter" };
	constexpr std::wstring_view HeaderPresenter { L"HeaderPresenter" };
}

namespace states {
	constexpr std::wstring_view NormalState { L"Normal" };
	constexpr std::wstring_view PointerOverState { L"PointerOver" };
	constexpr std::wstring_view PressedState { L"Pressed" };
	constexpr std::wstring_view DisabledState { L"Disabled" };

	constexpr std::wstring_view TextAndIconState { L"TextAndIconState" };
	constexpr std::wstring_view TextOnlyState { L"TextOnlyState" };
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
	DefaultStyleKey(winrt::box_value(resources::PositiveDesktop_UI_Controls_SettingsCard));
}

void SettingsCard::OnApplyTemplate() {
	OnDescriptionChanged(Description());
	OnHeaderChanged(Header());
	OnHeaderIconChanged(HeaderIcon());
	OnIsClickEnabledChanged();
	winrt::VisualStateManager::GoToState(*this, IsEnabled() ? states::NormalState : states::DisabledState, true);
	isEnabledChangedRevoker_ = IsEnabledChanged(winrt::auto_revoke, { this, &SettingsCard::OnIsEnabledChanged });
}

void SettingsCard::OnIsEnabledChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& e) {
	winrt::VisualStateManager::GoToState(*this, IsEnabled() ? states::NormalState : states::DisabledState, true);
}

void SettingsCard::RegisterButtonEvents() {
	UnregisterButtonEvents();

	pointerEnteredRevoker_ = PointerEntered(winrt::auto_revoke, { this, &SettingsCard::OnControlPointerEntered });
	pointerExitedRevoker_ = PointerExited(winrt::auto_revoke, { this, &SettingsCard::OnControlPointerExited });
	previewKeyDownRevoker_ = PreviewKeyDown(winrt::auto_revoke, { this, &SettingsCard::OnControlPreviewKeyDown });
	previewKeyUpRevoker_ = PreviewKeyUp(winrt::auto_revoke, { this, &SettingsCard::OnControlPreviewKeyUp });
}

void SettingsCard::UnregisterButtonEvents() noexcept {
	previewKeyUpRevoker_.revoke();
	previewKeyDownRevoker_.revoke();
	pointerExitedRevoker_.revoke();
	pointerEnteredRevoker_.revoke();
}

void SettingsCard::OnControlPointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& args) {
	winrt::VisualStateManager::GoToState(*this, states::PointerOverState, true);
}

void SettingsCard::OnControlPointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& args) {
	winrt::VisualStateManager::GoToState(*this, states::NormalState, true);
}

void SettingsCard::OnControlPreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& args) const {
	switch (args.Key()) {
	case winrt::VirtualKey::Enter:
	case winrt::VirtualKey::Space:
	case winrt::VirtualKey::GamepadA:
		winrt::VisualStateManager::GoToState(*this, states::PressedState, true);
		break;
	}
}

void SettingsCard::OnControlPreviewKeyUp(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& args) const {
	switch (args.Key()) {
	case winrt::VirtualKey::Enter:
	case winrt::VirtualKey::Space:
	case winrt::VirtualKey::GamepadA:
		winrt::VisualStateManager::GoToState(*this, states::NormalState, true);
		break;
	}
}

void SettingsCard::OnPointerPressed(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerPressed(args);
		winrt::VisualStateManager::GoToState(*this, states::PressedState, true);
	}
}

void SettingsCard::OnPointerReleased(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerReleased(args);
		winrt::VisualStateManager::GoToState(*this, states::NormalState, true);
	}
}


void SettingsCard::OnDescriptionChanged(winrt::IInspectable const& newValue) {
	winrt::FrameworkElement element { GetTemplateChild(controls::DescriptionPresenter).try_as<winrt::FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsCard::OnHeaderIconChanged(winrt::IconElement const& newValue) {
	//winrt::VisualStateManager::GoToState(*this, newValue != nullptr ? states::TextAndIconState : states::TextOnlyState, true);
	winrt::FrameworkElement element { GetTemplateChild(controls::HeaderIconPresenter).try_as<winrt::FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsCard::OnHeaderChanged(winrt::IInspectable const& newValue) {
	winrt::FrameworkElement element { GetTemplateChild(controls::HeaderPresenter).try_as<winrt::FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsCard::OnIsClickEnabledChanged() {
	if (IsClickEnabled()) {
		RegisterButtonEvents();
	} else {
		UnregisterButtonEvents();
	}
}

void SettingsCard::OnDescriptionChangedStatic(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args) {
	winrt::get_self<SettingsCard>(sender.as<winrt::PositiveDesktop::UI::Controls::SettingsCard>())->OnDescriptionChanged(args.NewValue());
}

void SettingsCard::OnHeaderIconChangedStatic(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args) {
	winrt::get_self<SettingsCard>(sender.as<winrt::PositiveDesktop::UI::Controls::SettingsCard>())->OnHeaderIconChanged(
		args.NewValue().as<winrt::Microsoft::UI::Xaml::Controls::IconElement>());
}

void SettingsCard::OnHeaderChangedStatic(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args) {
	winrt::get_self<SettingsCard>(sender.as<winrt::PositiveDesktop::UI::Controls::SettingsCard>())->OnHeaderChanged(args.NewValue());
}

void SettingsCard::OnIsClickEnabledChangedStatic(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args) {
	winrt::get_self<SettingsCard>(sender.as<winrt::PositiveDesktop::UI::Controls::SettingsCard>())->OnIsClickEnabledChanged();
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
		winrt::PropertyMetadata { nullptr, winrt::PropertyChangedCallback(&SettingsCard::OnDescriptionChangedStatic) })
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::HeaderIconProperty_ {
	winrt::DependencyProperty::Register(
		L"HeaderIcon",
		winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconElement>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata { nullptr, winrt::PropertyChangedCallback(&SettingsCard::OnHeaderIconChangedStatic) })
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::HeaderProperty_ {
	winrt::DependencyProperty::Register(
		L"Header",
		winrt::xaml_typename<winrt::IInspectable>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata { nullptr, winrt::PropertyChangedCallback(&SettingsCard::OnHeaderChangedStatic) })
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::IsClickEnabledProperty_ {
	winrt::DependencyProperty::Register(
		L"IsClickEnabled",
		winrt::xaml_typename<bool>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata { winrt::box_value(false), winrt::PropertyChangedCallback(&SettingsCard::OnIsClickEnabledChangedStatic) })
};

