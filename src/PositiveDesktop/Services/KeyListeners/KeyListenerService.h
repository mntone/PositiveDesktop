#pragma once
#include "Common/Reps.h"

#define KEYLISTENERS_SINGLETON

namespace app::keylisteners {

	class KeysListenerService final {
	public:
		~KeysListenerService();

		void initialize();

		void addObserver(reps::observer_t& observer) noexcept {
			subject_.addObserver(observer);
		}

		void clearObserver() noexcept {
			subject_.clearObserver();
		}

	private:
		LRESULT KbdProc(HHOOK hHook, int nCode, WPARAM wParam, KBDLLHOOKSTRUCT const& kbdStruct, bool& handled) noexcept;
		static LRESULT CALLBACK KbdProcStatic(int nCode, WPARAM wParam, LPARAM lParam) noexcept;

		static void addHook(KeysListenerService* that);
		static void removeHook(KeysListenerService* that);

	private:
		reps::single_subject_t subject_;

		static HHOOK hHook_;
#ifndef KEYLISTENERS_SINGLETON
		static std::set<KeysListenerService*> hooks_;
#else
		static KeysListenerService* hook_;
#endif
	};

}
