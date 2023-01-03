#include "pch.h"
#include "VirtualDesktopNotificationListener10240.h"

#include "VirtualDesktopDelegate.h"

using namespace app::desktop;

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = S_OK;
	if (nameEnabled_) {
		IVirtualDesktop2* pDesktop2 { nullptr };
		hr = pDesktop->QueryInterface(&pDesktop2);
		if (SUCCEEDED(hr)) {
			hr = cache_->CreateDelegateIfNeeded(pDesktop2, &delegate);
		}
	} else {
		hr = cache_->CreateDelegateIfNeeded(pDesktop, &delegate);
	}
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
		if (nameEnabled_) {
			VirtualDesktopDelegate19041* native = reinterpret_cast<VirtualDesktopDelegate19041*>(delegate);
			native->ForceCache();
			native->DeletePointer();
		} else {
			VirtualDesktopDelegate10240* native = reinterpret_cast<VirtualDesktopDelegate10240*>(delegate);
			native->DeletePointer();
		}
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

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopNameChanged(IVirtualDesktop2* pDesktop, HSTRING abiName) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		reinterpret_cast<VirtualDesktopDelegate19041*>(delegate)->Name(abiName);
		hr = callback_->VirtualDesktopNameChanged(delegate);
	}
	return hr;
}
