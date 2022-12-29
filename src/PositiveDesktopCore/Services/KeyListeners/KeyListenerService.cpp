#include "pch.h"

#ifndef KEYLISTENERS_SINGLETON
#include <set>
#endif

#include "KeyListenerService.h"

HHOOK app::keylistener::KeysListenerService::hHook_;
#ifndef KEYLISTENERS_SINGLETON
std::set<KeysListenerService*> app::keylistener::KeysListenerService::hooks_;
#else
app::keylistener::KeysListenerService* app::keylistener::KeysListenerService::hook_;
#endif

#include "kbevent_t.h"

#define IsKeyPressed(__nVirtualKey) ((GetKeyState(__nVirtualKey) & 0x8000) != 0)

using namespace app::keylistener;

KeysListenerService::~KeysListenerService() {
	KeysListenerService::removeHook(this);
}

void KeysListenerService::initialize() {
	KeysListenerService::addHook(this);
}

LRESULT KeysListenerService::KbdProc(HHOOK hHook, int nCode, WPARAM wParam, KBDLLHOOKSTRUCT const& kbdStruct, bool& handled) noexcept {
	// Win+Ctrl+X: Exit application.
	if ('X' == kbdStruct.vkCode && IsKeyPressed(VK_LCONTROL) && IsKeyPressed(VK_LWIN)) {
		handled = true;
		reps::next(subject_, kbe_exit);
		return TRUE;
	}
	return FALSE;
}

#pragma region Static implementation

void KeysListenerService::addHook(KeysListenerService* that) {
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
