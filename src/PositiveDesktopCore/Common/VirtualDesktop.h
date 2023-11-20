#pragma once
#include <ObjectArray.h>

namespace app::desktop {

	enum AdjacentDesktopDirection: int {
		AD_LEFT = 3,
		AD_RIGHT = 4,
	};

	// ===[ IApplicationViewCollection ]=============

	// Supported OS:
	// - [17763] October 2018 Update  (1809) / Redstone 5
	// - [18362] May 2019 Update      (1903) / 19H1
	// - [18363] November 2019 Update (1909) / 19H2
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	// - [22000] Windows 11 Version 21H2
	// - [22621] Windows 11 Version 22H2
	struct __declspec(uuid("1841C6D7-4F9D-42C0-AF41-8747538F10E5")) IApplicationViewCollection: public IUnknown {
		virtual HRESULT __stdcall GetViews(IObjectArray** ppViews) = 0;
		virtual HRESULT __stdcall GetViewsByZOrder(IObjectArray** ppViews) = 0;
		virtual HRESULT __stdcall GetViewsByAppUserModelId(LPCWSTR id, IObjectArray** ppViews) = 0;
		virtual HRESULT __stdcall GetViewForHwnd(HWND hWnd, /*IApplicationView*/ IUnknown** ppView) = 0;
		virtual HRESULT __stdcall GetViewForApplication(/*IImmersiveApplication*/ IUnknown* pImmersiveApplication, /*IApplicationView*/ IUnknown** ppView) = 0;
		virtual HRESULT __stdcall GetViewForAppUserModelId(LPCWSTR id, /*IApplicationView*/ IUnknown** ppView) = 0;
		virtual HRESULT __stdcall GetViewInFocus(/*IApplicationView*/ IUnknown** ppView) = 0;
		virtual HRESULT __stdcall Unknown1(/*IApplicationView*/ IUnknown** ppView) = 0;
		virtual HRESULT __stdcall RefreshCollection() = 0;
		virtual HRESULT __stdcall RegisterForApplicationViewChanges(/*IApplicationViewChangeListener*/ IUnknown* pListener, DWORD* dwCookie) = 0;
		virtual HRESULT __stdcall UnregisterForApplicationViewChanges(DWORD dwCookie) = 0;
	};

	// ===[ IVirtualDesktop ]=============

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
	struct __declspec(uuid("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")) IVirtualDesktop: public IUnknown {
		virtual HRESULT __stdcall IsViewVisible(IUnknown* pView, int* pfVisible) = 0;
		virtual HRESULT __stdcall GetID(winrt::guid* pGuid) = 0;
	};

	// Add this in Windows 10 Version 2004
	//
	// Supported OS: Build 18963? and later
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	struct __declspec(uuid("31EBDE3F-6EC3-4CBD-B9FB-0EF6D09B41F4")) IVirtualDesktop2: public IVirtualDesktop {
		virtual HRESULT __stdcall GetName(HSTRING* pName) = 0;
	};

	// Supported OS: Build 20231-20236
	struct __declspec(uuid("C863F05A-1378-4F55-9F8C-6B4C00EA03FC")) IVirtualDesktop20231: public IUnknown {
		virtual HRESULT __stdcall IsViewVisible(IUnknown* pView, int* pfVisible) = 0;
		virtual HRESULT __stdcall GetID(winrt::guid* pGuid) = 0;
		virtual HRESULT __stdcall GetMonitor(HMONITOR hMonitor) = 0;
		virtual HRESULT __stdcall GetName(HSTRING* pName) = 0;
	};

	// Supported OS: Build 20241-20348 (change UUID only, vtable is the same to IVirtualDesktop20231)
	struct __declspec(uuid("62FDF88B-11CA-4AFB-8BD8-2296DFAE49E2")) IVirtualDesktop20241: public IVirtualDesktop20231 {
	};

	// Supported OS: Build 21313 and later
	// - [22000] Windows 11 Version 21H2
	// - [22621] Windows 11 Version 22H2
	struct __declspec(uuid("536D3495-B208-4CC9-AE26-DE8111275BF8")) IVirtualDesktop21313: public IVirtualDesktop20241 {
		virtual HRESULT __stdcall GetWallpaper(HSTRING* pName) = 0;
	};

	// Supported OS: Build 22621.2215 and later
	// - [22621.2215] Windows 11 Version 22H2 + KB5029351
	struct __declspec(uuid("3F07F4BE-B107-441A-AF0F-39D82529072C")) IVirtualDesktop22621_2215: public IUnknown {
		virtual HRESULT __stdcall IsViewVisible(IUnknown* pView, int* pfVisible) = 0;
		virtual HRESULT __stdcall GetID(winrt::guid* pGuid) = 0;
		virtual HRESULT __stdcall GetName(HSTRING* pName) = 0;
		virtual HRESULT __stdcall GetWallpaper(HSTRING* pName) = 0;
		virtual HRESULT __stdcall IsRemote(int* pfRemote) = 0;
	};

	// ===[ IVirtualDesktopManagerInternal ]=============

	// Supported OS: Build 10240 and later (Build 10158 isn't working)
	//struct __declspec(uuid("AF8DA486-95BB-4460-B3B7-6E7A6B2962B5")) IVirtualDesktopManagerInternal10240: public IUnknown {
	//	virtual HRESULT __stdcall GetCount(int* pCount) = 0;
	//	virtual HRESULT __stdcall MoveViewToDesktop(IUnknown* pView, IVirtualDesktop* pDesktop) = 0;
	//	virtual HRESULT __stdcall CanViewMoveDesktops(IUnknown* pView, BOOL* bCanMove) = 0;
	//	virtual HRESULT __stdcall GetCurrentDesktop(IVirtualDesktop** ppDesktop) = 0;
	//	virtual HRESULT __stdcall GetDesktops(IObjectArray** ppArray) = 0;
	//	virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) = 0;
	//	virtual HRESULT __stdcall SwitchDesktop(IVirtualDesktop* pDesktop) = 0;
	//	virtual HRESULT __stdcall CreateDesktopW(IVirtualDesktop** ppDesktop) = 0;
	//	virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop* pRemoveDesktop, IVirtualDesktop* pFallbackDesktop) = 0;
	//	virtual HRESULT __stdcall FindDesktop(winrt::guid desktopId, IVirtualDesktop** ppDesktop) = 0;
	//};

	// Supported OS: Build 10130 and later
	// - [10586] November Update      (1511) / Threshold 2
	//struct __declspec(uuid("EF9F1A6C-D3CC-4358-B712-F84B635BEBE7")) IVirtualDesktopManagerInternal10130: public IVirtualDesktopManagerInternal10130 {
	//};

	// Supported OS: Build 14328 and later
	// - [14393] Aniversary Update    (1607) / Redstone 1
	// - [15063] Creators Update      (1703) / Redstone 2
	// - [16299] Fall Creators Update (1709) / Redstone 3
	// - [17134] April 2018 Update    (1803) / Redstone 4
	// - [17763] October 2018 Update  (1809) / Redstone 5
	// - [18362] May 2019 Update      (1903) / 19H1
	// - [18363] November 2019 Update (1909) / 19H2
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	struct __declspec(uuid("F31574D6-B682-4CDC-BD56-1827860ABEC6")) IVirtualDesktopManagerInternal14328: public IUnknown {
		virtual HRESULT __stdcall GetCount(int* pCount) = 0;
		virtual HRESULT __stdcall MoveViewToDesktop(/*IApplicationView*/ IUnknown* pView, IVirtualDesktop* pDesktop) = 0;
		virtual HRESULT __stdcall CanViewMoveDesktops(/*IApplicationView*/ IUnknown* pView, BOOL* bCanMove) = 0;
		virtual HRESULT __stdcall GetCurrentDesktop(IVirtualDesktop** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktops(IObjectArray** ppArray) = 0;
		virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) = 0;
		virtual HRESULT __stdcall SwitchDesktop(IVirtualDesktop* pDesktop) = 0;
		virtual HRESULT __stdcall CreateDesktopW(IVirtualDesktop** ppDesktop) = 0;
		virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop* pRemoveDesktop, IVirtualDesktop* pFallbackDesktop) = 0;
		virtual HRESULT __stdcall FindDesktop(winrt::guid desktopId, IVirtualDesktop** ppDesktop) = 0;
	};

	// Supported OS: Build 18963? and later
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	struct __declspec(uuid("0F3A72B0-4566-487E-9A33-4ED302F6D6CE")) IVirtualDesktopManagerInternal14328_2: public IVirtualDesktopManagerInternal14328 {
		virtual HRESULT __stdcall GetDesktopSwitchIncludeExcludeViews(IVirtualDesktop* pDesktop, IObjectArray** ppLeftDesktops, IObjectArray** ppRightDesktops) = 0;
		virtual HRESULT __stdcall SetDesktopName(IVirtualDesktop* pDesktop, HSTRING pName) = 0;
	};

	// Supported OS: Build 20206 and later
	struct __declspec(uuid("FE538FF5-D53B-4F5A-9DAD-8E72873CB360")) IVirtualDesktopManagerInternal14328_3: public IVirtualDesktopManagerInternal14328_2 {
		virtual HRESULT __stdcall CopyDesktopState(/*IApplicationView*/ IUnknown* pView0, /*IApplicationView*/ IUnknown* pView1) = 0;
	};

	// Supported OS: Build 20231-20236
	struct __declspec(uuid("B1AD6220-8B03-4345-B9FD-D8E6A8BAABAB")) IVirtualDesktopManagerInternal20231: public IUnknown {
		virtual HRESULT __stdcall GetCount(HMONITOR hMonitor, int* pCount) = 0;
		virtual HRESULT __stdcall MoveViewToDesktop(/*IApplicationView*/ IUnknown* pView, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall CanViewMoveDesktops(/*IApplicationView*/ IUnknown* pView, BOOL* bCanMove) = 0;
		virtual HRESULT __stdcall GetCurrentDesktop(HMONITOR hMonitor, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktops(HMONITOR hMonitor, IObjectArray** ppArray) = 0;
		virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop20231* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall SwitchDesktop(HMONITOR hMonitor, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall CreateDesktopW(HMONITOR hMonitor, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop20231* pRemoveDesktop, IVirtualDesktop20231* pFallbackDesktop) = 0;
		virtual HRESULT __stdcall FindDesktop(winrt::guid desktopId, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktopSwitchIncludeExcludeViews(IVirtualDesktop20231* pDesktop, IObjectArray** ppLeftDesktops, IObjectArray** ppRightDesktops) = 0;
		virtual HRESULT __stdcall SetDesktopName(IVirtualDesktop20231* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall SetDesktopWallpaper(IVirtualDesktop20231* pDesktop, HSTRING path) = 0;
		virtual HRESULT __stdcall CopyDesktopState(/*IApplicationView*/ IUnknown* pView0, /*IApplicationView*/ IUnknown* pView1) = 0;
		virtual HRESULT __stdcall GetDesktopIsPerMonitor(BOOL* bPerMonitor) = 0;
	};

	// Supported OS: Build 20241-20279 (change UUID only, vtable is the same to IVirtualDesktopManagerInternal20231)
	struct __declspec(uuid("094AFE11-44F2-4BA0-976F-29A97E263EE0")) IVirtualDesktopManagerInternal20241: public IVirtualDesktopManagerInternal20231 {
	};

	// Supported OS: Build 21313 and later
	struct __declspec(uuid("B2F925B9-5A0F-4D2E-9F4D-2B1507593C10")) IVirtualDesktopManagerInternal21313: public IUnknown {
		virtual HRESULT __stdcall GetCount(HMONITOR hMonitor, int* pCount) = 0;
		virtual HRESULT __stdcall MoveViewToDesktop(/*IApplicationView*/ IUnknown* pView, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall CanViewMoveDesktops(/*IApplicationView*/ IUnknown* pView, BOOL* bCanMove) = 0;
		virtual HRESULT __stdcall GetCurrentDesktop(HMONITOR hMonitor, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktops(HMONITOR hMonitor, IObjectArray** ppArray) = 0;
		virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop20231* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall SwitchDesktop(HMONITOR hMonitor, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall CreateDesktopW(HMONITOR hMonitor, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall MoveDesktop(IVirtualDesktop20231* pMoveDesktop, HMONITOR hMonitor, int nIndex) = 0; // Add this in build 21313
		virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop20231* pRemoveDesktop, IVirtualDesktop20231* pFallbackDesktop) = 0;
		virtual HRESULT __stdcall FindDesktop(winrt::guid desktopId, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktopSwitchIncludeExcludeViews(IVirtualDesktop20231* pDesktop, IObjectArray** ppLeftDesktops, IObjectArray** ppRightDesktops) = 0;
		virtual HRESULT __stdcall SetDesktopName(IVirtualDesktop20231* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall SetDesktopWallpaper(IVirtualDesktop20231* pDesktop, HSTRING path) = 0;
		virtual HRESULT __stdcall UpdateWallpaperPathForAllDesktops(HSTRING text) = 0;
		virtual HRESULT __stdcall CopyDesktopState(/*IApplicationView*/ IUnknown* pView0, /*IApplicationView*/ IUnknown* pView1) = 0;
		virtual HRESULT __stdcall GetDesktopIsPerMonitor(BOOL* bPerMonitor) = 0;
	};

	// Supported OS: Build 21359 and later
	// - [22000] Windows 11 Version 21H2
	struct __declspec(uuid("B2F925B9-5A0F-4D2E-9F4D-2B1507593C10")) IVirtualDesktopManagerInternal21359: public IVirtualDesktopManagerInternal21313 {
		virtual HRESULT __stdcall SetDesktopIsPerMonitor(BOOL state) = 0; // Add this in build 21359
	};

	// Supported OS: Build 22449 and later
	// - [22621] Windows 11 Version 22H2
	struct __declspec(uuid("B2F925B9-5A0F-4D2E-9F4D-2B1507593C10")) IVirtualDesktopManagerInternal22449: public IUnknown {
		virtual HRESULT __stdcall GetCount(HMONITOR hMonitor, int* pCount) = 0;
		virtual HRESULT __stdcall MoveViewToDesktop(/*IApplicationView*/ IUnknown* pView, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall CanViewMoveDesktops(/*IApplicationView*/ IUnknown* pView, BOOL* bCanMove) = 0;
		virtual HRESULT __stdcall GetCurrentDesktop(HMONITOR hMonitor, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall GetAllCurrentDesktops(IObjectArray** ppArray) = 0; // Add this in build 22449
		virtual HRESULT __stdcall GetDesktops(HMONITOR hMonitor, IObjectArray** ppArray) = 0;
		virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop20231* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall SwitchDesktop(HMONITOR hMonitor, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall CreateDesktopW(HMONITOR hMonitor, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall MoveDesktop(IVirtualDesktop20231* pMoveDesktop, HMONITOR hMonitor, int nIndex) = 0;
		virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop20231* pRemoveDesktop, IVirtualDesktop20231* pFallbackDesktop) = 0;
		virtual HRESULT __stdcall FindDesktop(winrt::guid desktopId, IVirtualDesktop20231** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktopSwitchIncludeExcludeViews(IVirtualDesktop20231* pDesktop, IObjectArray** ppLeftDesktops, IObjectArray** ppRightDesktops) = 0;
		virtual HRESULT __stdcall SetDesktopName(IVirtualDesktop20231* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall SetDesktopWallpaper(IVirtualDesktop20231* pDesktop, HSTRING path) = 0;
		virtual HRESULT __stdcall UpdateWallpaperPathForAllDesktops(HSTRING text) = 0;
		virtual HRESULT __stdcall CopyDesktopState(/*IApplicationView*/ IUnknown* pView0, /*IApplicationView*/ IUnknown* pView1) = 0;
		virtual HRESULT __stdcall GetDesktopIsPerMonitor(BOOL* bPerMonitor) = 0;
		virtual HRESULT __stdcall SetDesktopIsPerMonitor(BOOL state) = 0;
	};

	// Supported OS: Build 22621.2215 and later
	// - [22621.2215] Windows 11 Version 22H2 + KB5029351
	struct __declspec(uuid("A3175F2D-239C-4BD2-8AA0-EEBA8B0B138E")) IVirtualDesktopManagerInternal22621_2215: public IUnknown {
		virtual HRESULT __stdcall GetCount(HMONITOR hMonitor, int* pCount) = 0;
		virtual HRESULT __stdcall MoveViewToDesktop(/*IApplicationView*/ IUnknown* pView, IVirtualDesktop22621_2215* pDesktop) = 0;
		virtual HRESULT __stdcall CanViewMoveDesktops(/*IApplicationView*/ IUnknown* pView, BOOL* bCanMove) = 0;
		virtual HRESULT __stdcall GetCurrentDesktop(HMONITOR hMonitor, IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktops(HMONITOR hMonitor, IObjectArray** ppArray) = 0;
		virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop22621_2215* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall SwitchDesktop(HMONITOR hMonitor, IVirtualDesktop22621_2215* pDesktop) = 0;
		virtual HRESULT __stdcall CreateDesktopW(HMONITOR hMonitor, IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall MoveDesktop(IVirtualDesktop22621_2215* pMoveDesktop, HMONITOR hMonitor, int nIndex) = 0;
		virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop22621_2215* pRemoveDesktop, IVirtualDesktop22621_2215* pFallbackDesktop) = 0;
		virtual HRESULT __stdcall FindDesktop(winrt::guid desktopId, IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall GetDesktopSwitchIncludeExcludeViews(IVirtualDesktop22621_2215* pDesktop, IObjectArray** ppLeftDesktops, IObjectArray** ppRightDesktops) = 0;
		virtual HRESULT __stdcall SetDesktopName(IVirtualDesktop22621_2215* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall SetDesktopWallpaper(IVirtualDesktop22621_2215* pDesktop, HSTRING path) = 0;
		virtual HRESULT __stdcall UpdateWallpaperPathForAllDesktops(HSTRING text) = 0;
		virtual HRESULT __stdcall CopyDesktopState(/*IApplicationView*/ IUnknown* pView0, /*IApplicationView*/ IUnknown* pView1) = 0;
		virtual HRESULT __stdcall CreateRemoteDesktop(HSTRING text, IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall SwitchRemoteDesktop(IVirtualDesktop22621_2215* pDesktop) = 0;
		virtual HRESULT __stdcall SwitchDesktopWithAnimation(IVirtualDesktop22621_2215* pDesktop) = 0;
		virtual HRESULT __stdcall GetLastActiveDesktop(IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall WaitForAnimationToComplete() = 0;
	};

	// Supported OS: Build 25158 and later (change UUID only, vtable is the same to IVirtualDesktopManagerInternal22449)
	struct __declspec(uuid("88846798-1611-4D18-946B-4A67BFF58C1B")) IVirtualDesktopManagerInternal25158: public IVirtualDesktopManagerInternal22449 {
	};

	// ===[ IVirtualDesktopNotification ]=============

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
	struct __declspec(uuid("C179334C-4295-40D3-BEA1-C654D965605A")) IVirtualDesktopNotification: public IUnknown {
		virtual HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktop* pDesktop) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
		virtual HRESULT __stdcall ViewVirtualDesktopChanged(IUnknown* pView) = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) = 0;
	};

	// Add this in Windows 10 Version 2004
	//
	// Supported OS: Build 18963? and later
	// - [19041] May 2020 Update      (2004) / 20H1
	// - [19042] October 2020 Update  (20H2) / 20H2
	// - [19043] May 2021 Update      (21H1) / 21H1
	// - [19044] November 2021 Update (21H2) / 21H2
	// - [19045] 2022 Update          (22H2) / 22H2
	struct __declspec(uuid("1BA7CF30-3591-43FA-ABFA-4AAF7ABEEDB7")) IVirtualDesktopNotification2: public IVirtualDesktopNotification {
		virtual HRESULT __stdcall VirtualDesktopNameChanged(IVirtualDesktop2* pDesktop, HSTRING name) = 0;
	};

	// Supported OS: Build 20231-20236
	struct __declspec(uuid("5DAFEE84-F7D9-4CD1-B646-64AFC9EA4E22")) IVirtualDesktopNotification20231: public IUnknown {
		virtual HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopIsPerMonitorChanged(BOOL bPerMonitor) = 0;
		virtual HRESULT __stdcall VirtualDesktopNameChanged(IVirtualDesktop20231* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall ViewVirtualDesktopChanged(IUnknown* pView) = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktop20231* pDesktopOld, IVirtualDesktop20231* pDesktopNew) = 0;
	};

	// Supported OS: Build 20241-20349 (change UUID only, vtable is the same to IVirtualDesktopNotification20231)
	struct __declspec(uuid("F3163E11-6B04-433C-A64B-6F82C9094257")) IVirtualDesktopNotification20241: public IVirtualDesktopNotification20231 {
	};

	// Supported OS: Build 21359 and later
	struct __declspec(uuid("CD403E52-DEED-4C13-B437-B98380F2B1E8")) IVirtualDesktopNotification21359: public IUnknown {
		virtual HRESULT __stdcall VirtualDesktopCreated(IObjectArray* pArray, IVirtualDesktop20231* pDesktop) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopIsPerMonitorChanged(BOOL bPerMonitor) = 0;
		virtual HRESULT __stdcall VirtualDesktopMoved(IObjectArray* pArray, IVirtualDesktop20231* pDesktop, int nFromIndex, int nToIndex) = 0;
		virtual HRESULT __stdcall VirtualDesktopNameChanged(IVirtualDesktop20231* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall ViewVirtualDesktopChanged(/*IApplicationView*/ IUnknown* pView) = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IObjectArray* pArray, IVirtualDesktop20231* pDesktopOld, IVirtualDesktop20231* pDesktopNew) = 0;
		virtual HRESULT __stdcall VirtualDesktopWallpaperChanged(IVirtualDesktop20231* pDesktop, HSTRING path) = 0;
	};

	// Supported OS: Build 22621.2215 and later
	struct __declspec(uuid("B287FA1C-7771-471A-A2DF-9B6B21F0D675")) IVirtualDesktopNotification22621_2215: public IUnknown {
		virtual HRESULT __stdcall VirtualDesktopCreated(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktop) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopDestroyed(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* pDesktopFallback) = 0;
		virtual HRESULT __stdcall VirtualDesktopMoved(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktop, int nFromIndex, int nToIndex) = 0;
		virtual HRESULT __stdcall VirtualDesktopNameChanged(IVirtualDesktop22621_2215* pDesktop, HSTRING name) = 0;
		virtual HRESULT __stdcall ViewVirtualDesktopChanged(/*IApplicationView*/ IUnknown* pView) = 0;
		virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopOld, IVirtualDesktop22621_2215* pDesktopNew) = 0;
		virtual HRESULT __stdcall VirtualDesktopWallpaperChanged(IVirtualDesktop22621_2215* pDesktop, HSTRING path) = 0;
		virtual HRESULT __stdcall VirtualDesktopSwitched(IVirtualDesktop22621_2215** ppDesktop) = 0;
		virtual HRESULT __stdcall RemoteVirtualDesktopConnected(IVirtualDesktop22621_2215** ppDesktop) = 0;
	};

	// ===[ IVirtualDesktopNotificationService ]=============

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
	// - [22000] Windows 11 Version 21H2
	// - [22621] Windows 11 Version 22H2
	struct __declspec(uuid("0CD45E71-D927-4F15-8B0A-8FEF525337BF")) IVirtualDesktopNotificationService: public IUnknown {
		virtual HRESULT __stdcall Register(IUnknown* pNotification, DWORD* dwCookie) noexcept = 0;
		virtual HRESULT __stdcall Unregister(DWORD dwCookie) noexcept = 0;
	};

}

//namespace winrt::impl {
	//template <> inline constexpr guid guid_v<IVirtualDesktopNotification>{ 0xC179334C, 0x4295, 0x40D3, { 0xBE,0xA1,0xC6,0x54,0xD9,0x65,0x60,0x5A } };
	//template <> inline constexpr guid guid_v<IVirtualDesktopNotification2>{ 0x1BA7CF30, 0x3591, 0x43FA, { 0xAB,0xFA,0x4A,0xAF,0x7A,0xBE,0xED,0xB7 } };
	//template <> inline constexpr guid guid_v<IVirtualDesktopNotificationService>{ 0x0CD45E71, 0xD927, 0x4F15, { 0x8B,0x0A,0x8F,0xEF,0x52,0x53,0x37,0xBF } };
//}
