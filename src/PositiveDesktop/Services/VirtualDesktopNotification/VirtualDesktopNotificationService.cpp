#include "pch.h"
#include "Services/Services.h"

#include <atomic>
#include <vector>

#include "VirtualDesktopNotificationServiceImpl.h"

// NotificationPresenter impl
#include "UI/NotificationPresenter.h"
#include "UI/NotificationPresenterWinUI3.h"

class VirtualDesktopNotificationService final: public reps::observer_t {
public:
	VirtualDesktopNotificationService();

	void close();

private:
	void FASTCALL on(reps::bag_t const& value) noexcept override;

private:
	// Service
	app::IVirtualDesktopNotificationServiceImpl* impl_;
	void (*deleter_)(app::IVirtualDesktopNotificationServiceImpl*);

	// Presenter
	std::unique_ptr<app::UI::INotificationPresenter> presenter_;
};

using namespace app;

VirtualDesktopNotificationService::VirtualDesktopNotificationService()
	: impl_(nullptr)
	, deleter_(nullptr) {
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

	// Load the service
	if (osver.dwBuildNumber >= 21313 /* Windows 10 Insider, Windows 11 */) {
		// Init presenter
		presenter_.reset(CreateWinUI3NotificationPresenter(app::UI::NotificationPresenterHint::Windows11));

		deleter_ = app::win11::ReleaseVirtualDesktopNotificationServiceImpl;
		impl_ = app::win11::CreateVirtualDesktopNotificationServiceImpl(*this);
	} else if (osver.dwBuildNumber < 20231 && osver.dwBuildNumber >= 9841 /* general Windows 10 */) {
		// Init presenter
		presenter_.reset(CreateWinUI3NotificationPresenter(app::UI::NotificationPresenterHint::Windows10));

		deleter_ = app::win10::ReleaseVirtualDesktopNotificationServiceImpl;
		impl_ = app::win10::CreateVirtualDesktopNotificationServiceImpl(*this);
	} else {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}
}

void VirtualDesktopNotificationService::close() {
	WINRT_ASSERT(deleter_);

	app::IVirtualDesktopNotificationServiceImpl* impl = std::exchange(impl_, nullptr);
	impl->close();
	deleter_(impl);
	delete this;
}

#include "vdevent_t.h"

void FASTCALL VirtualDesktopNotificationService::on(reps::bag_t const& value) noexcept {
	if (value.hr < 0) {
		// error
		return;
	}

	switch (value.ev) {
	case reps::event_t::next:
	{
		vdevent_t ev = reps::data<vdevent_t>(value);
		switch (ev.type) {
		case vde_changed:
			app::UI::NotificationPresenterData data {
				app::UI::NotificationPresenterType::Changed,
				ev.name,
			};
			presenter_->show(std::move(data));
			break;
		}
		break;
	}
	case reps::event_t::completed:
		break;
	}
}

struct __VirtualDesktopNotificationServiceWrapperImpl final: ServiceWrapper {
	VirtualDesktopNotificationService service_;

	void close() noexcept try {
		service_.close();
	} catch (winrt::hresult_error const& /*err*/) {
		// TODO: error log
	}
};

service_t* CreateVirtualDesktopNotificationService() noexcept try {
	return new __VirtualDesktopNotificationServiceWrapperImpl();
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
	return nullptr;
}
