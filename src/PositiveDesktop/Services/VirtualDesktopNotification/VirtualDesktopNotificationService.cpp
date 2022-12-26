#include "pch.h"
#include "VirtualDesktopNotificationService.h"

#include "UI/NotificationPresenter.h"

using namespace app::listener;

VirtualDesktopNotificationService::VirtualDesktopNotificationService(app::ui::INotificationPresenter* presenter)
	: presenter_(presenter)
	, impl_(nullptr)
	, deleter_(nullptr) {
}

VirtualDesktopNotificationService::~VirtualDesktopNotificationService() {
	WINRT_ASSERT(deleter_);

	app::IVirtualDesktopNotificationServiceImpl* impl = std::exchange(impl_, nullptr);
	impl->close();
	deleter_(impl);
	delete this;
}

void VirtualDesktopNotificationService::initialize(uint32_t build) {
	// Load the service
	if (build >= 21313 /* Windows 10 Insider, Windows 11 */) {
		deleter_ = app::win11::ReleaseVirtualDesktopNotificationServiceImpl;
		impl_ = app::win11::CreateVirtualDesktopNotificationServiceImpl(build, *this);
	} else if (20231 > build && build >= 9841 /* general Windows 10 */) {
		deleter_ = app::win10::ReleaseVirtualDesktopNotificationServiceImpl;
		impl_ = app::win10::CreateVirtualDesktopNotificationServiceImpl(*this);
	} else {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}
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
			app::ui::NotificationPresenterData data {
				app::ui::NotificationPresenterType::Changed,
				ev.index,
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
