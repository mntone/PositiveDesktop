#pragma once

namespace winrt::PositiveDesktop::UI::Controls::implementation {

	struct KeyboardHelper final {
		using keymap_type = std::tuple<Windows::Foundation::IInspectable, hstring>;
		using keymap_array_type = std::array<keymap_type, 227>;

		static void init();

		static inline keymap_array_type Keymap() noexcept {
			return keymap_;
		}

	private:
		static keymap_array_type InitKeymap();

		static keymap_type GetKeymap(std::nullptr_t);
		static keymap_type GetKeymap(std::wstring_view keymapText);
		static keymap_type GetKeymap(std::wstring_view keymapText, winrt::hstring const& prefixText);
		static keymap_type GetKeymap(UINT code);

	private:
		static HKL keyboardLayout_;
		static keymap_array_type keymap_;
	};

}
