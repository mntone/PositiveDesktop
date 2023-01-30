#include "pch.h"
#include "KeyVisual.xaml.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h> // Fix buggy IntelliSense :(
#endif

#include "KeyboardHelper.h"

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_KeyVisual { L"PositiveDesktop.UI.Controls.KeyVisual" };
	constexpr std::wstring_view PositiveDesktop_UI_Controls_KeyVisual_Uri { L"ms-appx:///UI/Controls/KeyVisual.xaml" };
}

namespace controls {
	constexpr std::wstring_view Content { L"Content" };
	constexpr std::wstring_view PrefixContent { L"PrefixContent" };
}

namespace states {
	constexpr std::wstring_view Normal { L"Normal" };
	constexpr std::wstring_view Disabled { L"Disabled" };
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;

	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;

	using namespace ::winrt::PositiveDesktop::UI::Controls;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;


KeyVisual::KeyVisual() {
	KeyboardHelper::init();
	props_.DelayInitIfNeeded();
	DefaultStyleKey(box_value(resources::PositiveDesktop_UI_Controls_KeyVisual));
	DefaultStyleResourceUri(Uri(resources::PositiveDesktop_UI_Controls_KeyVisual_Uri));
}

void KeyVisual::OnApplyTemplate() {
	__super::OnApplyTemplate();
	OnKeyChanged(Key());
	IsEnabledChanged(&KeyVisual::OnIsEnabledChangedStatic); // The listener is the same lifecycle to the object.
}

void KeyVisual::OnIsEnabledChangedStatic(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args) {
	VisualStateManager::GoToState(
		sender.as<Control>(),
		unbox_value<bool>(args.NewValue()) ? states::Normal : states::Disabled,
		false);
}

void KeyVisual::OnKeyChanged(VirtualKey newValue) const {
	TextBlock prefixContent { GetTemplateChild(controls::PrefixContent).try_as<TextBlock>() };
	if (!prefixContent) return;

	ContentPresenter content { GetTemplateChild(controls::Content).try_as<ContentPresenter>() };
	if (!content) return;

	auto const& keymaps { KeyboardHelper::Keymap() };
	size_t val { static_cast<size_t>(newValue) };
	if (val < keymaps.size()) {
		auto const& keymap { keymaps[val] };
		auto const& prefix { std::get<1>(keymap) };
		if (!prefix.empty()) {
			prefixContent.Visibility(Visibility::Visible);
			prefixContent.Text(prefix);
		} else {
			prefixContent.Visibility(Visibility::Collapsed);
		}
		IInspectable key { std::get<0>(keymap) };
		if (auto iconSource { key.try_as<BitmapIconSource>() }) {
			IconSourceElement icon;
			icon.Height(20.0);
			icon.IconSource(iconSource);
			icon.Width(20.0);
			content.Content(icon);
		} else {
			content.Content(key);
		}
	} else {
		prefixContent.Visibility(Visibility::Collapsed);
		content.Content(box_value(L"Unknown"));
	}
}

void KeyVisual::OnKeyChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<KeyVisual>(sender.as<winrt::KeyVisual>())->OnKeyChanged(args.NewValue().as<VirtualKey>());
}
