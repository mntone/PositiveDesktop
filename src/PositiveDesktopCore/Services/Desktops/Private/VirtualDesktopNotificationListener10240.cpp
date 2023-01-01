#include "pch.h"
#include "VirtualDesktopNotificationListener10240.h"

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
	HRESULT hr = cache_->FromInterface(pDesktopDestroyed, &delegate);
	if (SUCCEEDED(hr)) {
		delegate->DeletePointer();
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

HRESULT VirtualDesktopNotificationListener10240::VirtualDesktopRenamed(IVirtualDesktop2* pDesktop, HSTRING abiName) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		delegate->Name(abiName);
		hr = callback_->VirtualDesktopRenamed(delegate);
	}
	return hr;
}
