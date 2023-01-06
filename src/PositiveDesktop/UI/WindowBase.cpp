#include "pch.h"
#include "WindowBase.h"

namespace app::ui::nonlocalized {

	constexpr std::wstring_view PositiveDesktop_WindowBase_ClassPointer = L"PositiveDesktop.WindowBase.ClassPointer";

	constexpr std::string_view ErrorMessage_UnsetWndProc = "Failed to unset the wndproc.";
	constexpr std::string_view ErrorMessage_RemoveWndProcProp = "Failed to remove the wndproc prop.";

}

using namespace app::ui::nonlocalized;

using namespace winrt::PositiveDesktop::implementation;

void WindowBase::Subclass(HWND hWnd) {
	winrt::check_bool(SetPropW(hWnd, PositiveDesktop_WindowBase_ClassPointer.data(), this)); // Set class pointer
	LONG_PTR nextWndProc = SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowBase::WndProcStatic));
	check_bool(nextWndProc);
	nextWndProc_ = reinterpret_cast<WNDPROC>(nextWndProc);
}

void WindowBase::ReleaseSubclass(HWND hWnd) noexcept {
	LOG_TRACE_BEGIN(app::logger::ltg_presenter);

	WNDPROC nextWndProc = std::exchange(nextWndProc_, nullptr);
	CHECK_WARN_BOOL_PASS(
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(nextWndProc)),
		ErrorMessage_UnsetWndProc.data());
	CHECK_INFO_BOOL_PASS(
		RemovePropW(hWnd, PositiveDesktop_WindowBase_ClassPointer.data()),
		ErrorMessage_RemoveWndProcProp.data());

	LOG_TRACE_END_NOLABEL();
}

LRESULT WindowBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	return CallWindowProcW(nextWndProc_, hWnd, message, wParam, lParam);
}

LRESULT WindowBase::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	WindowBase* that = static_cast<WindowBase*>(GetPropW(hWnd, PositiveDesktop_WindowBase_ClassPointer.data()));
	if (that) {
		return that->WndProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}
