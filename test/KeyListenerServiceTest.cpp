#include "pch.h"
#include "Services/KeyListeners/kbevent_t.h"
#include "Services/KeyListeners/KeyListenerService.h"

using namespace app::keylistener;

#include <array>
#include <functional>
#include <memory>

class listener_t final: public reps::observer_t {
public:
	listener_t(std::function<void(reps::bag_t const&)> callback) noexcept: callback_(callback) { }

private:
	void FASTCALL on(reps::bag_t const& value) noexcept override {
		callback_(value);
	}

private:
	std::function<void(reps::bag_t const&)> callback_;
};

constexpr bool IsExtendedKey(unsigned char virtualKey) {
	constexpr std::array<unsigned char, 9> extendedKey {
		VK_CONTROL,
		VK_RCONTROL,
		VK_MENU,
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

TEST(KeyListener, ExitApp) {
	std::unique_ptr<KeysListenerService> service = std::make_unique<KeysListenerService>();
	listener_t listener([](reps::bag_t const& value) {
		kbevent_t kbe = reps::data<kbevent_t>(value);
		EXPECT_EQ(kbe, kbe_exit);
	});
	service->addObserver(listener);
	service->initialize();

	// Send key
	std::array<INPUT, 3> keys;
	keys[0] = CreateKey(VK_LWIN);
	keys[1] = CreateKey(VK_CONTROL);
	keys[2] = CreateKey('X');
	SendInput(keys.size(), keys.data(), sizeof(INPUT));
}
