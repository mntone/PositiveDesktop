#include "pch.h"
#include "Services/Services.h"

#include <atomic>
#include <vector>

#include "VirtualDesktopNotificationServiceWin10.h"
#include "VirtualDesktopNotificationServiceWin11.h"

// NotificationPresenter impl
#include "NotificationWindow.xaml.h"
using NotificationPresenter = UI::NotificationPresenter<winrt::PositiveDesktop::NotificationWindow>;

class VirtualDesktopNotificationService final: public reps::observer_t {
public:
	VirtualDesktopNotificationService();

	void close();

private:
	void FASTCALL showWindows(UI::NotificationPresenterData data) noexcept;

	void FASTCALL on(reps::bag_t const& value) noexcept override;

private:
	// Sink service
	std::unique_ptr<app::IVirtualDesktopNotificationServiceImpl> impl_;

	// Presenter
	NotificationPresenter presenter_;
};

VirtualDesktopNotificationService::VirtualDesktopNotificationService() {
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
		impl_ = std::make_unique<win11::VirtualDesktopNotificationServiceImpl>(*this);
	} else if (osver.dwBuildNumber < 20231 && osver.dwBuildNumber >= 9841 /* general Windows 10 */) {
		impl_ = std::make_unique<win10::VirtualDesktopNotificationServiceImpl>(*this);
	} else {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}
}

void VirtualDesktopNotificationService::close() {
	impl_->close();
	impl_.reset(nullptr);
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
			UI::NotificationPresenterData data {
				UI::NotificationPresenterType::Changed,
				ev.name,
			};
			showWindows(std::move(data));
			break;
		}
		break;
	}
	case reps::event_t::completed:
		break;
	}
}

void FASTCALL VirtualDesktopNotificationService::showWindows(UI::NotificationPresenterData data) noexcept {
	presenter_.sync([data, &presenter = presenter_]() mutable {
		presenter.show(std::move(data));
	});
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
