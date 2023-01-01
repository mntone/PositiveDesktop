#pragma once

#include <vector>

namespace app::storage {

#pragma pack(2)
	union key_t final {
		struct {
			char key;
			char extra;
		};
		short raw;

		key_t(): key(0), extra() { }
		key_t(short raw): raw(raw) { }
		key_t(char key, char extra): key(key), extra(extra) { }

		constexpr bool empty() const noexcept { return raw == 0; }

		constexpr bool leftCtrl() const noexcept { return 0x2000 & raw; }
		constexpr bool rightCtrl() const noexcept { return 0x1000 & raw; }
		constexpr bool leftWin() const noexcept { return 0x0800 & raw; }
		constexpr bool rightWin() const noexcept { return 0x0400 & raw; }
		constexpr bool leftAlt() const noexcept { return 0x0200 & raw; }
		constexpr bool rightAlt() const noexcept { return 0x0100 & raw; }
	};
#pragma pack()

	struct keymap_t final {
		app::keylistener::kbevent_t ev;
		key_t key1;
		key_t key2;

		keymap_t(app::keylistener::kbevent_t ev): ev(ev), key1(), key2() { }
		keymap_t(app::keylistener::kbevent_t ev, key_t key1): ev(ev), key1(key1), key2() { }
		keymap_t(app::keylistener::kbevent_t ev, key_t key1, key_t key2): ev(ev), key1(key1), key2(key2) { }
	};

	struct key_config_t final {
		bool separate;
		std::vector<app::storage::keymap_t> map;
	};

}
