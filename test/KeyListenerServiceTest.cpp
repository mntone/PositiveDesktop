#include "pch.h"
#include "Services/KeyListeners/kbevent_t.h"
#include "Services/KeyListeners/KeyListenerService.h"

#include "Common/RepsUtil.h"

using namespace app::keylistener;

#include <array>
#include <memory>
#include <type_traits>

constexpr bool IsExtendedKey(unsigned char virtualKey) {
	constexpr std::array<unsigned char, 11> extendedKey {
		VK_CONTROL,
		VK_LCONTROL,
		VK_RCONTROL,
		VK_MENU,
		VK_LMENU,
		VK_RMENU,
		VK_LWIN,
		VK_RWIN,

		VK_SNAPSHOT,
		VK_INSERT,
		VK_DELETE,
	};
	return std::find(extendedKey.cbegin(), extendedKey.cend(), virtualKey) != extendedKey.cend();
}

INPUT CreateKey(unsigned char virtualKey) noexcept {
	INPUT key { INPUT_KEYBOARD };
	key.ki.wVk = virtualKey;
	key.ki.wScan = static_cast<WORD>(MapVirtualKeyW(virtualKey, MAPVK_VK_TO_VSC));
	key.ki.dwFlags = IsExtendedKey(virtualKey) ? KEYEVENTF_EXTENDEDKEY : 0;
	key.ki.time = 0;
	key.ki.dwExtraInfo = 0;
	return key;
}

template<
	typename Itr,
	typename Itr2,
	typename std::enable_if_t<std::is_same_v<typename Itr::value_type, INPUT>, std::nullptr_t> = nullptr,
	typename std::enable_if_t<std::is_same_v<typename Itr2::value_type, INPUT>, std::nullptr_t> = nullptr>
constexpr void AddAllKeysUp(const Itr first, const Itr last, Itr2 first2, Itr2 last2) {
	Itr itr = first;
	Itr2 itr2 = first2;
	for (; itr != last && itr2 != last2; ++itr, ++itr2) {
		*itr2 = *itr;
		itr2->ki.dwFlags = KEYEVENTF_KEYUP | itr->ki.dwFlags;
	}
}

template<typename Container>
constexpr void AddAllKeysUp(Container& c) {
	AddAllKeysUp(c.begin(), c.begin() + (c.size() >> 1), c.rbegin(), c.rbegin() + (c.size() >> 1));
}

TEST(KeyListener, ExitApp) {
	std::unique_ptr<KeysListenerService> service = std::make_unique<KeysListenerService>();
	reps::listener_t listener([](reps::bag_t const& value) {
		kbevent_t kbe = reps::data<kbevent_t>(value);
		EXPECT_EQ(kbe, kbe_exit);
		SUCCEED() << "Receive \"kbe_exit\".";
	});
	service->addObserver(listener);
	service->initialize();

	// Send key
	std::array<INPUT, 6> keys;
	keys[0] = CreateKey(VK_LWIN);
	keys[1] = CreateKey(VK_LCONTROL);
	keys[2] = CreateKey('X');
	AddAllKeysUp(keys);
	SendInput(keys.size(), keys.data(), sizeof(INPUT));
}

TEST(KeyListener, DuplicateKey) {
	std::unique_ptr<KeysListenerService> service = std::make_unique<KeysListenerService>();
	reps::listener_t listener([](reps::bag_t const& value) {
		static int i = 0;

		kbevent_t kbe = reps::data<kbevent_t>(value);
		EXPECT_EQ(kbe, kbe_exit);
		EXPECT_EQ(i++, 0);
		SUCCEED() << "Receive \"kbe_exit\".";
	});
	service->addObserver(listener);
	service->initialize();

	// Send key
	std::array<INPUT, 8> keys; // Win, Ctrl, X, X, X, (key up)
	keys[0] = CreateKey(VK_LWIN);
	keys[1] = CreateKey(VK_LCONTROL);
	keys[2] = CreateKey('X');
	keys[3] = CreateKey('X');
	keys[4] = CreateKey('X');
	AddAllKeysUp(keys.cbegin(), keys.cbegin() + 3, keys.rbegin(), keys.rbegin() + 3);
	SendInput(keys.size(), keys.data(), sizeof(INPUT));
}

TEST(KeyListener, MoveLeft) {
	std::unique_ptr<KeysListenerService> service = std::make_unique<KeysListenerService>();
	reps::listener_t listener([](reps::bag_t const& value) {
		kbevent_t kbe = reps::data<kbevent_t>(value);
		EXPECT_EQ(kbe, kbe_move_window_and_switch_left);
		SUCCEED() << "Receive \"kbe_move_window_and_switch_left\".";
	});
	service->addObserver(listener);
	service->initialize();

	// Send key
	std::array<INPUT, 8> keys;
	keys[0] = CreateKey(VK_LWIN);
	keys[1] = CreateKey(VK_LCONTROL);
	keys[2] = CreateKey(VK_LMENU);
	keys[3] = CreateKey(VK_LEFT);
	AddAllKeysUp(keys);
	SendInput(keys.size(), keys.data(), sizeof(INPUT));
}
