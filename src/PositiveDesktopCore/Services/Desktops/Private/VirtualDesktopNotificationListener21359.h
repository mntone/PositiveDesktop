#pragma once
#include "Common/VirtualDesktop.h"
#include "../VirtualDesktopCache.h"
#include "../VirtualDesktopInterface.h"

namespace app::desktop {

	struct VirtualDesktopNotificationListener21359
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener21359, IVirtualDesktopNotification21359> {
		VirtualDesktopNotificationListener21359(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

	private:
		// - IVirtualDesktopNotification21359
		IFACEMETHOD(VirtualDesktopCreated)(IObjectArray* pArray, IVirtualDesktop20231* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(Unknown1)(int nUnknown);
		IFACEMETHOD(VirtualDesktopMoved)(IObjectArray* pArray, IVirtualDesktop20231* pDesktop, int nFromIndex, int nToIndex);
		IFACEMETHOD(VirtualDesktopRenamed)(IVirtualDesktop20231* pDesktop, HSTRING name);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopOld, IVirtualDesktop20231* pDesktopNew);
		IFACEMETHOD(VirtualDesktopWallpaperChanged)(IVirtualDesktop20231* pDesktop, HSTRING path);

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		IVirtualDesktopNotificationCallback* callback_;
	};

}
