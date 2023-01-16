#pragma once
#include <microsoft.ui.xaml.window.h>

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Windowing.h>

#include <Common/Math.h>

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

inline void SetCenter(HWND hWnd, winrt::Windows::Graphics::SizeInt32 expectedSize) {
	winrt::Microsoft::UI::Windowing::AppWindow appWindow = GetAppWindow(hWnd);
	winrt::Microsoft::UI::DisplayId primaryDisplayId = GetPrimaryDisplayId();
	winrt::Microsoft::UI::Windowing::DisplayArea displayArea = winrt::Microsoft::UI::Windowing::DisplayArea::GetFromDisplayId(primaryDisplayId);
	app::int32x4_t workArea = displayArea.WorkArea();
	if (expectedSize.Width > workArea.z) {
		expectedSize.Width = workArea.z - 24;
	}
	if (expectedSize.Height > workArea.w) {
		expectedSize.Height = workArea.w - 24;
	}

	app::int32x2_t pt = (workArea.size() - expectedSize) >> 1;
	appWindow.MoveAndResize(winrt::Windows::Graphics::RectInt32 {
		pt.x,
		pt.y,
		expectedSize.Width,
		expectedSize.Height,
	});
}
