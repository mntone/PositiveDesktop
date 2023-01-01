#include "pch.h"
#include "app_t.h"

#include "Common/RepsUtil.h"

#include "Services/Desktops/vdevent_t.h"
#include "Services/Storages/WinRTConfigManager.h"

#include "UI/NotificationPresenterWinUI3.h"

using namespace app;

template<typename T>
void release(T*& ptr, typename std::enable_if_t<!std::is_base_of_v<reps::observable_t, T>, std::nullptr_t> = nullptr) {
	T* ptr2 = std::exchange(ptr, nullptr);
	if (ptr2) {
		delete ptr2;
	}
}

template<typename T, typename std::enable_if_t<std::is_base_of_v<reps::observable_t, T>, std::nullptr_t> = nullptr>
void release(T*& ptr) {
	T* ptr2 = std::exchange(ptr, nullptr);
	if (ptr2) {
		ptr2->clearObserver();
		delete ptr2;
	}
}

app_t::app_t()
	: configManager_(nullptr)
	, presenter_(nullptr)
	, keysLitener_(nullptr)
	, desktop_(nullptr) {
}

app_t::~app_t() noexcept {
	message_service_t::close();

	release(desktop_);

	keylistener::KeysListenerService* keysLitener = std::exchange(keysLitener_, nullptr);
	if (keysLitener) {
		keysLitener->clearObserver();
		delete keysLitener;
	}

	release(presenter_);
	release(configManager_);
}

void app_t::initialize() {
	// Retrieve OS Version
	OSVERSIONINFOW osver { sizeof(OSVERSIONINFOW) };
#pragma warning(push)
#pragma warning(disable: 4996)
	if (!GetVersionExW(&osver)) {
		winrt::throw_last_error();
	}
#pragma warning(pop)
	if (osver.dwMajorVersion != 10 || osver.dwMinorVersion != 0) {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}

	// Init config manager
	configManager_ = storage::CreateWinRTConfigManager();
	config_ = configManager_->Load();

	// Init presenter
	app::ui::NotificationPresenterHint hint = osver.dwBuildNumber >= 22000
		? app::ui::NotificationPresenterHint::Windows11 /* Build 22000- */
		: app::ui::NotificationPresenterHint::Windows10;
	presenter_ = CreateWinUI3NotificationPresenter(config_, hint);

	// Init key listener
	keysLitener_ = new keylistener::KeysListenerService();
	keysLitener_->addObserver(*this);
	keysLitener_->initialize();

	// Init desktop service
	desktop_ = new desktop::DesktopService();
	desktop_->addObserver(*new reps::dangerous_listener_t([this](reps::bag_t const& value) {
		if (value.hr < 0) {
			// error
			return;
		}

		desktop::vdevent_t ev = reps::data<desktop::vdevent_t>(value);
		switch (ev.type) {
		case desktop::vde_changed:
			ui::NotificationPresenterData data {
				ev.index,
				ev.name,
			};
			presenter_->show(std::move(data));
			break;
		}
	}));
	desktop_->initialize(osver.dwBuildNumber);

	// Start message service
	message_service_t::initialize();
}

void FASTCALL app_t::on(reps::bag_t const& value) noexcept {
	if (value.hr < 0) {
		// error
		return;
	}

	switch (value.ev) {
	case reps::event_t::next:
	{
		keylistener::kbevent_t const ev = reps::data<keylistener::kbevent_t>(value);
		message_service_t::send(ev); // DO NOT PROCESS data on key listener thread.
		break;
	}
	case reps::event_t::completed:
		break;
	}
}

void app_t::process(keylistener::kbevent_t ev) noexcept {
	using namespace app::keylistener;

	switch (flag(ev)) {
	case kbe_extend:
		switch (ev) {
		case kbe_exit:
			message_service_t::terminateWithoutLock();
			presenter_->closeAll();
			break;
		}
		break;
	case kbe_move_window:
	{
		int target = app::keylistener::desktop(ev);
		desktop_->moveForegroundWindow(target);
		break;
	}
	case kbe_move_window_and_switch:
	{
		int target = app::keylistener::desktop(ev);
		desktop_->moveForegroundWindowAndSwitch(target);
		break;
	}
	}
}
