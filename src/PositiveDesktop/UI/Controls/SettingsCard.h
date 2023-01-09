#pragma once
#include "./UI/Controls/SettingsCard.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct SettingsCard: SettingsCard_base<SettingsCard> {
		SettingsCard() noexcept;

		void OnApplyTemplate();

		void OnPointerEntered(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const;
		void OnPointerPressed(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const;
		void OnPointerReleased(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const;
		void OnPointerExited(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args) const;

		void OnKeyDown(winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args) const;
		void OnKeyUp(winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args) const;

	private:
		static void OnIsEnabledChangedStatic(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

		inline void OnButtonIconChanged(bool isClickEnabled);

	public:
		inline void OnDescriptionChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnHeaderIconChanged(winrt::Microsoft::UI::Xaml::Controls::IconElement const& newValue);
		inline void OnHeaderChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnIsClickEnabledChanged(bool newValue);
		inline void OnOrientationChanged(winrt::Microsoft::UI::Xaml::Controls::Orientation newValue);

	private:
		static void OnDescriptionChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderIconChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnIsClickEnabledChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnOrientationChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:
		inline winrt::Microsoft::UI::Xaml::Controls::IconElement ActionIcon() const { return GetValue(props_.ActionIcon).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void ActionIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(props_.ActionIcon, value); }

		inline winrt::hstring ActionIconTooltip() const { return winrt::unbox_value<winrt::hstring>(GetValue(props_.ActionIconTooltip)); }
		inline void ActionIconTooltip(winrt::param::hstring const& value) const { SetValue(props_.ActionIconTooltip, winrt::box_value(value)); }

		inline winrt::Windows::Foundation::IInspectable Description() const { return GetValue(props_.Description); }
		inline void Description(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.Description, value); }

		inline winrt::Microsoft::UI::Xaml::Controls::IconElement HeaderIcon() const { return GetValue(props_.HeaderIcon).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void HeaderIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(props_.HeaderIcon, value); }

		inline winrt::Windows::Foundation::IInspectable Header() const { return GetValue(props_.Header); }
		inline void Header(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(props_.Header, value); }

		inline bool IsClickEnabled() const { return winrt::unbox_value<bool>(GetValue(props_.IsClickEnabled)); }
		inline void IsClickEnabled(bool value) const { SetValue(props_.IsClickEnabled, winrt::box_value(value)); }

		inline winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const { return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(props_.Orientation)); }
		inline void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation value) const { SetValue(props_.Orientation, winrt::box_value(value)); }

		static winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconProperty() noexcept { return props_.ActionIcon; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconTooltipProperty() noexcept { return props_.ActionIconTooltip; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty() noexcept { return props_.Description; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty() noexcept { return props_.HeaderIcon; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderProperty() noexcept { return props_.Header; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty IsClickEnabledProperty() noexcept { return props_.IsClickEnabled; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty OrientationProperty() noexcept { return props_.Orientation; }

	private:
		struct DependencyProperties final {
			void DelayInitIfNeeded();

			bool initialized_ { false };
			winrt::Microsoft::UI::Xaml::DependencyProperty ActionIcon { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconTooltip { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Description { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIcon { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Header  { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty IsClickEnabled { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Orientation { nullptr };
		};
		static DependencyProperties props_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct SettingsCard: SettingsCardT<SettingsCard, implementation::SettingsCard> {
	};

}
