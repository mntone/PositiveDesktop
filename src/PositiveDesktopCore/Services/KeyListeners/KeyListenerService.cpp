#include "pch.h"

#ifndef KEYLISTENERS_SINGLETON
#include <set>
#endif

#include "KeyListenerService.h"

#ifdef DEBUG_KEYINPUT
#include "Common/Debug.h"
#endif

app::lock_t app::keylistener::KeysListenerService::locker_;
HHOOK app::keylistener::KeysListenerService::hHook_ = nullptr;
#ifndef KEYLISTENERS_SINGLETON
std::set<KeysListenerService*> app::keylistener::KeysListenerService::hooks_;
#else
app::keylistener::KeysListenerService* app::keylistener::KeysListenerService::hook_ = nullptr;
#endif

#define IsKeyPressed(__nVirtualKey) ((GetKeyState(__nVirtualKey) & 0x8000) != 0)

using namespace app::keylistener;

KeysListenerService::~KeysListenerService() {
	clearObserver();

	if (!suspending_) {
		KeysListenerService::removeHook(this);
	}
}

void KeysListenerService::initialize() {
	app::storage::key_config_t config;
	config.map.reserve(32);
	config.map.push_back({ kbe_exit, { 'X', 0x28 /* LCtrl+LWin */ } }); // Exit application.
	config.map.push_back({ kbe_settings, { 'I', 0x28 /* LCtrl+LWin */ } }); // Show settings window.

	// Switch desktop.
	for (char i = 0; i < 9; ++i) {
		config.map.push_back({ addingDesktop(kbe_switch, i), { '1' + i, 0x28 /* LCtrl+LWin*/ } });
	}
	config.map.push_back({ kbe_switch_desktop10, { '0', 0x28 /* LCtrl+LWin*/ } });
	config.map.push_back({ kbe_switch_first, { VK_HOME, 0x28 /* LCtrl+LWin*/ } });
	config.map.push_back({ kbe_switch_last, { VK_END, 0x28 /* LCtrl+LWin*/ } });
	config.map.push_back({ kbe_switch_left, { VK_PRIOR, 0x28 /* LCtrl+LWin*/ } });
	config.map.push_back({ kbe_switch_right, { VK_NEXT, 0x28 /* LCtrl+LWin*/ } });

	// Move window to the X desktop and switch.
	for (char i = 0; i < 9; ++i) {
		config.map.push_back({ addingDesktop(kbe_move_window_and_switch, i), { '1' + i, 0x2A /* LCtrl+LWin+LAlt*/ } });
	}
	config.map.push_back({ kbe_move_window_and_switch_desktop10, { '0', 0x2A /* LCtrl+LWin+LAlt*/ } });
	config.map.push_back({ kbe_move_window_and_switch_first, { VK_HOME, 0x2A /* LCtrl+LWin+LAlt*/ } });
	config.map.push_back({ kbe_move_window_and_switch_last, { VK_END, 0x2A /* LCtrl+LWin+LAlt*/ } });
	config.map.push_back({ kbe_move_window_and_switch_left, { VK_LEFT, 0x2A /* LCtrl+LWin+LAlt*/ }, { VK_PRIOR, 0x2A /* LCtrl+LWin+LAlt*/ } });
	config.map.push_back({ kbe_move_window_and_switch_right, { VK_RIGHT, 0x2A /* LCtrl+LWin+LAlt*/ }, { VK_NEXT, 0x2A /* LCtrl+LWin+LAlt*/ } });
	config.map.push_back({ kbe_move_window_and_switch_new, { 'D', 0x2A /* LCtrl+LWin+LAlt*/ } });

	// Toggle topmost window.
	config.map.push_back({ kbe_topmost_toggle, { 'T', 0x2A /* LCtrl+LWin+LAlt*/ } });

	updateConfigPrivate(config);

	KeysListenerService::addHook(this);
}

void KeysListenerService::suspend() {
	WINRT_ASSERT(!suspending_);

	suspending_ = true;
	KeysListenerService::removeHook(this);
}

void KeysListenerService::resume() {
	WINRT_ASSERT(suspending_);

	suspending_ = false;
	KeysListenerService::addHook(this);
}

void KeysListenerService::updateConfig(app::storage::key_config_t const& config) noexcept {
	if (suspending_) {
		updateConfigPrivate(config);
	} else {
		KeysListenerService::removeHook(this);
		updateConfigPrivate(config);
		KeysListenerService::addHook(this);
	}
}

void KeysListenerService::updateConfigPrivate(app::storage::key_config_t const& config) noexcept {
	std::unordered_map<short, kbevent_t> keymap;
	for (app::storage::keymap_t const& key : config.map) {
		if (!key.key1.empty()) {
			keymap.emplace(key.key1.raw, key.ev);
		}
		if (!key.key2.empty()) {
			keymap.emplace(key.key2.raw, key.ev);
		}
	}
	keymap_ = std::move(keymap);
}

LRESULT KeysListenerService::KbdProc(HHOOK /*hHook*/, int /*nCode*/, WPARAM wParam, KBDLLHOOKSTRUCT const& kbdStruct, bool& handled) noexcept {
	if (WM_KEYUP == wParam || WM_SYSKEYUP == wParam) {
		previousVirtualKey_ = 0;
		return FALSE;
	}
	char virtualKey = static_cast<char>(kbdStruct.vkCode);
	if (virtualKey == previousVirtualKey_) return FALSE;
	previousVirtualKey_ = virtualKey;

	short key { virtualKey };
	if (IsKeyPressed(VK_LCONTROL)) key |= 0x2000;
	if (IsKeyPressed(VK_RCONTROL)) key |= 0x1000;
	if (IsKeyPressed(VK_LWIN)) key |= 0x0800;
	if (IsKeyPressed(VK_RWIN)) key |= 0x0400;
	if (IsKeyPressed(VK_LMENU)) key |= 0x0200;
	if (IsKeyPressed(VK_RMENU)) key |= 0x0100;

#ifdef DEBUG_KEYINPUT
	if (0x30 <= key && key <= 0x5A) {
		OutputDebugStringFW(L"flag: %04X, key: %08X (%c)\n", kbdStruct.flags, key, virtualKey);
	} else if (VK_NUMPAD0 <= key && key <= VK_NUMPAD9) {
		OutputDebugStringFW(L"flag: %04X, key: %08X (Numpad %c)\n", kbdStruct.flags, key, virtualKey - VK_NUMPAD0 + '0');
	} else {
		OutputDebugStringFW(L"flag: %04X, key: %08X\n", kbdStruct.flags, key);
	}
#endif

	auto itr = keymap_.find(key);
	if (itr != keymap_.end()) {
		handled = true;
		next(itr->second);
		return TRUE;
	}
	return FALSE;
}

#pragma region Static implementation

void KeysListenerService::addHook(KeysListenerService* that) {
	app::lock_guard<app::lock_t> lock { locker_ };
#ifndef KEYLISTENERS_SINGLETON
	if (hooks_.empty()) {
		HHOOK hHook = SetWindowsHookExW(
			WH_KEYBOARD_LL,
			&KeysListenerService::KbdProcStatic,
			nullptr,
			0);
		winrt::check_pointer(hHook);
		hHook_ = hHook;
	}

	hooks_.emplace(that);
#else
	WINRT_ASSERT(!hook_);

	HHOOK hHook = SetWindowsHookExW(
		WH_KEYBOARD_LL,
		&KeysListenerService::KbdProcStatic,
		nullptr,
		0);
	winrt::check_pointer(hHook);
	hHook_ = hHook;
	hook_ = that;
#endif
}

void KeysListenerService::removeHook(KeysListenerService* that) {
	app::lock_guard<app::lock_t> lock { locker_ };
#ifndef KEYLISTENERS_SINGLETON
	auto itr = hooks_.find(that);
	if (itr == hooks_.end()) {
		// TODO: error log
		throw winrt::hresult_invalid_argument();
	}
	hooks_.erase(that);

	if (hooks_.empty()) {
		HHOOK hHook = std::exchange(hHook_, nullptr);
		BOOL result = UnhookWindowsHookEx(hHook);
		if (!result) {
			// TODO: error log
		}
	}
#else
	WINRT_ASSERT(hook_);
	hook_ = nullptr;

	HHOOK hHook = std::exchange(hHook_, nullptr);
	BOOL result = UnhookWindowsHookEx(hHook);
	if (!result) {
		// TODO: error log
	}
#endif
}

LRESULT KeysListenerService::KbdProcStatic(int nCode, WPARAM wParam, LPARAM lParam) noexcept {
	LPKBDLLHOOKSTRUCT kbdStruct = reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam);

	bool handled = false;
	app::lock_guard<app::lock_t> lock { locker_ };
#ifndef KEYLISTENERS_SINGLETON
	for (auto hook : hooks_) {
		LRESULT result = hook->KbdProc(hHook_, nCode, wParam, *kbdStruct, handled);
		if (handled) {
			return result;
		}
	}
#else
	LRESULT result = hook_->KbdProc(hHook_, nCode, wParam, *kbdStruct, handled);
	if (handled) {
		return result;
	}
#endif
	return CallNextHookEx(hHook_, nCode, wParam, lParam);
}

#pragma endregion
