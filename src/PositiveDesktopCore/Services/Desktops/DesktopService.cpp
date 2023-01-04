#include "pch.h"
#include "DesktopService.h"

#include "Common/WindowUtil.h"

#include "Private/VirtualDesktopDelegate.h"
#include "Private/VirtualDesktopManagerInternalDelegate.h"
#include "Private/VirtualDesktopNotificationListener.h"

namespace app::desktop {

	enum vdtarget_t: int {
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
		vdt_first,
		vdt_last,
		vdt_left,
		vdt_right,
		vdt_new,
		vdt_previous,
	};

	constexpr std::wstring_view PositiveDesktop_Mark_Topmost = L"PositiveDesktop.Mark.Topmost";

}

using namespace winrt;

using namespace app::desktop;

DesktopService::DesktopService()
	:  serviceProvider_(nullptr)
	, applicationViewCollection_(nullptr)
	, virtualDesktopManagerDelegate_(nullptr)
	, virtualDesktopNotificationService_(nullptr)
	, cookie_(0)
	, virtualDesktopCache_(std::make_shared<VirtualDesktopCache>()) {
}

DesktopService::~DesktopService() {
	DWORD cookie = std::exchange(cookie_, 0);
	HRESULT hr = virtualDesktopNotificationService_->Unregister(cookie);
	if (FAILED(hr)) {
		// TODO: error log
	}

	clearObserver();
	unsetTopmostAll();
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

	if (build >= 22449) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate22449>(build, virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 21313) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate21313>(build, virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 20231) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate20231>(build, virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 20211) {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	} else if (build >= 18963) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate18963>(virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 14238) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate14238>(virtualDesktopCache_, serviceProvider_.get());
	} else {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}

	check_hresult(serviceProvider_->QueryService(
		clsidVirtualNotificationService,
		__uuidof(IVirtualDesktopNotificationService),
		virtualDesktopNotificationService_.put_void()));

	if (build >= 21359 /* Windows 10 Insider, Windows 11 */) {
		com_ptr<VirtualDesktopNotificationListener21359> listener = make_self<VirtualDesktopNotificationListener21359>(virtualDesktopCache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification21359>().get(), &cookie_));
	} else if (build >= 20231 /* Windows 10 Insider */) {
		com_ptr<VirtualDesktopNotificationListener20231> listener = make_self<VirtualDesktopNotificationListener20231>(virtualDesktopCache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification20241>().get(), &cookie_));
	} else if (build >= 20211) {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	} else if (build >= 18963 /* general Windows 10 */) {
		com_ptr<VirtualDesktopNotificationListener18963> listener = make_self<VirtualDesktopNotificationListener18963>(virtualDesktopCache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification2>().get(), &cookie_));
	} else if (build >= 10159 /* previous Windows 10 */) {
		com_ptr<VirtualDesktopNotificationListener10240> listener = make_self<VirtualDesktopNotificationListener10240>(virtualDesktopCache_, this);
		check_hresult(virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification>().get(), &cookie_));
	} else {
		throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}

	check_hresult(virtualDesktopManagerDelegate_->LoadDesktops());
}

#pragma region Operation implementation

HRESULT DesktopService::GetTargetDesktop(int target, IVirtualDesktopDelegate** ppDesktop) const noexcept {
	HRESULT hr = S_OK;
	switch (target) {
	case vdt_left:
	{
		IVirtualDesktopDelegate* currentDesktop { nullptr };
		hr = virtualDesktopManagerDelegate_->GetCurrentDesktop(&currentDesktop);
		if (SUCCEEDED(hr)) {
			hr = virtualDesktopManagerDelegate_->GetAdjacentDesktop(currentDesktop, AD_LEFT, ppDesktop);
		}
		break;
	}
	case vdt_right:
	{
		IVirtualDesktopDelegate* currentDesktop { nullptr };
		hr = virtualDesktopManagerDelegate_->GetCurrentDesktop(&currentDesktop);
		if (SUCCEEDED(hr)) {
			hr = virtualDesktopManagerDelegate_->GetAdjacentDesktop(currentDesktop, AD_RIGHT, ppDesktop);
		}
		break;
	}
	case vdt_first:
		hr = virtualDesktopCache_->First(ppDesktop);
		break;
	case vdt_last:
		hr = virtualDesktopCache_->Last(ppDesktop);
		break;
	case vdt_new:
		hr = virtualDesktopManagerDelegate_->CreateDesktop(ppDesktop);
		break;
	case vdt_previous:
		hr = E_NOTIMPL;
		break;
	default:
		WINRT_ASSERT(target <= 9);
		hr = virtualDesktopCache_->GetAt(target, ppDesktop);
		break;
	}
	return hr;
}

void DesktopService::unsetTopmostAll() noexcept {
	check_bool(EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
		HANDLE hTopmost = RemovePropW(hWnd, PositiveDesktop_Mark_Topmost.data());
		if (!hTopmost) {
			return TRUE;
		}
		BOOL topmost = reinterpret_cast<BOOL>(hTopmost);
		if (topmost) {
			UnsetTopmost(hWnd); // Discard return value
		}
		return TRUE;
	}, 0));
}

void DesktopService::setTopmostToForegroundWindow() noexcept try {
	HWND hWnd = GetForegroundWindow();
	LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	check_bool(exStyle);
	if (!(exStyle & WS_EX_TOPMOST)) {
		check_bool(SetTopmost(hWnd));
		check_bool(SetPropW(hWnd, PositiveDesktop_Mark_Topmost.data(), reinterpret_cast<HANDLE>(TRUE))); // MARK
	}
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::unsetTopmostToForegroundWindow() noexcept try {
	HWND hWnd = GetForegroundWindow();
	LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	check_bool(exStyle);
	if (exStyle & WS_EX_TOPMOST) {
		RemovePropW(hWnd, PositiveDesktop_Mark_Topmost.data()); // UNMARK; discard return value
		check_bool(UnsetTopmost(hWnd));
	}
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::toggleTopmostToForegroundWindow() noexcept try {
	HWND hWnd = GetForegroundWindow();
	LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	check_bool(exStyle);
	if (exStyle & WS_EX_TOPMOST) {
		RemovePropW(hWnd, PositiveDesktop_Mark_Topmost.data()); // UNMARK; discard return value
		check_bool(UnsetTopmost(hWnd));
	} else {
		check_bool(SetTopmost(hWnd));
		check_bool(SetPropW(hWnd, PositiveDesktop_Mark_Topmost.data(), reinterpret_cast<HANDLE>(TRUE))); // MARK
	}
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::switchDesktop(int target) noexcept try {
	IVirtualDesktopDelegate* targetDesktop { nullptr };
	check_hresult(GetTargetDesktop(target, &targetDesktop));
	check_hresult(virtualDesktopManagerDelegate_->SwitchDesktop(targetDesktop));
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::moveForegroundWindow(int target) noexcept try {
	IVirtualDesktopDelegate* targetDesktop { nullptr };
	check_hresult(GetTargetDesktop(target, &targetDesktop));

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()));
	check_hresult(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), targetDesktop));
} catch (winrt::hresult_error const& /*err*/) {
	// TODO: error log
}

void DesktopService::moveForegroundWindowAndSwitch(int target) noexcept try {
	IVirtualDesktopDelegate* targetDesktop { nullptr };
	check_hresult(GetTargetDesktop(target, &targetDesktop));

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
