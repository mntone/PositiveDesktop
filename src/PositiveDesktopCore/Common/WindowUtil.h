#pragma once

inline BOOL SetTopmost(HWND hWnd) noexcept {
	return SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
}

inline BOOL UnsetTopmost(HWND hWnd) noexcept {
	return SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
}
