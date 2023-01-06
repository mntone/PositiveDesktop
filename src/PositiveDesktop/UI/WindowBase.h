#pragma once

namespace winrt::PositiveDesktop::implementation {

	struct WindowBase {
	protected:
		void Subclass(HWND hWnd);
		void ReleaseSubclass(HWND hWnd) noexcept;
		virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		static LRESULT WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		WNDPROC nextWndProc_;
	};

}
