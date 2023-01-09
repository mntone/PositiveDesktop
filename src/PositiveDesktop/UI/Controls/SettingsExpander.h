#pragma once
#include "./UI/Controls/SettingsExpander.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct SettingsExpander: SettingsExpander_base<SettingsExpander> {
		SettingsExpander() noexcept;

		void OnApplyTemplate();

		void OnPointerEntered(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const;
		void OnPointerExited(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const;

	private:
		//static void OnIsEnabledChangedStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:
		inline void OnDescriptionChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnHeaderIconChanged(winrt::Microsoft::UI::Xaml::Controls::IconElement const& newValue);
		inline void OnOrientationChanged(winrt::Microsoft::UI::Xaml::Controls::Orientation newValue);

	private:
		static void OnDescriptionChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderIconChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnOrientationChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:
		inline winrt::Windows::Foundation::IInspectable Description() const { return GetValue(props_.Description); }
		inline void Description(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.Description, value); }

		inline winrt::Windows::Foundation::IInspectable ExpanderContent() const { return GetValue(props_.ExpanderContent); }
		inline void ExpanderContent(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.ExpanderContent, value); }

		inline winrt::Microsoft::UI::Xaml::Controls::IconElement HeaderIcon() const { return GetValue(props_.HeaderIcon).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void HeaderIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(props_.HeaderIcon, value); }

		inline winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const { return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(props_.Orientation)); }
		inline void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation value) const { SetValue(props_.Orientation, winrt::box_value(value)); }

		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty() noexcept { return props_.Description; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty ExpanderContentProperty() noexcept { return props_.ExpanderContent; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty() noexcept { return props_.HeaderIcon; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty OrientationProperty() noexcept { return props_.Orientation; }

	private:
		struct DependencyProperties final {
			void DelayInitIfNeeded();

			bool initialized_ { false };
			winrt::Microsoft::UI::Xaml::DependencyProperty Description { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty ExpanderContent { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIcon { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Orientation { nullptr };
		};
		static DependencyProperties props_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct SettingsExpander: SettingsExpanderT<SettingsExpander, implementation::SettingsExpander> {
	};

}
