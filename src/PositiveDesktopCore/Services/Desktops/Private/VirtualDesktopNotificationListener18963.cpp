#include "pch.h"
#include "VirtualDesktopNotificationListener.h"

#include "VirtualDesktopDelegate.h"

using namespace app::desktop;

HRESULT VirtualDesktopNotificationListener18963::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	IVirtualDesktopDelegate* delegate { nullptr };
	IVirtualDesktop2* pDesktop2 { nullptr };
	HRESULT hr = pDesktop->QueryInterface(&pDesktop2);
	if (SUCCEEDED(hr)) {
		hr = cache_->CreateDelegateIfNeeded(pDesktop2, &delegate);
		if (SUCCEEDED(hr)) {
			hr = callback_->VirtualDesktopCreated(delegate);
		}
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener18963::VirtualDesktopDestroyBegin(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener18963::VirtualDesktopDestroyFailed(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener18963::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* /*pDesktopFallback*/) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->DetachDelegate(pDesktopDestroyed, &delegate);
	if (SUCCEEDED(hr)) {
		VirtualDesktopDelegate18963* native = reinterpret_cast<VirtualDesktopDelegate18963*>(delegate);
		native->ForceCache();
		native->DeletePointer();
		hr = callback_->VirtualDesktopDestroyed(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener18963::ViewVirtualDesktopChanged(IUnknown* pView) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener18963::CurrentVirtualDesktopChanged(IVirtualDesktop* /*pDesktopOld*/, IVirtualDesktop* pDesktopNew) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktopNew, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->CurrentVirtualDesktopChanged(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener18963::VirtualDesktopNameChanged(IVirtualDesktop2* pDesktop, HSTRING abiName) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		reinterpret_cast<VirtualDesktopDelegate18963*>(delegate)->Name(abiName);
		hr = callback_->VirtualDesktopNameChanged(delegate);
	}
	return hr;
}
