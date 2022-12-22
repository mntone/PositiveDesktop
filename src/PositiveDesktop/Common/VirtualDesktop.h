#pragma once

namespace win10 {

	// Supported OS:
	// - [10240]                      (1507) / Threshold 1
	// - [10586] November Update      (1511) / Threshold 2
	// - [14393] Aniversary Update    (1607) / Redstone 1
	// - [15063] Creators Update      (1703) / Redstone 2
	// - [16299] Fall Creators Update (1709) / Redstone 3
	// - [17134] April 2018 Update    (1803) / Redstone 4
	// - [17763] October 2018 Update  (1809) / Redstone 5 (This version supports WinUI 3!)
	// - [18362] May 2019 Update      (1903) / 19H1
	// - [18363] November 2019 Update (1909) / 19H2
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
	IVirtualDesktop : public IUnknown
	{
		virtual HRESULT __stdcall IsViewVisible(void* pView, int* pfVisible) = 0;
		virtual HRESULT __stdcall GetID(winrt::guid* pGuid) = 0;
	};

	// Supported OS: the same as win10::IVirtualDesktop
	MIDL_INTERFACE("C179334C-4295-40D3-BEA1-C654D965605A")
	IVirtualDesktopNotification : public IUnknown {
		virtual HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktop* pDesktop) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall ViewVirtualDesktopChanged(void* pView) = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) = 0;
	};

	// Supported OS: the same as win10::IVirtualDesktop
	MIDL_INTERFACE("0CD45E71-D927-4F15-8B0A-8FEF525337BF")
	IVirtualDesktopNotificationService : public IUnknown {
		virtual HRESULT __stdcall Register(IVirtualDesktopNotification* pNotification, DWORD* dwCookie) noexcept = 0;
		virtual HRESULT __stdcall Unregister(DWORD dwCookie) noexcept = 0;
	};

	// Add this in Windows 10 Version 2004
	// 
	// Supported OS:
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	MIDL_INTERFACE("31EBDE3F-6EC3-4CBD-B9FB-0EF6D09B41F4")
	IVirtualDesktop2 : public IVirtualDesktop
	{
	public:
		virtual HRESULT __stdcall GetDesktopName(HSTRING * pName) = 0;
	};

	// Supported OS: the same as win10::IVirtualDesktop2
	MIDL_INTERFACE("1BA7CF30-3591-43FA-ABFA-4AAF7ABEEDB7")
	IVirtualDesktopNotification2 : public IVirtualDesktopNotification {
		virtual HRESULT __stdcall VirtualDesktopRenamed(IVirtualDesktop* pDesktop, HSTRING name) = 0;
	};
}

namespace win11 {

	// Supported OS:
	// - [22000] Windows 11 Version 21H2
	// - [22621] Windows 11 Version 22H2
	MIDL_INTERFACE("536D3495-B208-4CC9-AE26-DE8111275BF8")
	IVirtualDesktop : public IUnknown
	{
	public:
		virtual HRESULT __stdcall IsViewVisible(void* pView, int* pfVisible) = 0;
		virtual HRESULT __stdcall GetID(winrt::guid* pGuid) = 0;
		virtual HRESULT __stdcall Unknown1(void* pUnknown) = 0;
		virtual HRESULT __stdcall GetDesktopName(HSTRING* pName) = 0;
		virtual HRESULT __stdcall GetDesktopWallpaper(HSTRING* pName) = 0;
	};

	// Supported OS: the same as win11::IVirtualDesktop
	MIDL_INTERFACE("CD403E52-DEED-4C13-B437-B98380F2B1E8")
	IVirtualDesktopNotification : public IUnknown {
		virtual HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktop* pDesktop) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall Unknown1(int nUnknown) = 0;
		virtual HRESULT __stdcall VirtualDesktopMoved(IVirtualDesktop* pDesktop, int nFromIndex, int nToIndex) = 0;
		virtual HRESULT __stdcall VirtualDesktopRenamed(IVirtualDesktop* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall ViewVirtualDesktopChanged(void* pView) = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) = 0;
		virtual HRESULT __stdcall VirtualDesktopWallpaperChanged(IVirtualDesktop* pDesktop, HSTRING path) = 0;
	};

	// Supported OS: the same as win11::IVirtualDesktop
	MIDL_INTERFACE("0CD45E71-D927-4F15-8B0A-8FEF525337BF")
	IVirtualDesktopNotificationService : public IUnknown {
		virtual HRESULT __stdcall Register(IVirtualDesktopNotification* pNotification, DWORD* dwCookie) noexcept = 0;
		virtual HRESULT __stdcall Unregister(DWORD dwCookie) noexcept = 0;
	};

}

//namespace winrt::impl {
	//template <> inline constexpr guid guid_v<IVirtualDesktopNotification>{ 0xC179334C, 0x4295, 0x40D3, { 0xBE,0xA1,0xC6,0x54,0xD9,0x65,0x60,0x5A } };
	//template <> inline constexpr guid guid_v<IVirtualDesktopNotification2>{ 0x1BA7CF30, 0x3591, 0x43FA, { 0xAB,0xFA,0x4A,0xAF,0x7A,0xBE,0xED,0xB7 } };
	//template <> inline constexpr guid guid_v<IVirtualDesktopNotificationService>{ 0x0CD45E71, 0xD927, 0x4F15, { 0x8B,0x0A,0x8F,0xEF,0x52,0x53,0x37,0xBF } };
//}
