#pragma once
#include "VirtualDesktopNotificationServiceImpl.h"

#include "Common/VirtualDesktop.h"
#include "Common/Reps.h"

namespace app::win11 {

	struct VirtualDesktopNotificationServiceWin11
		: public app::IVirtualDesktopNotificationServiceImpl
		, winrt::implements<VirtualDesktopNotificationServiceWin11, IVirtualDesktopNotification> {
		VirtualDesktopNotificationServiceWin11(reps::observer_t& observer);

		void close() override;

		// - IVirtualDesktopNotification
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(Unknown1)(int nUnknown);
		IFACEMETHOD(VirtualDesktopMoved)(IVirtualDesktop* pDesktop, int nFromIndex, int nToIndex);
		IFACEMETHOD(VirtualDesktopRenamed)(IVirtualDesktop* pDesktop, HSTRING name);
		IFACEMETHOD(ViewVirtualDesktopChanged)(void* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);
		IFACEMETHOD(VirtualDesktopWallpaperChanged)(IVirtualDesktop* pDesktop, HSTRING path);

	private:
		winrt::com_ptr<IServiceProvider> serviceProvider_;
		winrt::com_ptr<IVirtualDesktopNotificationService> virtualDesktopNotificationService_;
		DWORD cookie_;
		reps::single_subject_t subject_;
	};

}
