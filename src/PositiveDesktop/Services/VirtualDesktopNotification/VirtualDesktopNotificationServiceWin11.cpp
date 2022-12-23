#include "pch.h"
#include "VirtualDesktopNotificationServiceWin11.h"

#include <atomic>
#include <vector>

using namespace app::win11;

#pragma region Service implementation

app::IVirtualDesktopNotificationServiceImpl* app::win11::CreateVirtualDesktopNotificationServiceImpl(reps::observer_t& observer) {
	winrt::impl::com_ref<VirtualDesktopNotificationServiceWin11> impl = winrt::make_self<VirtualDesktopNotificationServiceWin11>(observer);
	return impl.detach();
}

void app::win11::ReleaseVirtualDesktopNotificationServiceImpl(IVirtualDesktopNotificationServiceImpl* impl) {
	reinterpret_cast<IUnknown*>(impl)->Release();
}

VirtualDesktopNotificationServiceWin11::VirtualDesktopNotificationServiceWin11(reps::observer_t& observer)
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

	winrt::com_ptr<app::win11::IVirtualDesktopNotification> sink;
	winrt::check_hresult(QueryInterface(__uuidof(app::win11::IVirtualDesktopNotification), sink.put_void()));
	winrt::check_hresult(virtualDesktopNotificationService_->Register(sink.get(), &cookie_));
}

void VirtualDesktopNotificationServiceWin11::close() {
	WINRT_ASSERT(cookie_);

	subject_.clearObserver();
	winrt::check_hresult(virtualDesktopNotificationService_->Unregister(cookie_));
	cookie_ = 0;
}

#pragma endregion

#pragma region Sink implementation

#include "vdevent_t.h"

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::Unknown1(int nUnknown) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopMoved(IVirtualDesktop* pDesktop, int nFromIndex, int nToIndex) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopRenamed(IVirtualDesktop* pDesktop, HSTRING hName) {
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

HRESULT VirtualDesktopNotificationServiceWin11::ViewVirtualDesktopChanged(void* pView) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) {
	winrt::hstring hstrName;
	{
		HSTRING hDesktopNewName;
		HRESULT hr = pDesktopNew->GetDesktopName(&hDesktopNewName);
		if (SUCCEEDED(hr)) {
			winrt::attach_abi(hstrName, hDesktopNewName);
		}
	}

	vdevent_t data {
		vde_changed,
		-1,
		hstrName,
	};
	reps::next(subject_, data);
	return S_OK;
}

HRESULT VirtualDesktopNotificationServiceWin11::VirtualDesktopWallpaperChanged(IVirtualDesktop* pDesktop, HSTRING hPath) {
	return S_OK;
}

#pragma endregion
