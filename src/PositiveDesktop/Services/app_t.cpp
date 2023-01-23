#include "pch.h"
#include "app_t.h"

#include "Common/RepsUtil.h"
#include "Common/VersionHelper.h"

#include "Services/Desktops/vdevent_t.h"
#include "Services/Storages/config_t.h"
#include "Services/Storages/WinRTConfigManager.h"

#include "UI/NotificationPresenterWinUI3.h"

using namespace app;

template<typename T>
void release(T*& ptr) {
	T* ptr2 = std::exchange(ptr, nullptr);
	if (ptr2) {
		delete ptr2;
	}
}

app_t::app_t()
	: config_(nullptr)
	, presenter_(nullptr)
	, keysLitener_(nullptr)
	, desktop_(nullptr) {
}

app_t::~app_t() noexcept {
	// Stop message service
	message_service_t::close();

	close();
}

void app_t::initialize() {
	// Retrieve OS Version
	VersionHelper::init();

	// Init config
	storage::IConfigManager* configManager = storage::CreateWinRTConfigManager();
	config_ = new storage::ConfigService(configManager);

	// Init presenter
	presenter_ = CreateWinUI3NotificationPresenter(config_->desktop());

	// Init key listener
	keysLitener_ = new keylistener::KeysListenerService();
	keysLitener_->addObserver(reps::basic_observer_t<app::keylistener::kbevent_t>::observer());
	keysLitener_->initialize();

	// Init desktop service
	desktop_ = new desktop::DesktopService();
	desktop_->addObserver(reps::basic_observer_t<app::desktop::vdevent_t>::observer());
	desktop_->initialize();

	// Start message service
	message_service_t::initialize();
}

void app_t::close() noexcept {
	// Release each service
	release(desktop_);
	release(keysLitener_);
	release(presenter_);
	release(config_);
}

void FASTCALL app_t::on(reps::bag_t<app::keylistener::kbevent_t> const& value) noexcept {
	if (value.hr < 0) {
		// error
		return;
	}

	switch (value.ev) {
	case reps::event_t::next:
	{
		message_service_t::send(value.data); // DO NOT PROCESS data on key listener thread.
		break;
	}
	case reps::event_t::completed:
		break;
	}
}

void FASTCALL app_t::on(reps::bag_t<app::desktop::vdevent_t> const& value) noexcept {
	if (value.hr < 0) {
		// error
		return;
	}

	switch (value.data.type) {
	case desktop::vde_changed:
		ui::NotificationPresenterData data {
			value.data.index,
			value.data.name,
		};
		presenter_->show(std::move(data));
		break;
	}
}

void app_t::process(keylistener::kbevent_t ev) noexcept {
	using namespace app::keylistener;

	switch (flag(ev)) {
	case kbe_switch:
	{
		int target = app::keylistener::desktop(ev);
		desktop_->switchDesktop(target);
		break;
	}
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
	case kbe_extend:
		switch (ev) {
		case kbe_exit:
			message_service_t::terminateWithoutLock();
			presenter_->closeAll();
			close();
			break;
		case kbe_settings:
			presenter_->showSettings();
			break;
		case kbe_topmost_toggle:
			desktop_->toggleTopmostToForegroundWindow();
			break;
		case kbe_topmost:
			desktop_->setTopmostToForegroundWindow();
			break;
		case kbe_topmost_remove:
			desktop_->unsetTopmostToForegroundWindow();
			break;
		}
		break;
	}
}
