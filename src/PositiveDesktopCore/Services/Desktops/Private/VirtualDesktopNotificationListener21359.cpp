#include "pch.h"
#include "VirtualDesktopNotificationListener21359.h"

using namespace app::desktop;

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopCreated(IObjectArray* /*pArray*/, IVirtualDesktop20231* pDesktop) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->CreateDelegateIfNeeded(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->VirtualDesktopCreated(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopDestroyBegin(IObjectArray* /*pArray*/, IVirtualDesktop20231* /*pDesktopDestroyed*/, IVirtualDesktop20231* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopDestroyFailed(IObjectArray* /*pArray*/, IVirtualDesktop20231* /*pDesktopDestroyed*/, IVirtualDesktop20231* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopDestroyed(IObjectArray* /*pArray*/, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* /*pDesktopFallback*/) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktopDestroyed, &delegate);
	if (SUCCEEDED(hr)) {
		delegate->DeletePointer();
		hr = callback_->VirtualDesktopDestroyed(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener21359::Unknown1(int /*nUnknown*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopMoved(IObjectArray* /*pArray*/, IVirtualDesktop20231* pDesktop, int nFromIndex, int nToIndex) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->MoveDelegate(pDesktop, nFromIndex, nToIndex, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->VirtualDesktopMoved(delegate, nFromIndex);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopRenamed(IVirtualDesktop20231* pDesktop, HSTRING abiName) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktop, &delegate);
	if (SUCCEEDED(hr)) {
		delegate->Name(abiName);
		hr = callback_->VirtualDesktopRenamed(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener21359::ViewVirtualDesktopChanged(IUnknown* /*pView*/) {
	return S_OK;
}

HRESULT VirtualDesktopNotificationListener21359::CurrentVirtualDesktopChanged(IObjectArray* /*pArray*/, IVirtualDesktop20231* /*pDesktopOld*/, IVirtualDesktop20231* pDesktopNew) {
	IVirtualDesktopDelegate* delegate { nullptr };
	HRESULT hr = cache_->FromInterface(pDesktopNew, &delegate);
	if (SUCCEEDED(hr)) {
		hr = callback_->CurrentVirtualDesktopChanged(delegate);
	}
	return hr;
}

HRESULT VirtualDesktopNotificationListener21359::VirtualDesktopWallpaperChanged(IVirtualDesktop20231* /*pDesktop*/, HSTRING /*hPath*/) {
	return S_OK;
}
