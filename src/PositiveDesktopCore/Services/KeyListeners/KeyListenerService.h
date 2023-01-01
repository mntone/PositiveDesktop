#pragma once
#include "kbevent_t.h"

#include <unordered_map>

#include "Common/key_config_t.h"
#include "Common/lock_t.h"
#include "Common/Reps.h"

#define KEYLISTENERS_SINGLETON

namespace app::keylistener {

	class KeysListenerService final {
	public:
		~KeysListenerService();

		void initialize();
		void updateConfig(app::storage::key_config_t const& config) noexcept;

		void suspend();
		void resume();

		void addObserver(reps::observer_t& observer) noexcept {
			subject_.addObserver(observer);
		}

		void clearObserver() noexcept {
			subject_.clearObserver();
		}

	private:
		void updateConfigPrivate(app::storage::key_config_t const& config) noexcept;

		LRESULT KbdProc(HHOOK hHook, int nCode, WPARAM wParam, KBDLLHOOKSTRUCT const& kbdStruct, bool& handled) noexcept;
		static LRESULT CALLBACK KbdProcStatic(int nCode, WPARAM wParam, LPARAM lParam) noexcept;

		static void addHook(KeysListenerService* that);
		static void removeHook(KeysListenerService* that);

	private:
		char previousVirtualKey_;
		bool suspending_;
		std::unordered_map<short, kbevent_t> keymap_;
		reps::single_subject_t subject_;

		static app::lock_t locker_;
		static HHOOK hHook_;
#ifndef KEYLISTENERS_SINGLETON
		static std::set<KeysListenerService*> hooks_;
#else
		static KeysListenerService* hook_;
#endif
	};

}
