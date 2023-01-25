#pragma once
#include "UI/Controls/KeyInputBox.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct KeyInputBox: KeyInputBox_base<KeyInputBox> {
		KeyInputBox();

		void OnApplyTemplate();

		void OnKeyDown(Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args);
		void OnKeyUp(Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args);

	private:
		void SetVisibility(param::hstring const& controlName, uint32_t oldValue, uint32_t newValue, uint32_t mask) const;

		inline void OnKeyChanged(Windows::System::VirtualKey newValue) const;
		static void OnKeyChangedStatic(
			Microsoft::UI::Xaml::DependencyObject const& sender,
			Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

		inline void OnKeyModifiersChanged(
			std::optional<Windows::System::VirtualKeyModifiers> oldValue,
			Windows::System::VirtualKeyModifiers newValue) const;
		static void OnKeyModifiersChangedStatic(
			Microsoft::UI::Xaml::DependencyObject const& sender,
			Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:  // - Properties
		inline Windows::System::VirtualKey Key() const { return GetValue(props_.Key).as<Windows::System::VirtualKey>(); }
		void Key(Windows::System::VirtualKey value) const;

		inline Windows::System::VirtualKeyModifiers KeyModifiers() const { return GetValue(props_.KeyModifiers).as<Windows::System::VirtualKeyModifiers>(); }
		void KeyModifiers(Windows::System::VirtualKeyModifiers value) const;

		inline static Microsoft::UI::Xaml::DependencyProperty KeyProperty() noexcept { return props_.Key; }
		inline static Microsoft::UI::Xaml::DependencyProperty KeyModifiersProperty() noexcept { return props_.KeyModifiers; }

	private: // - Variables
		struct DependencyProperties final {
			void DelayInitIfNeeded();

			bool initialized_ { false };
			winrt::Microsoft::UI::Xaml::DependencyProperty InputScope { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Key { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty KeyModifiers { nullptr };
		};
		static DependencyProperties props_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct KeyInputBox: KeyInputBoxT<KeyInputBox, implementation::KeyInputBox> {
	};

}
