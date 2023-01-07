#pragma once
#include "./UI/Controls/SettingsCard.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct SettingsCard: SettingsCard_base<SettingsCard> {
		SettingsCard() noexcept;

		void OnApplyTemplate();

		void OnPointerPressed(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
		void OnPointerReleased(winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;

	private:
		void RegisterButtonEvents();
		void UnregisterButtonEvents() noexcept;

		void OnIsEnabledChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
		void OnControlPointerEntered(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
		void OnControlPointerExited(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
		void OnControlPreviewKeyDown(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args) const;
		void OnControlPreviewKeyUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args) const;

	public:
		inline void OnDescriptionChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnHeaderIconChanged(winrt::Microsoft::UI::Xaml::Controls::IconElement const& newValue);
		inline void OnHeaderChanged(winrt::Windows::Foundation::IInspectable const& newValue);
		inline void OnIsClickEnabledChanged();

	private:
		static void OnDescriptionChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderIconChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnHeaderChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);
		static void OnIsClickEnabledChangedStatic(winrt::Microsoft::UI::Xaml::DependencyObject const& sender, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:
		inline winrt::Microsoft::UI::Xaml::Controls::IconElement ActionIcon() const { return GetValue(ActionIconProperty_).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void ActionIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(ActionIconProperty_, value); }

		inline winrt::hstring ActionIconTooltip() const { return winrt::unbox_value<winrt::hstring>(GetValue(ActionIconTooltipProperty_)); }
		inline void ActionIconTooltip(winrt::param::hstring const& value) const { SetValue(ActionIconTooltipProperty_, winrt::box_value(value)); }

		inline winrt::Windows::Foundation::IInspectable Description() const { return GetValue(DescriptionProperty_); }
		inline void Description(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(DescriptionProperty_, value); }

		inline winrt::Microsoft::UI::Xaml::Controls::IconElement HeaderIcon() const { return GetValue(HeaderIconProperty_).as<winrt::Microsoft::UI::Xaml::Controls::IconElement>(); }
		inline void HeaderIcon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value) const { SetValue(HeaderIconProperty_, value); }

		inline winrt::Windows::Foundation::IInspectable Header() const { return GetValue(HeaderProperty_); }
		inline void Header(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(HeaderProperty_, value); }

		inline bool IsClickEnabled() const { return winrt::unbox_value<bool>(GetValue(IsClickEnabledProperty_)); }
		inline void IsClickEnabled(bool value) const { SetValue(IsClickEnabledProperty_, winrt::box_value(value)); }

		static winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconProperty() noexcept { return ActionIconProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconTooltipProperty() noexcept { return ActionIconTooltipProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty() noexcept { return DescriptionProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty() noexcept { return HeaderIconProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderProperty() noexcept { return HeaderProperty_; }
		static winrt::Microsoft::UI::Xaml::DependencyProperty IsClickEnabledProperty() noexcept { return IsClickEnabledProperty_; }

	private:
		winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::IsEnabledChanged_revoker isEnabledChangedRevoker_ {};
		winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::PointerEntered_revoker pointerEnteredRevoker_ {};
		winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::PointerExited_revoker pointerExitedRevoker_ {};
		winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::PreviewKeyDown_revoker previewKeyDownRevoker_ {};
		winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase::PreviewKeyUp_revoker previewKeyUpRevoker_ {};

		static winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty ActionIconTooltipProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty DescriptionProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderIconProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty HeaderProperty_;
		static winrt::Microsoft::UI::Xaml::DependencyProperty IsClickEnabledProperty_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct SettingsCard: SettingsCardT<SettingsCard, implementation::SettingsCard> {
	};

}
