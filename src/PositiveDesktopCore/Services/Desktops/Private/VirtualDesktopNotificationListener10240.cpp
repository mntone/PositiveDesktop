#include "pch.h"
#include "VirtualDesktopNotificationListener.h"

#include "VirtualDesktopDelegate.h"

using namespace app::desktop;

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->CreateDelegateIfNeeded(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->VirtualDesktopCreated(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopDestroyBegin(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopDestroyFailed(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* /*pDesktopFallback*/) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->DetachDelegate(pDesktopDestroyed, &delegate);
	if (SUCCEEDED(hr)) {
		VirtualDesktopDelegate10240* native = reinterpret_cast<VirtualDesktopDelegate10240*>(delegate);
		native->DeletePointer();
		hr = callback_->VirtualDesktopDestroyed(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener10240::ViewVirtualDesktopChanged(IUnknown* /*pView*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener10240::CurrentVirtualDesktopChanged(IVirtualDesktop* /*pDesktopOld*/, IVirtualDesktop* pDesktopNew) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktopNew, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->CurrentVirtualDesktopChanged(delegate);
	}
	return hr;
}
