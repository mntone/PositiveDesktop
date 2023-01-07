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

	namespace nonlocalized {

		constexpr std::string_view ErrorMessage_InitIServiceProvider = "Failed to init IServiceProvider.";
		constexpr std::string_view ErrorMessage_InitIApplicationViewCollection = "Failed to init IApplicationViewCollection.";
		constexpr std::string_view ErrorMessage_InitIVirtualDesktopNotificationService = "Failed to init IVirtualDesktopNotificationService.";
		constexpr std::string_view ErrorMessage_NotSupported = "This Windows build isn't supported.";

		constexpr std::string_view ErrorMessage_GetExstyle = "Failed to get exstyle.";
		constexpr std::string_view ErrorMessage_SetTopmost = "Failed to set topmost.";
		constexpr std::string_view ErrorMessage_UnsetTopmost = "Failed to unset topmost.";
		constexpr std::string_view ErrorMessage_SetTopmostProp = "Failed to set topmost prop.";
		constexpr std::string_view ErrorMessage_RemoveTopmostProp = "Failed to remove topmost prop.";

		constexpr std::string_view ErrorMessage_LoadDesktops = "Failed to load desktops.";
		constexpr std::string_view ErrorMessage_GetTargetDesktop = "Failed to get a target desktop.";
		constexpr std::string_view ErrorMessage_GetForegroundView = "Failed to get the foreground view.";
		constexpr std::string_view ErrorMessage_SwitchTargetDesktop = "Failed to switch the target desktop.";
		constexpr std::string_view ErrorMessage_MoveViewToTargetDesktop = "Failed to move the view to the target desktop.";

		constexpr std::string_view ErrorMessage_GetName = "Failed to get desktop name.";

	}

}

using namespace winrt;

using namespace app::desktop;

DesktopService::DesktopService()
	: serviceProvider_(nullptr)
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
	LOG_TRACE_BEGIN_INTHROW(logger::ltg_desktop);

	CHECK_FATAL_HRESULT_THROW(CoCreateInstance(
		clsidImmersiveShell,
		nullptr,
		CLSCTX_LOCAL_SERVER,
		guid_of<IServiceProvider>(),
		serviceProvider_.put_void()),
		nonlocalized::ErrorMessage_InitIServiceProvider);

	CHECK_FATAL_HRESULT_THROW(serviceProvider_->QueryService(
		__uuidof(IApplicationViewCollection),
		__uuidof(IApplicationViewCollection),
		applicationViewCollection_.put_void()),
		nonlocalized::ErrorMessage_InitIApplicationViewCollection);

	if (build >= 22449) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate22449>(build, virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 21313) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate21313>(build, virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 20231) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate20231>(build, virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 20211) {
		CHECK_FATAL_HRESULT_THROW(0x80131515 /*COR_E_NOTSUPPORTED*/, nonlocalized::ErrorMessage_NotSupported);
	} else if (build >= 18963) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate18963>(virtualDesktopCache_, serviceProvider_.get());
	} else if (build >= 14238) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate14238>(virtualDesktopCache_, serviceProvider_.get());
	} else {
		CHECK_FATAL_HRESULT_THROW(0x80131515 /*COR_E_NOTSUPPORTED*/, nonlocalized::ErrorMessage_NotSupported);
	}

	CHECK_FATAL_HRESULT_THROW(serviceProvider_->QueryService(
		clsidVirtualNotificationService,
		__uuidof(IVirtualDesktopNotificationService),
		virtualDesktopNotificationService_.put_void()),
		nonlocalized::ErrorMessage_InitIVirtualDesktopNotificationService);

	if (build >= 21359 /* Windows 10 Insider, Windows 11 */) {
		com_ptr<VirtualDesktopNotificationListener21359> listener = make_self<VirtualDesktopNotificationListener21359>(virtualDesktopCache_, this);
		CHECK_FATAL_HRESULT_THROW(
			virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification21359>().get(), &cookie_),
			"Failed to register IVirtualDesktopNotification21359.");
	} else if (build >= 20231 /* Windows 10 Insider */) {
		com_ptr<VirtualDesktopNotificationListener20231> listener = make_self<VirtualDesktopNotificationListener20231>(virtualDesktopCache_, this);
		CHECK_FATAL_HRESULT_THROW(
			virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification20241>().get(), &cookie_),
			"Failed to register IVirtualDesktopNotification20241.");
	} else if (build >= 20211) {
		CHECK_FATAL_HRESULT_THROW(0x80131515 /*COR_E_NOTSUPPORTED*/, nonlocalized::ErrorMessage_NotSupported);
	} else if (build >= 18963 /* general Windows 10 */) {
		com_ptr<VirtualDesktopNotificationListener18963> listener = make_self<VirtualDesktopNotificationListener18963>(virtualDesktopCache_, this);
		CHECK_FATAL_HRESULT_THROW(
			virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification2>().get(), &cookie_),
			"Failed to register IVirtualDesktopNotification2.");
	} else if (build >= 10159 /* previous Windows 10 */) {
		com_ptr<VirtualDesktopNotificationListener10240> listener = make_self<VirtualDesktopNotificationListener10240>(virtualDesktopCache_, this);
		CHECK_FATAL_HRESULT_THROW(
			virtualDesktopNotificationService_->Register(listener.as<IVirtualDesktopNotification>().get(), &cookie_),
			"Failed to register IVirtualDesktopNotification.");
	} else {
		CHECK_FATAL_HRESULT_THROW(0x80131515 /*COR_E_NOTSUPPORTED*/, nonlocalized::ErrorMessage_NotSupported);
	}

	CHECK_FATAL_HRESULT_THROW(virtualDesktopManagerDelegate_->LoadDesktops(), nonlocalized::ErrorMessage_LoadDesktops);

	LOG_TRACE_END_INTHROW();
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
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	check_bool(EnumWindows([](HWND hWnd, LPARAM /*lParam*/) -> BOOL {
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

	LOG_TRACE_END_NOLABEL();
}

void DesktopService::setTopmostToForegroundWindow() noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	HWND hWnd = GetForegroundWindow();
	LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	CHECK_ERROR_BOOL_GOTO(exStyle, nonlocalized::ErrorMessage_GetExstyle);

	if (!(exStyle & WS_EX_TOPMOST)) {
		CHECK_ERROR_BOOL_GOTO(SetTopmost(hWnd), nonlocalized::ErrorMessage_SetTopmost);
		CHECK_WARN_BOOL_PASS(SetPropW(hWnd, PositiveDesktop_Mark_Topmost.data(), reinterpret_cast<HANDLE>(TRUE)), nonlocalized::ErrorMessage_SetTopmostProp); // MARK
	}

	LOG_TRACE_END();
}

void DesktopService::unsetTopmostToForegroundWindow() noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	HWND hWnd = GetForegroundWindow();
	LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	CHECK_ERROR_BOOL_GOTO(exStyle, nonlocalized::ErrorMessage_GetExstyle);

	if (exStyle & WS_EX_TOPMOST) {
		CHECK_WARN_BOOL_PASS(RemovePropW(hWnd, PositiveDesktop_Mark_Topmost.data()), nonlocalized::ErrorMessage_RemoveTopmostProp); // UNMARK; discard return value
		CHECK_ERROR_BOOL_PASS(UnsetTopmost(hWnd), nonlocalized::ErrorMessage_UnsetTopmost);
	}

	LOG_TRACE_END();
}

void DesktopService::toggleTopmostToForegroundWindow() noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	HWND hWnd = GetForegroundWindow();
	LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	CHECK_ERROR_BOOL_GOTO(exStyle, nonlocalized::ErrorMessage_GetExstyle);

	if (exStyle & WS_EX_TOPMOST) {
		CHECK_WARN_BOOL_PASS(RemovePropW(hWnd, PositiveDesktop_Mark_Topmost.data()), nonlocalized::ErrorMessage_RemoveTopmostProp); // UNMARK; discard return value
		CHECK_ERROR_BOOL_PASS(UnsetTopmost(hWnd), nonlocalized::ErrorMessage_UnsetTopmost);
	} else {
		CHECK_ERROR_BOOL_GOTO(SetTopmost(hWnd), nonlocalized::ErrorMessage_SetTopmost);
		CHECK_WARN_BOOL_PASS(SetPropW(hWnd, PositiveDesktop_Mark_Topmost.data(), reinterpret_cast<HANDLE>(TRUE)), nonlocalized::ErrorMessage_SetTopmostProp); // MARK
	}

	LOG_TRACE_END();
}

void DesktopService::switchDesktop(int target) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	IVirtualDesktopDelegate* targetDesktop { nullptr };
	CHECK_ERROR_HRESULT_GOTO(GetTargetDesktop(target, &targetDesktop), nonlocalized::ErrorMessage_GetTargetDesktop);
	CHECK_ERROR_HRESULT_PASS(virtualDesktopManagerDelegate_->SwitchDesktop(targetDesktop), nonlocalized::ErrorMessage_SwitchTargetDesktop);

	LOG_TRACE_END();
}

void DesktopService::moveForegroundWindow(int target) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	IVirtualDesktopDelegate* targetDesktop { nullptr };
	com_ptr<IUnknown> view;
	CHECK_ERROR_HRESULT_GOTO(GetTargetDesktop(target, &targetDesktop), nonlocalized::ErrorMessage_GetTargetDesktop);
	CHECK_ERROR_HRESULT_GOTO(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()), nonlocalized::ErrorMessage_GetForegroundView);
	CHECK_ERROR_HRESULT_PASS(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), targetDesktop), nonlocalized::ErrorMessage_MoveViewToTargetDesktop);

	LOG_TRACE_END();
}

void DesktopService::moveForegroundWindowAndSwitch(int target) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop);

	IVirtualDesktopDelegate* targetDesktop { nullptr };
	com_ptr<IUnknown> view;
	CHECK_ERROR_HRESULT_GOTO(GetTargetDesktop(target, &targetDesktop), nonlocalized::ErrorMessage_GetTargetDesktop);
	CHECK_ERROR_HRESULT_GOTO(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()), nonlocalized::ErrorMessage_GetForegroundView);
	CHECK_ERROR_HRESULT_GOTO(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), targetDesktop), nonlocalized::ErrorMessage_MoveViewToTargetDesktop);
	CHECK_ERROR_HRESULT_PASS(virtualDesktopManagerDelegate_->SwitchDesktop(targetDesktop), nonlocalized::ErrorMessage_SwitchTargetDesktop);

	LOG_TRACE_END();
}

#pragma endregion

#pragma region Callback implementation

HRESULT DesktopService::VirtualDesktopCreated(IVirtualDesktopDelegate* pDesktop) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop_listener);

	vdevent_t data {
		vde_created,
		0,
		pDesktop->Index(),
		L"",
	};
	next(std::move(data));

	LOG_TRACE_END_NOLABEL();
	return S_OK;
}

HRESULT DesktopService::VirtualDesktopDestroyed(IVirtualDesktopDelegate* pDesktopDestroyed) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop_listener);

	vdevent_t data {
		vde_removed,
		0,
		pDesktopDestroyed->Index(),
		pDesktopDestroyed->Name(), // NO TRY. The name is cached before destroying.
	};

	delete pDesktopDestroyed;

	LOG_DEBUG(std::format("Create a new desktop #{}.", data.index + 1));
	next(std::move(data));

	LOG_TRACE_END_NOLABEL();
	return S_OK;
}

HRESULT DesktopService::VirtualDesktopMoved(IVirtualDesktopDelegate* pDesktop, int nFromIndex) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop_listener);

	if (nFromIndex > 0x1FFFFFFF) {
		// TODO: error log
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;
	winrt::hstring name;
	try {
		name = pDesktop->Name();
	} catch (winrt::hresult_error const& err) {
		hr = err.code();
		LOG_WARN_HRESULT(hr, nonlocalized::ErrorMessage_GetName);
	}

	vdevent_t data {
		vde_moved,
		nFromIndex,
		pDesktop->Index(),
		name,
	};
	LOG_DEBUG(std::format("Move the desktop #{} ({}).", data.index + 1, winrt::to_string(data.name)));
	next(std::move(data));

	LOG_TRACE_END_NOLABEL();
	return hr;
}

HRESULT DesktopService::VirtualDesktopNameChanged(IVirtualDesktopDelegate* pDesktop) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop_listener);

	HRESULT hr = S_OK;
	winrt::hstring name;
	try {
		name = pDesktop->Name();
	} catch (winrt::hresult_error const& err) {
		hr = err.code();
		LOG_WARN_HRESULT(hr, nonlocalized::ErrorMessage_GetName);
	}

	vdevent_t data {
		vde_renamed,
		0,
		pDesktop->Index(),
		name,
	};
	LOG_DEBUG(std::format("Rename the desktop #{} ({}).", data.index + 1, winrt::to_string(data.name)));
	next(std::move(data));

	LOG_TRACE_END_NOLABEL();
	return hr;
}

HRESULT DesktopService::CurrentVirtualDesktopChanged(IVirtualDesktopDelegate* pDesktopNew) noexcept {
	LOG_TRACE_BEGIN(logger::ltg_desktop_listener);

	HRESULT hr = S_OK;
	winrt::hstring name;
	try {
		name = pDesktopNew->Name();
	} catch (winrt::hresult_error const& err) {
		hr = err.code();
		LOG_WARN_HRESULT(hr, nonlocalized::ErrorMessage_GetName);
	}

	vdevent_t data {
		vde_changed,
		0,
		pDesktopNew->Index(),
		name,
	};
	LOG_DEBUG(std::format("Change the desktop #{} ({}).", data.index + 1, winrt::to_string(data.name)));
	next(std::move(data));

	LOG_TRACE_END_NOLABEL();
	return hr;
}

#pragma endregion
