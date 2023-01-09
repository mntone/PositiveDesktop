#pragma once
#include "./UI/Controls/SettingsExpander.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct SettingsExpander: SettingsExpander_base<SettingsExpander> {
		SettingsExpander() noexcept;

		void OnApplyTemplate();

	private:
		void RegisterButtonEvents();

		static void OnControlPointerEnteredStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
		static void OnControlPointerPressedStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
		static void OnControlPointerReleasedStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
		static void OnControlPointerExitedStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
		static void OnIsEnabledChangedStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

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
		inline winrt::Windows::Foundation::IInspectable CardContent() const { return GetValue(props_.CardContent); }
		inline void CardContent(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.CardContent, value); }

		inline winrt::Windows::Foundation::IInspectable Description() const { return GetValue(props_.Description); }
		inline void Description(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.Description, value); }

		inline winrt::Microsoft::UI::Xaml::Controls::IconElement HeaderIcon() const { return GetValue(props_.HeaderIcon).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void HeaderIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(props_.HeaderIcon, value); }

		inline winrt::Windows::Foundation::IInspectable Header() const { return GetValue(props_.Header); }
		inline void Header(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.Header, value); }

		inline bool IsExpanded() const { return winrt::unbox_value<bool>(GetValue(props_.IsExpanded)); }
		inline void IsExpanded(bool value) const { SetValue(props_.IsExpanded, winrt::box_value(value)); }

		inline winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const { return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(props_.Orientation)); }
		inline void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation value) const { SetValue(props_.Orientation, winrt::box_value(value)); }

		static winrt::Microsoft::UI::Xaml::DependencyProperty CardContentProperty() noexcept { return props_.CardContent; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty() noexcept { return props_.Description; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty() noexcept { return props_.HeaderIcon; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderProperty() noexcept { return props_.Header; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty IsExpandedProperty() noexcept { return props_.IsExpanded; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty OrientationProperty() noexcept { return props_.Orientation; }

	private:
		winrt::Microsoft::UI::Xaml::Controls::ContentControl cardContentControl_;
		//winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::IsEnabledChanged_revoker isEnabledChangedRevoker_ {};

		struct DependencyProperties final {
			void DelayInitIfNeeded();

			bool initialized_ { false };
			winrt::Microsoft::UI::Xaml::DependencyProperty CardContent { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Description { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIcon { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Header  { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty IsExpanded { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Orientation { nullptr };
		};
		static DependencyProperties props_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct SettingsExpander: SettingsExpanderT<SettingsExpander, implementation::SettingsExpander> {
	};

}
