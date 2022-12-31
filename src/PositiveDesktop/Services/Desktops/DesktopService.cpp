#include "pch.h"
#include "DesktopService.h"

#include "UI/NotificationPresenter.h"

using namespace app::desktop;

DesktopService::DesktopService(app::ui::INotificationPresenter* presenter)
	: presenter_(presenter)
	, impl_(nullptr)
	, deleter_(nullptr) {
}

DesktopService::~DesktopService() {
	WINRT_ASSERT(deleter_);

	app::IDesktopServiceImpl* impl = std::exchange(impl_, nullptr);
	impl->close();
	deleter_(impl);
}

void DesktopService::initialize(uint32_t build) {
	// Load the service
	if (build >= 21359 /* Windows 10 Insider, Windows 11 */) {
		deleter_ = app::win11::ReleaseDesktopServiceImpl;
		impl_ = app::win11::CreateDesktopServiceImpl(build, *this);
	} else if (20231 > build && build >= 9841 /* general Windows 10 */) {
		deleter_ = app::win10::ReleaseDesktopServiceImpl;
		impl_ = app::win10::CreateDesktopServiceImpl(*this);
	} else {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}
}

void DesktopService::moveWindowLeft() noexcept try {
	impl_->moveForegroundWindowToLeftOfCurrent();
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::moveWindowRight() noexcept try {
	impl_->moveForegroundWindowToRightOfCurrent();
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

#include "vdevent_t.h"

void FASTCALL DesktopService::on(reps::bag_t const& value) noexcept {
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
