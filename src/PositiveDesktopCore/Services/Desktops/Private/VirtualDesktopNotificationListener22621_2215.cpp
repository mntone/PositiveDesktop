#include "pch.h"
#include "VirtualDesktopNotificationListener.h"

#include "VirtualDesktopDelegate.h"

using namespace app::desktop;

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopCreated(IObjectArray* /*pArray*/, IVirtualDesktop22621_2215* pDesktop) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->CreateDelegateIfNeeded(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->VirtualDesktopCreated(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopDestroyBegin(IObjectArray* /*pArray*/, IVirtualDesktop22621_2215* /*pDesktopDestroyed*/, IVirtualDesktop22621_2215* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopDestroyFailed(IObjectArray* /*pArray*/, IVirtualDesktop22621_2215* /*pDesktopDestroyed*/, IVirtualDesktop22621_2215* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopDestroyed(IObjectArray* /*pArray*/, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* /*pDesktopFallback*/) {
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

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopMoved(IObjectArray* /*pArray*/, IVirtualDesktop22621_2215* pDesktop, int nFromIndex, int nToIndex) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->MoveDelegate(pDesktop, nFromIndex, nToIndex, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->VirtualDesktopMoved(delegate, nFromIndex);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopNameChanged(IVirtualDesktop22621_2215* pDesktop, HSTRING abiName) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		reinterpret_cast<VirtualDesktopDelegate20231*>(delegate)->Name(abiName);
		hr = callback_->VirtualDesktopNameChanged(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener22621_2215::ViewVirtualDesktopChanged(IUnknown* /*pView*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener22621_2215::CurrentVirtualDesktopChanged(IObjectArray* /*pArray*/, IVirtualDesktop22621_2215* /*pDesktopOld*/, IVirtualDesktop22621_2215* pDesktopNew) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktopNew, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->CurrentVirtualDesktopChanged(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopWallpaperChanged(IVirtualDesktop22621_2215* /*pDesktop*/, HSTRING /*hPath*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener22621_2215::VirtualDesktopSwitched(IVirtualDesktop22621_2215** /*ppDesktop*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener22621_2215::RemoteVirtualDesktopConnected(IVirtualDesktop22621_2215** /*ppDesktop*/) {
	return S_OK;
}
