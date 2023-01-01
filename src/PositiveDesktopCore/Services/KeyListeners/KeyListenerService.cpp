#include "pch.h"

#include <unordered_map>

#ifndef KEYLISTENERS_SINGLETON
#include <set>
#endif

#include "KeyListenerService.h"

app::lock_t app::keylistener::KeysListenerService::locker_;
HHOOK app::keylistener::KeysListenerService::hHook_;
#ifndef KEYLISTENERS_SINGLETON
std::set<KeysListenerService*> app::keylistener::KeysListenerService::hooks_;
#else
app::keylistener::KeysListenerService* app::keylistener::KeysListenerService::hook_;
#endif

#define IsKeyPressed(__nVirtualKey) ((GetKeyState(__nVirtualKey) & 0x8000) != 0)

using namespace app::keylistener;

KeysListenerService::~KeysListenerService() {
	if (suspending_) {
		KeysListenerService::removeHook(this);
	}
}

void KeysListenerService::initialize() {
	app::storage::key_config_t config;
	config.map.push_back({ kbe_exit, { 'X', 0x28 /* LCtrl+LWin */ } }); // Exit application.
	config.map.push_back({ kbe_move_window_left, { VK_LEFT, 0x2A /* LCtrl+LWin+LAlt*/ } }); // Move window to the left desktop.
	config.map.push_back({ kbe_move_window_right, { VK_RIGHT, 0x2A /* LCtrl+LWin+LAlt*/ } }); // Move window to the right desktop.
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

LRESULT KeysListenerService::KbdProc(HHOOK /*hHook*/, int /*nCode*/, WPARAM /*wParam*/, KBDLLHOOKSTRUCT const& kbdStruct, bool& handled) noexcept {
	short key { static_cast<char>(kbdStruct.vkCode) };
	if (IsKeyPressed(VK_LCONTROL)) key |= 0x2000;
	if (IsKeyPressed(VK_RCONTROL)) key |= 0x1000;
	if (IsKeyPressed(VK_LWIN)) key |= 0x0800;
	if (IsKeyPressed(VK_RWIN)) key |= 0x0400;
	if (IsKeyPressed(VK_LMENU)) key |= 0x0200;
	if (IsKeyPressed(VK_RMENU)) key |= 0x0100;

	auto itr = keymap_.find(key);
	if (itr != keymap_.end()) {
		handled = true;
		reps::next(subject_, itr->second);
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
