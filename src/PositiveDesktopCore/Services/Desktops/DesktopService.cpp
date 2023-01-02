#include "pch.h"
#include "DesktopService.h"

#include "Private/VirtualDesktopDelegate.h"
#include "Private/VirtualDesktopManagerInternalDelegate.h"
#include "Private/VirtualDesktopNotificationListener10240.h"
#include "Private/VirtualDesktopNotificationListener20231.h"
#include "Private/VirtualDesktopNotificationListener21359.h"

namespace app::desktop {

	enum vdtarget_t: int {
		vdt_first,
		vdt_desktop1,
		vdt_desktop2,
		vdt_desktop3,
		vdt_desktop4,
		vdt_desktop5,
		vdt_desktop6,
		vdt_desktop7,
		vdt_desktop8,
		vdt_desktop9,
		vdt_desktop10,
		vdt_last,
		vdt_left,
		vdt_right,
		vdt_new,
		vdt_previous,
	};

}

using namespace winrt;

using namespace app::desktop;

DesktopService::DesktopService()
	: listener_(nullptr)
	, serviceProvider_(nullptr)
	, applicationViewCollection_(nullptr)
	, virtualDesktopManagerDelegate_(nullptr)
	, virtualDesktopNotificationService_(nullptr)
	, cookie_(0)
	, cache_(std::make_shared<VirtualDesktopCache>()) {
}

DesktopService::~DesktopService() {
	clearObserver();

	IVirtualDesktopNotificationListener* listener = std::exchange(listener_, nullptr);
	if (listener) {
		winrt::com_ptr<IUnknown> unknown { listener, take_ownership_from_abi };
	}
}

void DesktopService::initialize(uint32_t build) {
	check_hresult(CoCreateInstance(
		clsidImmersiveShell,
		nullptr,
		CLSCTX_LOCAL_SERVER,
		guid_of<IServiceProvider>(),
		serviceProvider_.put_void()));

	check_hresult(serviceProvider_->QueryService(
		__uuidof(IApplicationViewCollection),
		__uuidof(IApplicationViewCollection),
		applicationViewCollection_.put_void()));

	bool nameEnabled = true;
	if (build >= 22449) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate22449>(build, cache_, serviceProvider_.get());
	} else if (build >= 21313) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate21313>(build, cache_, serviceProvider_.get());
	} else if (build >= 20231) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate20231>(build, cache_, serviceProvider_.get());
	} else if (build >= 20211) {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	} else if (build >= 14238) try {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate19041>(cache_, serviceProvider_.get());
	} catch (winrt::hresult_error const& /*err*/) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate14238>(cache_, serviceProvider_.get());
		nameEnabled = false;
	} else {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}

	check_hresult(serviceProvider_->QueryService(
		clsidVirtualNotificationService,
		__uuidof(IVirtualDesktopNotificationService),
		virtualDesktopNotificationService_.put_void()));

	if (build >= 21359 /* Windows 10 Insider, Windows 11 */) {
		com_ptr<VirtualDesktopNotificationListener21359> listener = make_self<VirtualDesktopNotificationListener21359>(cache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification21359>().get(), &cookie_));
		listener_ = listener.detach();
	} else if (build >= 20231 /* Windows 10 Insider */) {
		com_ptr<VirtualDesktopNotificationListener20231> listener = make_self<VirtualDesktopNotificationListener20231>(cache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification20241>().get(), &cookie_));
		listener_ = listener.detach();
	} else if (build >= 20211) {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	} else if (build >= 10159 /* general Windows 10 */) {
		com_ptr<VirtualDesktopNotificationListener10240> listener = make_self<VirtualDesktopNotificationListener10240>(nameEnabled, cache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification2>().get(), &cookie_));
		listener_ = listener.detach();
	} else {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}

	check_hresult(virtualDesktopManagerDelegate_->LoadDesktops());
}

#pragma region Operation implementation

void DesktopService::moveForegroundWindow(int target) noexcept try {
	IVirtualDesktopDelegate* currentDesktop;
	check_hresult(virtualDesktopManagerDelegate_->GetCurrentDesktop(&currentDesktop));

	IVirtualDesktopDelegate* targetDesktop;
	switch (target) {
	case vdt_left:
		check_hresult(virtualDesktopManagerDelegate_->GetAdjacentDesktop(currentDesktop, AD_LEFT, &targetDesktop));
		break;
	case vdt_right:
		check_hresult(virtualDesktopManagerDelegate_->GetAdjacentDesktop(currentDesktop, AD_RIGHT, &targetDesktop));
		break;
	case vdt_new:
		check_hresult(virtualDesktopManagerDelegate_->CreateDesktop(&targetDesktop));
		break;
	default:
		throw hresult_not_implemented();
	}

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()));
	check_hresult(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), targetDesktop));
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::moveForegroundWindowAndSwitch(int target) noexcept try {
	IVirtualDesktopDelegate* currentDesktop;
	check_hresult(virtualDesktopManagerDelegate_->GetCurrentDesktop(&currentDesktop));

	IVirtualDesktopDelegate* targetDesktop;
	switch (target) {
	case vdt_left:
		check_hresult(virtualDesktopManagerDelegate_->GetAdjacentDesktop(currentDesktop, AD_LEFT, &targetDesktop));
		break;
	case vdt_right:
		check_hresult(virtualDesktopManagerDelegate_->GetAdjacentDesktop(currentDesktop, AD_RIGHT, &targetDesktop));
		break;
	case vdt_new:
		check_hresult(virtualDesktopManagerDelegate_->CreateDesktop(&targetDesktop));
		break;
	default:
		throw hresult_not_implemented();
	}

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()));
	check_hresult(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), targetDesktop));
	check_hresult(virtualDesktopManagerDelegate_->SwitchDesktop(targetDesktop));
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

#pragma endregion

#pragma region Callback implementation

HRESULT DesktopService::VirtualDesktopCreated(IVirtualDesktopDelegate* pDesktop) noexcept try {
	vdevent_t data {
		vde_created,
		0,
		pDesktop->Index(),
		L"",
	};
	next(std::move(data));
	return S_OK;
} catch (winrt::hresult_error const& err) {
	// TODO: error log
	return err.code();
}

HRESULT DesktopService::VirtualDesktopDestroyed(IVirtualDesktopDelegate* pDesktopDestroyed) noexcept try {
	vdevent_t data {
		vde_removed,
		0,
		pDesktopDestroyed->Index(),
		pDesktopDestroyed->Name(),
	};

	delete pDesktopDestroyed;

	next(std::move(data));
	return S_OK;
} catch (winrt::hresult_error const& err) {
	// TODO: error log
	return err.code();
}

HRESULT DesktopService::VirtualDesktopMoved(IVirtualDesktopDelegate* pDesktop, int nFromIndex) noexcept try {
	if (nFromIndex > 0x1FFFFFFF) {
		// TODO: error log
		return E_INVALIDARG;
	}

	vdevent_t data {
		vde_moved,
		nFromIndex,
		pDesktop->Index(),
		pDesktop->Name(),
	};
	next(std::move(data));
	return S_OK;
} catch (winrt::hresult_error const& err) {
	// TODO: error log
	return err.code();
}

HRESULT DesktopService::VirtualDesktopNameChanged(IVirtualDesktopDelegate* pDesktop) noexcept try {
	vdevent_t data {
		vde_renamed,
		0,
		pDesktop->Index(),
		pDesktop->Name(),
	};
	next(std::move(data));
	return S_OK;
} catch (winrt::hresult_error const& err) {
	// TODO: error log
	return err.code();
}

HRESULT DesktopService::CurrentVirtualDesktopChanged(IVirtualDesktopDelegate* pDesktopNew) noexcept try {
	vdevent_t data {
		vde_changed,
		0,
		pDesktopNew->Index(),
		pDesktopNew->Name(),
	};
	next(std::move(data));
	return S_OK;
} catch (winrt::hresult_error const& err) {
	// TODO: error log
	return err.code();
}

#pragma endregion
