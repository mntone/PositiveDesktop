#pragma once
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Windowing.h>

inline winrt::Microsoft::UI::DisplayId GetPrimaryDisplayId() {
	HMONITOR hMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
	WINRT_ASSERT(hMonitor);
	winrt::Microsoft::UI::DisplayId displayId = winrt::Microsoft::UI::GetDisplayIdFromMonitor(hMonitor);
	return displayId;
}

template<typename TWindow>
inline HWND GetHwnd(TWindow window) {
	HWND hWnd { nullptr };
	winrt::impl::com_ref<IWindowNative> windowNative = window.as<IWindowNative>();
	winrt::check_hresult(windowNative->get_WindowHandle(&hWnd));
	return hWnd;
}

template<typename TWindow>
inline HWND GetNullableHwnd(TWindow window) noexcept {
	HWND hWnd { nullptr };
	winrt::impl::com_ref<IWindowNative> windowNative = window.try_as<IWindowNative>();
	if (windowNative) {
		windowNative->get_WindowHandle(&hWnd);
	}
	return hWnd;
}

inline winrt::Microsoft::UI::Windowing::AppWindow GetAppWindow(HWND hWnd) {
	winrt::Microsoft::UI::WindowId windowId = winrt::Microsoft::UI::GetWindowIdFromWindow(hWnd);
	winrt::Microsoft::UI::Windowing::AppWindow appWindow = winrt::Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
	return appWindow;
}

template<typename TWindow>
inline winrt::Microsoft::UI::Windowing::AppWindow GetAppWindow(TWindow window) {
	HWND hWnd { GetHwnd(window) };
	winrt::Microsoft::UI::WindowId windowId = winrt::Microsoft::UI::GetWindowIdFromWindow(hWnd);
	winrt::Microsoft::UI::Windowing::AppWindow appWindow = winrt::Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
	return appWindow;
}
