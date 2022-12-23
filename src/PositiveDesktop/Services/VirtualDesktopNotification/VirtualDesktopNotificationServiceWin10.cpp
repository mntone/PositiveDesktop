#include "pch.h"
#include "VirtualDesktopNotificationServiceWin10.h"

#include <atomic>
#include <vector>

using namespace app::win10;

#pragma region Support functions

inline winrt::hstring GetDesktopName(IVirtualDesktop2* pDesktop) noexcept {
	HSTRING abiName;
	HRESULT hr = pDesktop->GetDesktopName(&abiName);
	if (FAILED(hr)) {
		return hstring { };
	}

	winrt::hstring name;
	winrt::attach_abi(name, abiName);
	return name;
}

inline winrt::hstring GetDesktopName(IVirtualDesktop* pDesktop) noexcept {

	winrt::com_ptr<IVirtualDesktop2> pDesktop2;
	HRESULT hr = pDesktop->QueryInterface(pDesktop2.put());
	if (FAILED(hr)) {
		return winrt::hstring { };
	}

	WINRT_ASSERT(pDesktop2);
	return GetDesktopName(pDesktop2.get());
}

#pragma endregion

#pragma region Service implementation

app::IVirtualDesktopNotificationServiceImpl* app::win10::CreateVirtualDesktopNotificationServiceImpl(reps::observer_t& observer) {
	winrt::impl::com_ref<VirtualDesktopNotificationServiceWin10> impl = winrt::make_self<VirtualDesktopNotificationServiceWin10>(observer);
	return impl.detach();
}

void app::win10::ReleaseVirtualDesktopNotificationServiceImpl(IVirtualDesktopNotificationServiceImpl* impl) {
	reinterpret_cast<IUnknown*>(impl)->Release();
}

VirtualDesktopNotificationServiceWin10::VirtualDesktopNotificationServiceWin10(reps::observer_t& observer)
	: serviceProvider_(nullptr)
	, virtualDesktopNotificationService_(nullptr)
	, cookie_(0) {
	winrt::check_hresult(CoCreateInstance(
		clsidImmersiveShell,
		nullptr,
		CLSCTX_LOCAL_SERVER,
		winrt::guid_of<IServiceProvider>(),
		serviceProvider_.put_void()));

	winrt::check_hresult(serviceProvider_->QueryService(
		clsidVirtualNotificationService,
		__uuidof(IVirtualDesktopNotificationService),
		virtualDesktopNotificationService_.put_void()));

	subject_.addObserver(observer);

	winrt::com_ptr<app::win10::IVirtualDesktopNotification2> sink;
	winrt::check_hresult(QueryInterface(__uuidof(app::win10::IVirtualDesktopNotification2), sink.put_void()));
	winrt::check_hresult(virtualDesktopNotificationService_->Register(sink.get(), &cookie_));
}

void VirtualDesktopNotificationServiceWin10::close() {
	WINRT_ASSERT(cookie_);

	subject_.clearObserver();
	winrt::check_hresult(virtualDesktopNotificationService_->Unregister(cookie_));
	cookie_ = 0;
}

#pragma endregion

#pragma region Sink implementation

#include "vdevent_t.h"

HRESULT VirtualDesktopNotificationServiceWin10::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin10::VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin10::VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin10::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin10::ViewVirtualDesktopChanged(void* pView) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin10::CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) {
	vdevent_t data {
		vde_changed,
		-1,
		GetDesktopName(pDesktopNew),
	};
	reps::next(subject_, data);
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin10::VirtualDesktopRenamed(IVirtualDesktop* pDesktop, HSTRING hName) {
	winrt::hstring hstrName;
	winrt::copy_from_abi(hstrName, hName);

	vdevent_t data {
		vde_renamed,
		-1,
		hstrName,
	};
	reps::next(subject_, data);
	return S_OK;
}

#pragma endregion
