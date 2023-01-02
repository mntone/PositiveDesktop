#include "pch.h"
#include "VirtualDesktopNotificationListener20231.h"

#include "VirtualDesktopDelegate.h"

using namespace app::desktop;

HRESULT VirtualDesktopNotificationListener20231::VirtualDesktopCreated(IVirtualDesktop20231* pDesktop) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->CreateDelegateIfNeeded(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->VirtualDesktopCreated(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener20231::VirtualDesktopDestroyBegin(IVirtualDesktop20231* /*pDesktopDestroyed*/, IVirtualDesktop20231* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener20231::VirtualDesktopDestroyFailed(IVirtualDesktop20231* /*pDesktopDestroyed*/, IVirtualDesktop20231* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener20231::VirtualDesktopDestroyed(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* /*pDesktopFallback*/) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->DetachDelegate(pDesktopDestroyed, &delegate);
	if (SUCCEEDED(hr)) {
		VirtualDesktopDelegate20231* native = reinterpret_cast<VirtualDesktopDelegate20231*>(delegate);
		native->ForceCache();
		native->DeletePointer();
		hr = callback_->VirtualDesktopDestroyed(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener20231::VirtualDesktopIsPerMonitorChanged(BOOL /*bPerMonitor*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener20231::VirtualDesktopNameChanged(IVirtualDesktop20231* pDesktop, HSTRING abiName) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		delegate->Name(abiName);
		hr = callback_->VirtualDesktopNameChanged(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener20231::ViewVirtualDesktopChanged(IUnknown* /*pView*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener20231::CurrentVirtualDesktopChanged(IVirtualDesktop20231* /*pDesktopOld*/, IVirtualDesktop20231* pDesktopNew) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktopNew, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->CurrentVirtualDesktopChanged(delegate);
	}
	return hr;
}
