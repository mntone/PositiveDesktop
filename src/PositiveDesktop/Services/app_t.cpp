#include "pch.h"
#include "app_t.h"

using namespace app;

template<typename T>
void release(T*& ptr) {
	T* ptr2 = std::exchange(ptr, nullptr);
	if (ptr2) {
		delete ptr2;
	}
}

app_t::app_t()
	: configManager_(nullptr)
	, presenter_(nullptr)
	, notificationListener_(nullptr) {
}

app_t::~app_t() noexcept {
	release(notificationListener_);
	release(presenter_);
	release(configManager_);
}

#include "Storages/WinRTConfigManager.h"
#include "UI/NotificationPresenterWinUI3.h"

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
	app::UI::NotificationPresenterHint hint = osver.dwBuildNumber >= 22000
		? app::UI::NotificationPresenterHint::Windows11 /* Build 22000- */
		: app::UI::NotificationPresenterHint::Windows10;
	presenter_ = CreateWinUI3NotificationPresenter(hint);

	// Init listener
	notificationListener_ = new listener::VirtualDesktopNotificationService(presenter_);
	notificationListener_->initialize(osver.dwBuildNumber);
}
