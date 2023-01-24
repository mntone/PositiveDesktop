#pragma once
#include "UI/Controls/KeyTop.g.h"

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct KeyTop: KeyTop_base<KeyTop> {
		KeyTop();

		void OnApplyTemplate();

	private:
		inline void OnKeyTopChanged(Windows::System::VirtualKey newValue) const;
		static void OnKeyTopChangedStatic(
			Microsoft::UI::Xaml::DependencyObject const& sender,
			Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	public:  // - Properties
		inline bool HasError() const { return GetValue(props_.HasError).as<bool>(); }
		void HasError(bool value) const;

		inline Windows::System::VirtualKey Key() const { return GetValue(props_.Key).as<Windows::System::VirtualKey>(); }
		void Key(Windows::System::VirtualKey value) const;

		inline static Microsoft::UI::Xaml::DependencyProperty KeyProperty() noexcept { return props_.Key; }
		inline static Microsoft::UI::Xaml::DependencyProperty HasErrorProperty() noexcept { return props_.HasError; }

	private: // - Variables
		std::array<IInspectable, 167> keymap_;
		Microsoft::UI::Xaml::Controls::TextBlock content_;

		struct DependencyProperties final {
			void DelayInitIfNeeded();

			bool initialized_ { false };
			winrt::Microsoft::UI::Xaml::DependencyProperty HasError { nullptr };
			winrt::Microsoft::UI::Xaml::DependencyProperty Key { nullptr };
		};
		static DependencyProperties props_;
	};

}

namespace winrt::PositiveDesktop::UI::Controls::factory_implementation {

	struct KeyTop: KeyTopT<KeyTop, implementation::KeyTop> {
	};

}
