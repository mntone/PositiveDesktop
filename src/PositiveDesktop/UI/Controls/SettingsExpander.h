#pragma once
#include "./UI/Controls/SettingsExpander.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct SettingsExpander: SettingsExpander_base<SettingsExpander> {
		SettingsExpander() noexcept;

		void OnApplyTemplate();

	private:
		void OnIsEnabledChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

	public:
		inline void OnDescriptionChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnHeaderIconChanged(winrt::Microsoft::UI::Xaml::Controls::IconElement const& newValue);
		inline void OnHeaderChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnIsExpandedChanged();
		inline void OnOrientationChanged(winrt::Microsoft::UI::Xaml::Controls::Orientation newValue);

	private:
		static void OnDescriptionChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderIconChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnIsExpandedChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnOrientationChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:
		inline winrt::Windows::Foundation::IInspectable CardContent() const { return GetValue(CardContentProperty_); }
		inline void CardContent(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(CardContentProperty_, value); }

		inline winrt::Windows::Foundation::IInspectable Description() const { return GetValue(DescriptionProperty_); }
		inline void Description(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(DescriptionProperty_, value); }

		inline winrt::Microsoft::UI::Xaml::Controls::IconElement HeaderIcon() const { return GetValue(HeaderIconProperty_).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void HeaderIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(HeaderIconProperty_, value); }

		inline winrt::Windows::Foundation::IInspectable Header() const { return GetValue(HeaderProperty_); }
		inline void Header(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(HeaderProperty_, value); }

		inline bool IsExpanded() const { return winrt::unbox_value<bool>(GetValue(IsExpandedProperty_)); }
		inline void IsExpanded(bool value) const { SetValue(IsExpandedProperty_, winrt::box_value(value)); }

		inline winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const { return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty_)); }
		inline void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation value) const { SetValue(OrientationProperty_, winrt::box_value(value)); }

		static winrt::Microsoft::UI::Xaml::DependencyProperty CardContentProperty() noexcept { return CardContentProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty() noexcept { return DescriptionProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty() noexcept { return HeaderIconProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderProperty() noexcept { return HeaderProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty IsExpandedProperty() noexcept { return IsExpandedProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty OrientationProperty() noexcept { return OrientationProperty_; }

	private:
		winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::IsEnabledChanged_revoker isEnabledChangedRevoker_ {};

		static winrt::Microsoft::UI::Xaml::DependencyProperty CardContentProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty IsExpandedProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty OrientationProperty_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct SettingsExpander: SettingsExpanderT<SettingsExpander, implementation::SettingsExpander> {
	};

}
