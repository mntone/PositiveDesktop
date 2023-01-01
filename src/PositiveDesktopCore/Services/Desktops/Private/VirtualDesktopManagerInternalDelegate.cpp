#include "pch.h"
#include "VirtualDesktopManagerInternalDelegate.h"

using namespace app::desktop;

#pragma region LoadDesktops()

HRESULT VirtualDesktopManagerInternalDelegate14238::LoadDesktops() noexcept {
	HRESULT hr = S_OK;

	winrt::com_ptr<IObjectArray> desktops;
	hr = iface_->GetDesktops(desktops.put());
	if (FAILED(hr)) return hr;

	UINT count { 0 };
	hr = desktops->GetCount(&count);
	if (FAILED(hr)) return hr;

	for (UINT i = 0; i < count; ++i) {
		IVirtualDesktop* desktop;
		hr = desktops->GetAt(i, __uuidof(IVirtualDesktop), reinterpret_cast<void**>(&desktop));
		if (FAILED(hr)) return hr;

		IVirtualDesktopDelegate* delegate { nullptr };
		hr = cache_->CreateDelegate(desktop, static_cast<int>(i), &delegate);
		if (FAILED(hr)) {
			desktop->Release();
			return hr;
		}
	}
	return hr;
}

HRESULT VirtualDesktopManagerInternalDelegate19041::LoadDesktops() noexcept {
	HRESULT hr = S_OK;

	winrt::com_ptr<IObjectArray> desktops;
	hr = iface_->GetDesktops(desktops.put());
	if (FAILED(hr)) return hr;

	UINT count { 0 };
	hr = desktops->GetCount(&count);
	if (FAILED(hr)) return hr;

	for (UINT i = 0; i < count; ++i) {
		IVirtualDesktop2* desktop;
		hr = desktops->GetAt(i, __uuidof(IVirtualDesktop2), reinterpret_cast<void**>(&desktop));
		if (FAILED(hr)) return hr;

		IVirtualDesktopDelegate* delegate { nullptr };
		hr = cache_->CreateDelegate(desktop, static_cast<int>(i), &delegate);
		if (FAILED(hr)) {
			desktop->Release();
			return hr;
		}
	}
	return hr;
}

HRESULT VirtualDesktopManagerInternalDelegate20231::LoadDesktops() noexcept {
	HRESULT hr = S_OK;

	winrt::com_ptr<IObjectArray> desktops;
	hr = iface_->GetDesktops(nullptr, desktops.put());
	if (FAILED(hr)) return hr;

	UINT count { 0 };
	hr = desktops->GetCount(&count);
	if (FAILED(hr)) return hr;

	for (UINT i = 0; i < count; ++i) {
		IVirtualDesktop20231* desktop;
		hr = desktops->GetAt(i, __uuidof(IVirtualDesktop20231), reinterpret_cast<void**>(&desktop));
		if (FAILED(hr)) return hr;

		IVirtualDesktopDelegate* delegate { nullptr };
		hr = cache_->CreateDelegate(desktop, static_cast<int>(i), &delegate);
		if (FAILED(hr)) {
			desktop->Release();
			return hr;
		}
	}
	return hr;
}

HRESULT VirtualDesktopManagerInternalDelegate21313::LoadDesktops() noexcept {
	HRESULT hr = S_OK;

	winrt::com_ptr<IObjectArray> desktops;
	hr = iface_->GetDesktops(nullptr, desktops.put());
	if (FAILED(hr)) return hr;

	UINT count { 0 };
	hr = desktops->GetCount(&count);
	if (FAILED(hr)) return hr;

	for (UINT i = 0; i < count; ++i) {
		IVirtualDesktop21313* desktop;
		hr = desktops->GetAt(i, __uuidof(IVirtualDesktop21313), reinterpret_cast<void**>(&desktop));
		if (FAILED(hr)) return hr;

		IVirtualDesktopDelegate* delegate { nullptr };
		hr = cache_->CreateDelegate(desktop, static_cast<int>(i), &delegate);
		if (FAILED(hr)) {
			desktop->Release();
			return hr;
		}
	}
	return hr;
}

HRESULT VirtualDesktopManagerInternalDelegate22449::LoadDesktops() noexcept {
	HRESULT hr = S_OK;

	winrt::com_ptr<IObjectArray> desktops;
	hr = iface_->GetDesktops(nullptr, desktops.put());
	if (FAILED(hr)) return hr;

	UINT count { 0 };
	hr = desktops->GetCount(&count);
	if (FAILED(hr)) return hr;

	for (UINT i = 0; i < count; ++i) {
		IVirtualDesktop21313* desktop;
		hr = desktops->GetAt(i, __uuidof(IVirtualDesktop21313), reinterpret_cast<void**>(&desktop));
		if (FAILED(hr)) return hr;

		IVirtualDesktopDelegate* delegate { nullptr };
		hr = cache_->CreateDelegate(desktop, static_cast<int>(i), &delegate);
		if (FAILED(hr)) {
			desktop->Release();
			return hr;
		}
	}
	return hr;
}
