#pragma once
#include "DesktopServiceImpl.h"

#include "Common/VirtualDesktop.h"
#include "Common/Reps.h"

namespace app::win11 {

	struct IVirtualDesktopManagerInternalDelegate {
		virtual ~IVirtualDesktopManagerInternalDelegate() { }

		virtual HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktop* pDesktop) noexcept = 0;
		virtual HRESULT GetCurrentDesktop(IVirtualDesktop** ppDesktop) noexcept = 0;
		virtual HRESULT GetDesktops(IObjectArray** ppArray) noexcept = 0;
		virtual HRESULT GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) noexcept = 0;
	};

	struct DesktopServiceImplWin11
		: public app::IDesktopServiceImpl
		, winrt::implements<DesktopServiceImplWin11, IVirtualDesktopNotification> {
		DesktopServiceImplWin11(DWORD build, reps::observer_t& observer);

		void close() override;

		// - Operations
		void moveForegroundWindowToLeftOfCurrent() const override;
		void moveForegroundWindowToRightOfCurrent() const override;

		// - IVirtualDesktopNotification
		IFACEMETHOD(VirtualDesktopCreated)(IObjectArray* pArray, IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IObjectArray* pArray, IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IObjectArray* pArray, IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IObjectArray* pArray, IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(Unknown1)(int nUnknown);
		IFACEMETHOD(VirtualDesktopMoved)(IObjectArray* pArray, IVirtualDesktop* pDesktop, int nFromIndex, int nToIndex);
		IFACEMETHOD(VirtualDesktopRenamed)(IVirtualDesktop* pDesktop, HSTRING name);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IObjectArray* pArray, IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);
		IFACEMETHOD(VirtualDesktopWallpaperChanged)(IVirtualDesktop* pDesktop, HSTRING path);

	private:
		void loadDesktops();

	private:
		winrt::com_ptr<IServiceProvider> serviceProvider_;
		winrt::com_ptr<IApplicationViewCollection> applicationViewCollection_;
		std::unique_ptr<IVirtualDesktopManagerInternalDelegate> virtualDesktopManagerDelegate_;
		winrt::com_ptr<IVirtualDesktopNotificationService> virtualDesktopNotificationService_;
		DWORD cookie_;
		reps::single_subject_t subject_;

		std::unordered_map<winrt::guid, VirtualDesktopBag> bag_;
	};

}
