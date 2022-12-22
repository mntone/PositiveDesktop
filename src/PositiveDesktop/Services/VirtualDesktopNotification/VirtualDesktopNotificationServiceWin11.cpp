#include "pch.h"
#include "VirtualDesktopNotificationServiceWin11.h"

using namespace app;
using namespace win11;

#pragma region Service implementation

VirtualDesktopNotificationServiceImpl::VirtualDesktopNotificationServiceImpl(reps::observer_t& observer)
	: serviceProvider_(nullptr)
	, virtualDesktopNotificationService_(nullptr)
	, sink_(nullptr)
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

	winrt::com_ptr<VirtualDesktopNotificationSink> sink = winrt::make_self<VirtualDesktopNotificationSink>();
	winrt::check_hresult(virtualDesktopNotificationService_->Register(sink.as<IVirtualDesktopNotification>().get(), &cookie_));

	sink->addObserver(observer);
	sink_ = std::move(sink);
}

void VirtualDesktopNotificationServiceImpl::close() {
	WINRT_ASSERT(cookie_);

	sink_->clearObserver();
	winrt::check_hresult(virtualDesktopNotificationService_->Unregister(cookie_));
	sink_ = nullptr;
	cookie_ = 0;
}

#pragma endregion

#pragma region Sink implementation

#include "vdevent_t.h"

HRESULT VirtualDesktopNotificationSink::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::Unknown1(int nUnknown) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::VirtualDesktopMoved(IVirtualDesktop* pDesktop, int nFromIndex, int nToIndex) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::VirtualDesktopRenamed(IVirtualDesktop* pDesktop, HSTRING hName) {
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

HRESULT VirtualDesktopNotificationSink::ViewVirtualDesktopChanged(void* pView) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationSink::CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) {
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

HRESULT VirtualDesktopNotificationSink::VirtualDesktopWallpaperChanged(IVirtualDesktop* pDesktop, HSTRING hPath) {
	return S_OK;
}

void VirtualDesktopNotificationSink::addObserver(reps::observer_t& observer) noexcept {
	subject_.addObserver(observer);
}

void VirtualDesktopNotificationSink::clearObserver() noexcept {
	subject_.clearObserver();
}

#pragma endregion
