#pragma once
#include "kbevent_t.h"

#include <unordered_map>

#include "Common/lock_t.h"
#include "Common/Reps.h"

#include "Services/Storages/KeyConfig.h"

#define KEYLISTENERS_SINGLETON

namespace app::keylistener {

	class KeysListenerService final: public reps::single_subject_t<kbevent_t> {
	public:
		KeysListenerService() noexcept: previousVirtualKey_(0), suspending_(true), keymap_() { }
		~KeysListenerService();

		void initialize();
		void updateConfig(std::shared_ptr<app::storage::KeyConfig> config) noexcept;

		void suspend();
		void resume();

	private:
		void updateConfigPrivate(std::shared_ptr<app::storage::KeyConfig> config) noexcept;

		LRESULT KbdProc(HHOOK hHook, int nCode, WPARAM wParam, KBDLLHOOKSTRUCT const& kbdStruct, bool& handled) noexcept;
		static LRESULT CALLBACK KbdProcStatic(int nCode, WPARAM wParam, LPARAM lParam) noexcept;

		static void addHook(KeysListenerService* that);
		static void removeHook(KeysListenerService* that);

	private:
		char previousVirtualKey_;
		bool suspending_;
		std::unordered_map<short, kbevent_t> keymap_;

		static app::lock_t locker_;
		static HHOOK hHook_;
#ifndef KEYLISTENERS_SINGLETON
		static std::set<KeysListenerService*> hooks_;
#else
		static KeysListenerService* hook_;
#endif
	};

}
