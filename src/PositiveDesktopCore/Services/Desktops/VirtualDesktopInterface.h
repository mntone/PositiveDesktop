#pragma once

namespace app::desktop {

	struct IVirtualDesktopDelegate {
		virtual ~IVirtualDesktopDelegate() = default;

		virtual int Index() const noexcept = 0;
		virtual winrt::hstring Name() = 0;
	};

	struct IVirtualDesktopManagerInternalDelegate {
		virtual ~IVirtualDesktopManagerInternalDelegate() = default;

		// Delegates
		virtual HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept = 0;
		virtual HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept = 0;
		virtual HRESULT GetDesktops(IObjectArray** ppArray) const noexcept = 0;
		virtual HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept = 0;
		virtual HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept = 0;
		virtual HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept = 0;

		// Helpers
		virtual HRESULT LoadDesktops() noexcept = 0;
	};

	struct IVirtualDesktopNotificationListener {
		virtual ~IVirtualDesktopNotificationListener() = default;
	};

	struct IVirtualDesktopNotificationCallback {
		virtual ~IVirtualDesktopNotificationCallback() = default;

		virtual HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktopDelegate* pDesktop) noexcept = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktopDelegate* pDesktopDestroyed) noexcept = 0;
		virtual HRESULT __stdcall VirtualDesktopMoved(IVirtualDesktopDelegate* pDesktop, int nFromIndex) noexcept = 0;
		virtual HRESULT __stdcall VirtualDesktopNameChanged(IVirtualDesktopDelegate* pDesktop) noexcept = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktopDelegate* pDesktopNew) noexcept = 0;
	};

	static constexpr winrt::guid clsidImmersiveShell { 0xC2F03A33, 0x21F5, 0x47FA, { 0xB4,0xBB,0x15,0x63,0x62,0xA2,0xF2,0x39 } };
	static constexpr winrt::guid clsidVirtualDesktopManagerInternal { 0xC5E0CDCA, 0x7B6E, 0x41B2, { 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B } };
	static constexpr winrt::guid clsidVirtualNotificationService { 0xA501FDEC, 0x4A09, 0x464C, { 0xAE,0x4E,0x1B,0x9C,0x21,0xB8,0x49,0x18 } };

}
