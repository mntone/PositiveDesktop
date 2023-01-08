#include "pch.h"
#include "SettingsExpander.h"
#if __has_include("./UI/Controls/SettingsExpander.g.cpp")
#include "./UI/Controls/SettingsExpander.g.cpp"
#endif

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_SettingsExpander { L"PositiveDesktop.UI.Controls.SettingsExpander" };
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
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Input;
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

SettingsExpander::SettingsExpander() noexcept {
	DefaultStyleKey(box_value(resources::PositiveDesktop_UI_Controls_SettingsExpander));
}

void SettingsExpander::OnApplyTemplate() {
	OnOrientationChanged(Orientation());
	OnDescriptionChanged(Description());
	OnHeaderChanged(Header());
	OnHeaderIconChanged(HeaderIcon());
	VisualStateManager::GoToState(*this, IsEnabled() ? states::Normal : states::Disabled, true);
	isEnabledChangedRevoker_ = IsEnabledChanged(auto_revoke, { this, &SettingsExpander::OnIsEnabledChanged });
}

void SettingsExpander::OnIsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e) {
	VisualStateManager::GoToState(*this, unbox_value<bool>(e.NewValue()) ? states::Normal : states::Disabled, true);
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
	//VisualStateManager::GoToState(*this, newValue != nullptr ? states::TextAndIconState : states::TextOnlyState, true);
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

#include "Common/Debug.h"

void SettingsExpander::OnIsExpandedChanged() {
	OutputDebugStringFW(L"%d\n", IsExpanded());
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
	get_self<SettingsExpander>(sender.as<PositiveDesktop::UI::Controls::SettingsExpander>())->OnOrientationChanged(
		winrt::unbox_value<winrt::Orientation>(args.NewValue()));
}

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsExpander::CardContentProperty_ {
	winrt::DependencyProperty::Register(
		L"CardContent",
		winrt::xaml_typename<winrt::IInspectable>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsExpander>(),
		winrt::PropertyMetadata(winrt::IInspectable { nullptr }))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsExpander::DescriptionProperty_ {
	winrt::DependencyProperty::Register(
		L"Description",
		winrt::xaml_typename<winrt::IInspectable>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsExpander>(),
		winrt::PropertyMetadata(winrt::IInspectable { nullptr }, winrt::PropertyChangedCallback(&SettingsExpander::OnDescriptionChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsExpander::HeaderIconProperty_ {
	winrt::DependencyProperty::Register(
		L"HeaderIcon",
		winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconElement>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsExpander>(),
		winrt::PropertyMetadata(winrt::Microsoft::UI::Xaml::Controls::IconElement { nullptr }, winrt::PropertyChangedCallback(&SettingsExpander::OnHeaderIconChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsExpander::HeaderProperty_ {
	winrt::DependencyProperty::Register(
		L"Header",
		winrt::xaml_typename<winrt::IInspectable>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsExpander>(),
		winrt::PropertyMetadata(winrt::IInspectable { nullptr }, winrt::PropertyChangedCallback(&SettingsExpander::OnHeaderChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsExpander::IsExpandedProperty_ {
	winrt::DependencyProperty::Register(
		L"IsExpanded",
		winrt::xaml_typename<bool>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsExpander>(),
		winrt::PropertyMetadata(winrt::box_value(false), winrt::PropertyChangedCallback(&SettingsExpander::OnIsExpandedChangedStatic)))
};

winrt::Microsoft::UI::Xaml::DependencyProperty SettingsExpander::OrientationProperty_ {
	winrt::DependencyProperty::Register(
		L"Orientation",
		winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
		winrt::xaml_typename<winrt::PositiveDesktop::UI::Controls::SettingsExpander>(),
		winrt::PropertyMetadata(winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal), winrt::PropertyChangedCallback(&SettingsExpander::OnOrientationChangedStatic)))
};
