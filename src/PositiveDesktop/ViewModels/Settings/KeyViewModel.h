#pragma once
#include "ViewModels/Settings/KeyViewModel.g.h"

#include "ViewModels/ObservableObject.h"

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct KeyViewModel: KeyViewModelT<KeyViewModel, ::winrt::PositiveDesktop::ViewModels::implementation::ObservableObject> {
		KeyViewModel() noexcept = default;

	public:  // - Properties
		inline constexpr Windows::System::VirtualKey Key() const noexcept {
			return key_;
		}
		void Key(Windows::System::VirtualKey value) noexcept;

		inline constexpr Windows::System::VirtualKeyModifiers KeyModifiers() const noexcept {
			return keyModifiers_;
		}
		void KeyModifiers(Windows::System::VirtualKeyModifiers value) noexcept;

		inline constexpr Windows::System::VirtualKey Key2() const noexcept {
			return key2_;
		}
		void Key2(Windows::System::VirtualKey value) noexcept;

		inline constexpr Windows::System::VirtualKeyModifiers KeyModifiers2() const noexcept {
			return keyModifiers2_;
		}
		void KeyModifiers2(Windows::System::VirtualKeyModifiers value) noexcept;

	private:
		Windows::System::VirtualKey key_, key2_;
		Windows::System::VirtualKeyModifiers keyModifiers_, keyModifiers2_;
	};

}
