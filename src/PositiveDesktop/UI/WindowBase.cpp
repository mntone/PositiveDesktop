#include "pch.h"
#include "WindowBase.h"

constexpr std::wstring_view PositiveDesktop_WindowBase_ClassPointer = L"PositiveDesktop.WindowBase.ClassPointer";

using namespace winrt::PositiveDesktop::implementation;

void WindowBase::Subclass(HWND hWnd) {
	winrt::check_bool(SetPropW(hWnd, PositiveDesktop_WindowBase_ClassPointer.data(), this)); // Set class pointer
	LONG_PTR nextWndProc = SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowBase::WndProcStatic));
	check_bool(nextWndProc);
	nextWndProc_ = reinterpret_cast<WNDPROC>(nextWndProc);
}

LRESULT WindowBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	if (WM_CLOSE == message) {
		WNDPROC nextWndProc = std::exchange(nextWndProc_, nullptr);
		LRESULT result = CallWindowProcW(nextWndProc, hWnd, message, wParam, lParam);
		if (!SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(nextWndProc))) {
			// TODO: Error message. Failed to unregister the own subclass.
		}
		RemovePropW(hWnd, PositiveDesktop_WindowBase_ClassPointer.data());
		return result;
	}
	return CallWindowProcW(nextWndProc_, hWnd, message, wParam, lParam);
}

LRESULT WindowBase::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	WindowBase* that = static_cast<WindowBase*>(GetPropW(hWnd, PositiveDesktop_WindowBase_ClassPointer.data()));
	if (that) {
		return that->WndProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}
