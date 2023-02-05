#include "pch.h"
#include "KeyConfig.h"

using namespace app::storage;

KeyConfig::KeyConfig(IConfigManager* repository)
	: repository_(repository)
	, raw_(repository_->LoadKeymaps()) {
	if (raw_.keymaps.size() == 0) {
		reset();
	}
}

void KeyConfig::reset() {
	using namespace app::keylistener;

	raw_.separate = kSeparateDefault;

	raw_.keymaps.clear();
	raw_.keymaps.reserve(32);

	raw_.keymaps.push_back({ kbe_exit, { 'X', 0x28 /* LCtrl+LWin */ } }); // Exit application.
	raw_.keymaps.push_back({ kbe_settings, { 'I', 0x28 /* LCtrl+LWin */ } }); // Show settings window.
	raw_.keymaps.push_back({ kbe_topmost_toggle, { 'T', 0x2A /* LCtrl+LWin+LAlt*/ } }); // Toggle topmost window.

	// Switch desktop.
	for (char i = 0; i < 9; ++i) {
		raw_.keymaps.push_back({ addingDesktop(kbe_switch, i), { '1' + i, 0x28 /* LCtrl+LWin*/ } });
	}
	raw_.keymaps.push_back({ kbe_switch_desktop10, { '0', 0x28 /* LCtrl+LWin*/ } });
	raw_.keymaps.push_back({ kbe_switch_first, { VK_HOME, 0x28 /* LCtrl+LWin*/ } });
	raw_.keymaps.push_back({ kbe_switch_last, { VK_END, 0x28 /* LCtrl+LWin*/ } });
	raw_.keymaps.push_back({ kbe_switch_left, { VK_PRIOR, 0x28 /* LCtrl+LWin*/ } });
	raw_.keymaps.push_back({ kbe_switch_right, { VK_NEXT, 0x28 /* LCtrl+LWin*/ } });

	// Move window to the X desktop and switch.
	for (char i = 0; i < 9; ++i) {
		raw_.keymaps.push_back({ addingDesktop(kbe_move_window_and_switch, i), { '1' + i, 0x2A /* LCtrl+LWin+LAlt*/ } });
	}
	raw_.keymaps.push_back({ kbe_move_window_and_switch_desktop10, { '0', 0x2A /* LCtrl+LWin+LAlt*/ } });
	raw_.keymaps.push_back({ kbe_move_window_and_switch_first, { VK_HOME, 0x2A /* LCtrl+LWin+LAlt*/ } });
	raw_.keymaps.push_back({ kbe_move_window_and_switch_last, { VK_END, 0x2A /* LCtrl+LWin+LAlt*/ } });
	raw_.keymaps.push_back({ kbe_move_window_and_switch_left, { VK_LEFT, 0x2A /* LCtrl+LWin+LAlt*/ }, { VK_PRIOR, 0x2A /* LCtrl+LWin+LAlt*/ } });
	raw_.keymaps.push_back({ kbe_move_window_and_switch_right, { VK_RIGHT, 0x2A /* LCtrl+LWin+LAlt*/ }, { VK_NEXT, 0x2A /* LCtrl+LWin+LAlt*/ } });
	raw_.keymaps.push_back({ kbe_move_window_and_switch_new_last, { 'D', 0x2A /* LCtrl+LWin+LAlt*/ } });
}

void KeyConfig::load() {
	raw_ = repository_->LoadKeymaps();
}

void KeyConfig::store() const {
	repository_->store(raw_);
}
