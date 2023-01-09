#include "pch.h"
#include "SettingsCard.h"
#if __has_include("./UI/Controls/SettingsCard.g.cpp")
#include "./UI/Controls/SettingsCard.g.cpp"
#endif

#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_SettingsCard { L"PositiveDesktop.UI.Controls.SettingsCard" };
}

namespace controls {
	constexpr std::wstring_view ActionIcon { L"ActionIcon" };
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

	using namespace ::winrt::Microsoft::UI::Input;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Input;

	using namespace ::winrt::PositiveDesktop::UI::Controls;
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
	OnIsClickEnabledChanged(IsClickEnabled());
	VisualStateManager::GoToState(*this, IsEnabled() ? states::Normal : states::Disabled, true);
	IsEnabledChanged(&SettingsCard::OnIsEnabledChangedStatic); // The listener is the same lifecycle to the object.
}

void SettingsCard::OnIsEnabledChangedStatic(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args) {
	VisualStateManager::GoToState(sender.as<Control>(), unbox_value<bool>(args.NewValue()) ? states::Normal : states::Disabled, true);
}

void SettingsCard::OnPreviewKeyDown(KeyRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPreviewKeyDown(args);

		switch (args.Key()) {
		case VirtualKey::Enter:
		case VirtualKey::Space:
		case VirtualKey::GamepadA:
			VisualStateManager::GoToState(*this, states::Pressed, true);
			break;
		}
	}
}

void SettingsCard::OnPreviewKeyUp(KeyRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPreviewKeyDown(args);

		switch (args.Key()) {
		case VirtualKey::Enter:
		case VirtualKey::Space:
		case VirtualKey::GamepadA:
			VisualStateManager::GoToState(*this, states::Normal, true);
			break;
		}
	}
}

void SettingsCard::OnPointerEntered(PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerEntered(args);

		VisualStateManager::GoToState(*this, states::PointerOver, true);
	}
}

void SettingsCard::OnPointerPressed(PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerPressed(args);

		VisualStateManager::GoToState(*this, states::Pressed, true);
	}
}

void SettingsCard::OnPointerReleased(PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerReleased(args);

		winrt::SettingsCard proj { *this };
		winrt::Windows::Foundation::Numerics::float2 point { args.GetCurrentPoint(proj).Position() };
		if (point.x < 0 || point.y < 0 || point.x > ActualWidth() || point.y > ActualHeight()) {
			VisualStateManager::GoToState(proj, states::Normal, true);
		} else {
			VisualStateManager::GoToState(proj, states::PointerOver, true);
		}
	}
}

void SettingsCard::OnPointerExited(PointerRoutedEventArgs const& args) const {
	if (IsClickEnabled()) {
		__super::OnPointerExited(args);

		VisualStateManager::GoToState(*this, states::Normal, true);
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
	FrameworkElement element { GetTemplateChild(controls::HeaderIcon).try_as<FrameworkElement>() };
	if (element) {
		element.Visibility(newValue != nullptr ? Visibility::Visible : Visibility::Collapsed);
	}
}

void SettingsCard::OnHeaderChanged(IInspectable const& newValue) {
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

void SettingsCard::OnIsClickEnabledChanged(bool newValue) {
	if (newValue) {
		OnButtonIconChanged(true);
		//RegisterButtonEvents();
	} else {
		VisualStateManager::GoToState(*this, states::Normal, true); // Force-reset states.
		OnButtonIconChanged(false);
		//UnregisterButtonEvents();
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
	get_self<SettingsCard>(sender.as<PositiveDesktop::UI::Controls::SettingsCard>())->OnIsClickEnabledChanged(
		winrt::unbox_value<bool>(args.NewValue()));
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
		winrt::PropertyMetadata(winrt::box_value(winrt::hstring(L"\ue974"))))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsCard::ActionIconTooltipProperty_ {
	winrt::DependencyProperty::Register(
		L"ActionIconTooltip",
		winrt::xaml_typename<winrt::hstring>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsCard>(),
		winrt::PropertyMetadata(winrt::box_value(winrt::hstring(L"More"))))
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
		winrt::PropertyMetadata(winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal), winrt::PropertyChangedCallback(&SettingsCard::OnOrientationChangedStatic)))
};
